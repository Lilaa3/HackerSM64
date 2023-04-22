
#include "rendering_graph_node.h"
#include "engine/math_util.h"
#include <PR/ultratypes.h>
#include <PR/gbi.h>

#include "config.h"
#include "macros.h"
#include "types.h"
#include "game/ingame_menu.h"

#include "make_const_nonconst.h"
#include "particle_system.h"

ALIGNED16 struct FastParticle particleArray[PARTICLE_ARRAY_SIZE];
u8 curParticleAddID = 0;

s16 particleAmount = 0;

struct ParticalMaterial *curMaterial; // switch materials if curMaterial does not match previous one
u8 curMatIncrement = 0;

extern Gfx *gDisplayListHeap;
extern ALIGNED16 Mat4 gMatStack[];
extern ALIGNED16 Mat4 gMatStackFixed[];
extern u8 gMatStackIndex;

ALWAYS_INLINE void mtxf_billboard_fast(s16 angle, Vec3f trans, Mat4 dest, Mat4 src) {
    Mat4* cameraMat = &gCameraTransform;
    for (s32 i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            dest[i][j] = (*cameraMat)[j][i];
        }
        dest[i][3] = 0.0f;
    }
    if (angle != 0x0) {
        float m00 = dest[0][0]; 
        float m01 = dest[0][1];
        float m02 = dest[0][2];
        float m10 = dest[1][0];
        float m11 = dest[1][1];
        float m12 = dest[1][2];
        float cosa = coss(angle);
        float sina = sins(angle);
        dest[0][0] = cosa * m00 + sina * m10; 
        dest[0][1] = cosa * m01 + sina * m11; 
        dest[0][2] = cosa * m02 + sina * m12;
        dest[1][0] = -sina * m00 + cosa * m10;
        dest[1][1] = -sina * m01 + cosa * m11;
        dest[1][2] = -sina * m02 + cosa * m12;
    }
    vec3f_copy(dest[3], trans);
    vec3f_add(dest[3], src[3]);
    MTXF_END(dest);
}

void mtxf_rotate_zxy_and_mul_fast(Vec3s rot, Vec3f trans, Mat4 dest, Mat4 src) {
    register f32 sx = sins(rot[0]);
    register f32 cx = coss(rot[0]);
    register f32 sy = sins(rot[1]);
    register f32 cy = coss(rot[1]);
    register f32 sz = sins(rot[2]);
    register f32 cz = coss(rot[2]);
    Vec3f entry;
    register f32 sysz = (sy * sz);
    register f32 cycz = (cy * cz);
    entry[0] = ((sysz * sx) + cycz);
    entry[1] = (sz * cx);
    register f32 cysz = (cy * sz);
    register f32 sycz = (sy * cz);
    entry[2] = ((cysz * sx) - sycz);
    linear_mtxf_mul_vec3f(src, dest[0], entry);
    entry[0] = ((sycz * sx) - cysz);
    entry[1] = (cz * cx);
    entry[2] = ((cycz * sx) + sysz);
    linear_mtxf_mul_vec3f(src, dest[1], entry);
    entry[0] = (cx * sy);
    entry[1] = -sx;
    entry[2] = (cx * cy);
    linear_mtxf_mul_vec3f(src, dest[2], entry);
    linear_mtxf_mul_vec3f(src, dest[3], trans);
    vec3f_add(dest[3], src[3]);
    MTXF_END(dest);
}

extern void apply_drag_to_value(f32 *value, f32 dragStrength);
extern void geo_append_display_list(void *displayList, s32 layer);

