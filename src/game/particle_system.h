#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H
#include <PR/ultratypes.h>
#include "types.h"

struct ParticalMaterial {
    u8 frameAmount;
    Gfx **frames;
    Gfx *revert;
    Gfx *displayList;
};

struct FastParticle {
    struct ParticalMaterial *material;
    u8 despawnTime;
    u8 flags;
    u8 matIncrement;
    u8 opacity;
    s8 opacityVel;
    u8 r, g, b;
    u8 tR, tG, tB;
    Vec3f pos;
    Vec3s angles;
    s16 angleVel;
    f32 scale;
    f32 scaleVel;
    f32 forwardVel, velY;
    f32 maxYVel;
    f32 dragStrength;
    f32 gravity;
};

enum ParticleFlags {
    PARTICLE_FLAG_BILLBOARD = BIT(0),
    PARTICLE_FLAG_SCALE_VEL_BY_OPACITY_PROPS = BIT(1),
    PARTICLE_FLAG_SCALE_VEL_BY_OPACITY_PROPS_REVERSE = BIT(2),
};

struct SpawnFastParticlesInfo {
    s8 count;
    u8 despawnTime;

    u8 flags;
    
    struct ParticalMaterial *material;

    s8 offsetY;
    s8 lateralTransRange;

    u8 opacity;
    s8 opacityVel;

    // Starting rgb values
    u8 r, g, b;
    // Target rgb values (fully reached when despawn time reaches 0).
    u8 tR, tG, tB;

    s16 angleVel;

    f32 forwardVelBase, forwardVelRange;

    f32 velYBase, velYRange, maxYVel;
    f32 gravity;

    f32 dragStrength;

    f32 scaleBase, scaleRange, scaleVel;
};

#define PARTICLE_ARRAY_SIZE 50

void render_particles(s32 gPhysicAdvances);
void spawn_fast_particle(Vec3f pos, struct SpawnFastParticlesInfo info);
void run_particle_physics(struct FastParticle* curParticle, s32 physicAdvances, f32 f32PhysicAdvances);

#endif // PARTICLE_SYSTEM_H
