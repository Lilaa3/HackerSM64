
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

extern const Gfx init_rsp[];

// Renders and applies physics to particles. 
// Should be called before geo processing so particles be rendered first.
ALWAYS_INLINE void render_particles(s32 physicAdvances) {
    f32 mtx[16];
    curMaterial = NULL;
    f32 f32PhysicAdvances = (f32) physicAdvances;

	Gfx *gfxStart = alloc_display_list(sizeof(Gfx) * ((4 * PARTICLE_ARRAY_SIZE) + 3));
	Mtx *mtxStart = alloc_display_list(sizeof(Mtx) * PARTICLE_ARRAY_SIZE);

	Gfx *gfxHead = gfxStart;
    Mtx *mtxHead = mtxStart;

    gSPDisplayList(gfxHead++, init_rsp);

    for (s32 i = 0; i < PARTICLE_ARRAY_SIZE; i++) {
        struct FastParticle* curParticle = &particleArray[i];

        if (curParticle->despawnTime > 0) {
            // Check if the current particle is a billboard, if so use the billboarding function and ignore the first 2 angles.
            // If not, use the rotate and translate function. 
            if (curParticle->isBillboard == TRUE) {
				Vec3f scale = {curParticle->scale, curParticle->scale, curParticle->scale};
                mtxf_billboard(mtx, gMatStack[0], curParticle->pos, scale, curParticle->angles[2]);
            } else {
                mtxf_rotate_zxy_and_translate(mtx, curParticle->pos, &curParticle->angles);

				for (s32 j = 0; j < 4; j++) {
					mtx[j + 0] *= curParticle->scale;
					mtx[j + 4] *= curParticle->scale;
					mtx[j + 8] *= curParticle->scale;
				}
            }

            mtxf_to_mtx(mtxHead, mtx); // Converts floating point matrix to a fixed point matrix.

            gDPSetEnvColor(gfxHead++, curParticle->r, curParticle->g, curParticle->b, curParticle->opacity); // Sets enviroment colour.
            if (curMaterial != curParticle->material || curParticle->matIncrement != curMatIncrement) {
                if (curParticle->matIncrement >= curParticle->material->frameAmount){
                    curParticle->matIncrement = 0;
                }

                gSPDisplayList(gfxHead++, curParticle->material->frames[curParticle->matIncrement]); // Sets material.

                curMaterial = curParticle->material;
                curMatIncrement = curParticle->matIncrement;
            }

			gSPMatrix(gfxHead++, mtxHead++, G_MTX_NOPUSH | G_MTX_LOAD); // Applies matrix transformation. 
																		// No push means that it is not need to pop the matrix.

            gSPDisplayList(gfxHead++, curParticle->material->displayList);

            // Executes physics, allows for up to 60 fps. 30 fps = 2 physics advances, 60 fps = 1.
            run_particle_physics(curParticle, physicAdvances, f32PhysicAdvances);
        }
    }
	
	gSPDisplayList(gfxHead++, init_rsp);
	gSPEndDisplayList(gfxHead);

	geo_append_display_list(gfxStart, LAYER_TRANSPARENT); // Adds the display list to the transperant layer display list.
}

ALWAYS_INLINE void run_particle_physics(struct FastParticle* curParticle, s32 physicAdvances, f32 f32PhysicAdvances){
    curParticle->matIncrement += physicAdvances;

    curParticle->angles[2] += curParticle->angleVel;

    curParticle->velY += curParticle->gravity;

    if(curParticle->velY > curParticle->maxYVel){
        curParticle->velY = curParticle->maxYVel;
    }

    curParticle->pos[0] += sins(curParticle->angles[1]) * curParticle->forwardVel * f32PhysicAdvances;
    curParticle->pos[1] += curParticle->velY * f32PhysicAdvances;
    curParticle->pos[2] += coss(curParticle->angles[1]) * curParticle->forwardVel * f32PhysicAdvances;

    apply_drag_to_value(&curParticle->forwardVel, curParticle->dragStrength * f32PhysicAdvances);
    apply_drag_to_value(&curParticle->velY, curParticle->dragStrength * f32PhysicAdvances);
    
    curParticle->opacity = CLAMP_U8(curParticle->opacity + ( curParticle->opacityVel * physicAdvances));
    f32 fraction = (1.f / curParticle->despawnTime) * f32PhysicAdvances;
    curParticle->r =  CLAMP_U8((curParticle->tR - curParticle->r) * fraction + curParticle->r);
    curParticle->g =  CLAMP_U8((curParticle->tG - curParticle->g) * fraction + curParticle->g);
    curParticle->b =  CLAMP_U8((curParticle->tB - curParticle->b) * fraction + curParticle->b);

    curParticle->scale += curParticle->scaleVel * f32PhysicAdvances;

	curParticle->despawnTime -= physicAdvances;
}

void spawn_fast_particle(Vec3f pos, struct SpawnFastParticlesInfo info) {
    s32 i;

    for (i = 0; i < info.count; i++) {
        struct FastParticle *curParticle = &particleArray[curParticleAddID];
        
        curParticle->despawnTime = info.despawnTime + 1;

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