// Renders and applies physics to particles. 
// Should be called before geo processing so particles be rendered first.
ALWAYS_INLINE void render_particles(s32 physicAdvances) {
    f32* destMtx = gMatStack[gMatStackIndex + 1];
    f32* srcMtx = gMatStack[gMatStackIndex];

    curMaterial = NULL;
    f32 f32PhysicAdvances = (f32) physicAdvances;

	Gfx *gfxStart = alloc_display_list(sizeof(Gfx) * ((5 * PARTICLE_ARRAY_SIZE) + 2));
	Mtx *mtxStart = alloc_display_list(sizeof(Mtx) * PARTICLE_ARRAY_SIZE);

    if (!gfxStart || !mtxStart)
        print_text(30, 30, "Failed to allocate display list for particles");

	Gfx *gfxHead = gfxStart;
    Mtx *mtxHead = mtxStart;

    struct FastParticle* curParticle = NULL;
    for (u32 i = 0; i < PARTICLE_ARRAY_SIZE; i++) {
        if (particleArray[i].despawnTime == 0)
            continue;
        curParticle = &particleArray[i];
        
        s32 isSameMaterial = curMaterial == curParticle->material;
        if (!isSameMaterial) {
            gSPDisplayList(gfxHead++, curParticle->material->revert);
            curMaterial = curParticle->material;
        }
        if (!isSameMaterial || curParticle->matIncrement != curMatIncrement) {
            gSPDisplayList(gfxHead++, curParticle->material->frames[curParticle->matIncrement]); // Sets material.
            curMatIncrement = curParticle->matIncrement;
        }

        f32 fraction = (1.f / curParticle->despawnTime) * f32PhysicAdvances;
        u8 r =  CLAMP_U8((curParticle->tR - curParticle->r) * fraction + curParticle->r);
        u8 g =  CLAMP_U8((curParticle->tG - curParticle->g) * fraction + curParticle->g);
        u8 b =  CLAMP_U8((curParticle->tB - curParticle->b) * fraction + curParticle->b);
        gDPSetEnvColor(gfxHead++, r, g, b, curParticle->opacity); // Sets enviroment colour.

        // Check if the current particle is a billboard, if so use the billboarding function and ignore the first 2 angles.
        // If not, use the rotate and translate function. 

        if (curParticle->isBillboard) {
            mtxf_billboard_fast(curParticle->angles[2], curParticle->pos, destMtx, srcMtx);
        } else {
            mtxf_rotate_zxy_and_mul_fast(curParticle->angles, curParticle->pos, destMtx, srcMtx);
        }
        for (s32 i = 0; i < 3; i++) {
            destMtx[0 + i] *= curParticle->scale;
            destMtx[4 + i] *= curParticle->scale;
            destMtx[8 + i] *= curParticle->scale;
        }

        mtxf_to_mtx(mtxHead, destMtx); // Converts floating point matrix to a fixed point matrix.
        gSPMatrix(gfxHead++, mtxHead++, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW); // Applies matrix transformation. 
                                                                    // No push means that it is not need to pop the matrix.

        gSPDisplayList(gfxHead++, curParticle->material->displayList);

        // Executes physics, allows for up to 60 fps. 30 fps = 2 physics advances, 60 fps = 1.
        run_particle_physics(curParticle, physicAdvances, f32PhysicAdvances);
    }

    if (curParticle)
        gSPDisplayList(gfxHead++, curParticle->material->revert);

	gSPEndDisplayList(gfxHead);
	geo_append_display_list(gfxStart, LAYER_TRANSPARENT); // Adds the display list to the transperant layer display list.
}

ALWAYS_INLINE void run_particle_physics(struct FastParticle* curParticle, s32 physicAdvances, f32 f32PhysicAdvances){
    if (physicAdvances > curParticle->despawnTime) {
        curParticle->despawnTime = 0;
        return;
    }

    curParticle->matIncrement = CLAMP(curParticle->matIncrement + physicAdvances, 0, curParticle->material->frameAmount - 1);

    curParticle->angles[2] += curParticle->angleVel;

    apply_drag_to_value(&curParticle->forwardVel, curParticle->dragStrength * f32PhysicAdvances);
    apply_drag_to_value(&curParticle->velY, curParticle->dragStrength * f32PhysicAdvances);
    osSyncPrintf("forwardVel %d\n", (s32) curParticle->forwardVel);

    curParticle->velY += curParticle->gravity;
    if(curParticle->velY > curParticle->maxYVel){
        curParticle->velY = curParticle->maxYVel;
    }

    curParticle->pos[0] += sins(curParticle->angles[1]) * (curParticle->forwardVel * f32PhysicAdvances);
    curParticle->pos[1] += curParticle->velY * f32PhysicAdvances;
    curParticle->pos[2] += coss(curParticle->angles[1]) * (curParticle->forwardVel * f32PhysicAdvances);
    
    curParticle->opacity = CLAMP_U8(curParticle->opacity + ( curParticle->opacityVel * physicAdvances));

    curParticle->scale += curParticle->scaleVel * f32PhysicAdvances;

	curParticle->despawnTime -= physicAdvances;
}

void spawn_fast_particle(Vec3f pos, struct SpawnFastParticlesInfo info) {
    s32 i;

    for (i = 0; i < info.count; i++) {
        struct FastParticle *curParticle = &particleArray[curParticleAddID];
        
        curParticle->despawnTime = info.despawnTime;

        curParticle->material = info.material;
        
        curParticle->isBillboard = info.isBillboard;

        curParticle->opacity = info.opacity;
        curParticle->opacityVel = info.opacityVel;

        curParticle->angleVel = info.angleVel;
        curParticle->angles[0] = random_u16();
        curParticle->angles[1] = random_u16();
        curParticle->angles[2] = random_u16();
        curParticle->gravity = info.gravity;
        curParticle->dragStrength = info.dragStrength;

        curParticle->r = info.r;
        curParticle->g = info.g;
        curParticle->b = info.b;

        curParticle->tR = info.tR;
        curParticle->tG = info.tG;
        curParticle->tB = info.tB;
        
        f32 fromX = pos[0];
        f32 fromZ = pos[2];
        curParticle->pos[0] = fromX + random_f32_around_zero(info.lateralTransRange);
        curParticle->pos[2] = fromZ + random_f32_around_zero(info.lateralTransRange);
		curParticle->pos[1] = pos[1] + info.offsetY;
        
        curParticle->forwardVel = random_float() * info.forwardVelRange + info.forwardVelBase;

        curParticle->velY = random_float() * info.velYRange + info.velYBase;
        curParticle->maxYVel = info.maxYVel;

        curParticle->scale = (random_float() * info.scaleRange) + info.scaleBase;
        curParticle->scaleVel = info.scaleVel;

        curParticleAddID = (curParticleAddID + 1) % 50;
    }
}