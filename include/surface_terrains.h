#ifndef SURFACE_TERRAINS_H
#define SURFACE_TERRAINS_H

#include <PR/ultratypes.h>
#include "config.h"
#include "sounds.h"

enum SurfaceClass {
    SURFACE_CLASS_DEFAULT,
    SURFACE_CLASS_VERY_SLIPPERY,
    SURFACE_CLASS_SLIPPERY,
    SURFACE_CLASS_NOT_SLIPPERY,
    SURFACE_CLASS_SUPER_SLIPPERY,
};


#define COLLISION_TYPE(warpsAndLevel, special, slipperiness, camera, particles, sound, nonDecalShadow, vanish, canGetStuck, noCameraCollision) \
    ((nonDecalShadow << NON_DECAL_SHADOW_POS) | \
    (vanish << VANISH_POS) | \
    (canGetStuck << CAN_GET_STUCK_POS) | \
    (warpsAndLevel << WARPS_AND_LEVEL_POS) | \
    (special << SPECIAL_POS) | \
    (slipperiness << SLIPPERINESS_POS) | \
    (noCameraCollision << NO_CAMERA_COLLISION_POS) | \
    (camera << CAMERA_POS) | \
    (particles << PARTICLES_POS) | \
    (sound << SOUND_POS))

#define COLLISION_TYPE_COMMON(special, slipperiness, particles, sound) \
    COLLISION_TYPE(0, special, slipperiness, 0, particles, sound, FALSE, FALSE, FALSE, FALSE)

enum WarpsAndLevelTypes {
    COL_TYPE_LEVEL_DEFAULT,
    COL_TYPE_WARP,
    COL_TYPE_INSTANT_WARP_0,
    COL_TYPE_INSTANT_WARP_1,
    COL_TYPE_INSTANT_WARP_2,
    COL_TYPE_INSTANT_WARP_3,
    COL_TYPE_LOOK_UP_WARP,
    COL_TYPE_TIMER_START,
    COL_TYPE_TIMER_END,
    COL_TYPE_MUSIC,
    // 10
};

enum SpecialCollisionTypes {
    COL_TYPE_SPECIAL_DEFAULT,
    COL_TYPE_HANGABLE,
    COL_TYPE_INTANGIBLE,
    COL_TYPE_DEATH_PLANE,
    COL_TYPE_BURNING,
    COL_TYPE_WATER,
    COL_TYPE_WATER_BOTTOM,
    COL_TYPE_SLOW,
    // Force based
    COL_TYPE_FORCE_AS_SPEED,
    COL_TYPE_VERTICAL_WIND,
    COL_TYPE_HORIZONTAL_WIND,
    COL_TYPE_FLOWING_WATER,
    COL_TYPE_QUICKSAND,
    COL_TYPE_SHALLOW_QUICKSAND,
    COL_TYPE_DEEP_MOVING_QUICKSAND,
    COL_TYPE_MOVING_QUICKSAND,
    COL_TYPE_SHALLOW_MOVING_QUICKSAND,
    COL_TYPE_DEEP_QUICKSAND,
    COL_TYPE_INSTANT_QUICKSAND,
    COL_TYPE_INSTANT_MOVING_QUICKSAND,
    // 19
};

enum CameraCollisionTypes {
    COL_FLAG_CAMERA_DEFAULT,
    COL_TYPE_CAMERA_WALL,
    COL_TYPE_CLOSE_CAMERA,
    COL_TYPE_CAMERA_FREE_ROAM,
    COL_TYPE_BOSS_FIGHT_CAMERA,
    COL_TYPE_CAMERA_8_DIR,
    COL_TYPE_CAMERA_MIDDLE,
    COL_TYPE_CAMERA_ROTATE_RIGHT,
    COL_TYPE_CAMERA_ROTATE_LEFT,
    COL_TYPE_CAMERA_BOUNDARY, 
    // 9
};

enum ParticlesCollisionTypes {
    COL_TYPE_PARTICLE_DEFAULT,
    COL_TYPE_PARTICLE_SPARKLES,
    COL_TYPE_PARTICLE_DUST,
    COL_TYPE_PARTICLE_WATER_SPLASH,
    COL_TYPE_PARTICLE_WAVE_TRAIL,
    COL_TYPE_PARTICLE_FIRE,
    COL_TYPE_PARTICLE_SHALLOW_WATER,
    COL_TYPE_PARTICLE_LEAF,
    COL_TYPE_PARTICLE_SNOW,
    COL_TYPE_PARTICLE_BREATH,
    COL_TYPE_PARTICLE_DIRT,
    COL_TYPE_PARTICLE_TRIANGLE,
    // 13
};

#define INSTANT_WARP_INDEX_START  0x00
#define INSTANT_WARP_INDEX_STOP   0x04

// Example of default collision type macro:
// COLLISION_TYPE(0, 0, 0, 0, 0, 0, FALSE, FALSE, FALSE, FALSE)

// Surface Types
#define SURFACE_DEFAULT                         0

#define SURFACE_BURNING                         COLLISION_TYPE_COMMON(COL_TYPE_BURNING, 0, COL_TYPE_PARTICLE_FIRE, 0) // Lava / Frostbite
#define SURFACE_NEW_WATER                       COLLISION_TYPE_COMMON(COL_TYPE_WATER, 0, 0, 0)
#define SURFACE_NEW_WATER_BOTTOM                COLLISION_TYPE_COMMON(COL_TYPE_WATER_BOTTOM, 0, 0, 0)
#define SURFACE_HANGABLE                        COLLISION_TYPE_COMMON(COL_TYPE_HANGABLE, 0, 0, 0)
// Warps to ID of force parameter's second byte if set,
// otherwise warps to 0xF3 if it exists, otherwise defaults to ID 0xF1.
#define SURFACE_DEATH_PLANE                     COLLISION_TYPE_COMMON(COL_TYPE_DEATH_PLANE, 0, 0, 0)
#define SURFACE_FLOWING_WATER                   COLLISION_TYPE_COMMON(COL_TYPE_FLOWING_WATER, 0, 0, 0)
#define SURFACE_INTANGIBLE                      COLLISION_TYPE_COMMON(COL_TYPE_INTANGIBLE, 0, 0, 0)

// Plays the Merry go round usic, see handle_merry_go_round_music in bbh_merry_go_round.inc.c for
// more details
#define SURFACE_MGR_MUSIC                       COLLISION_TYPE(COL_TYPE_MUSIC, 0, 0, 0, 0, 0, FALSE, FALSE, FALSE, FALSE)

#define SURFACE_QUICKSAND                       COLLISION_TYPE_COMMON(COL_TYPE_QUICKSAND, 0, 0, 0) // (depth of 60 units)
#define SURFACE_SHALLOW_QUICKSAND               COLLISION_TYPE_COMMON(COL_TYPE_SHALLOW_QUICKSAND, 0, 0, 0) // (depth of 10 units)
#define SURFACE_DEEP_MOVING_QUICKSAND           COLLISION_TYPE_COMMON(COL_TYPE_DEEP_MOVING_QUICKSAND, 0, 0, 0) // Moving (depth of 160 units)
#define SURFACE_MOVING_QUICKSAND                COLLISION_TYPE_COMMON(COL_TYPE_MOVING_QUICKSAND, 0, 0, 0) // Moving (depth of 60 units)
#define SURFACE_SHALLOW_MOVING_QUICKSAND        COLLISION_TYPE_COMMON(COL_TYPE_SHALLOW_MOVING_QUICKSAND, 0, 0, 0) // Moving (depth of 25 units)
#define SURFACE_DEEP_QUICKSAND                  COLLISION_TYPE_COMMON(COL_TYPE_DEEP_QUICKSAND, 0, 0, 0) // Lethal, slow (depth of 160 units)
#define SURFACE_INSTANT_QUICKSAND               COLLISION_TYPE_COMMON(COL_TYPE_INSTANT_QUICKSAND, 0, 0, 0) // Lethal, instant
#define SURFACE_INSTANT_MOVING_QUICKSAND        COLLISION_TYPE_COMMON(COL_TYPE_INSTANT_MOVING_QUICKSAND, 0, 0, 0) // Moving, lethal, instant

#define SURFACE_WALL_MISC                       COLLISION_TYPE(0, 0, 0, COL_TYPE_CAMERA_WALL, 0, 0, FALSE, FALSE, FALSE, FALSE) // Cannon to adjust the camera

#define SURFACE_HORIZONTAL_WIND                 COLLISION_TYPE_COMMON(COL_TYPE_HORIZONTAL_WIND, 0, 0, 0) // Horizontal wind, has parameters
#define SURFACE_VERTICAL_WIND                   COLLISION_TYPE_COMMON(COL_TYPE_VERTICAL_WIND, 0, 0, 0) // Death at bottom with vertical wind. Warps to ID of force parameter's

#define SURFACE_TIMER_START                     COLLISION_TYPE(COL_TYPE_TIMER_START, 0, 0, 0, 0, 0, FALSE, FALSE, FALSE, FALSE)// Timer start (Peach's secret slide)
#define SURFACE_TIMER_END                       COLLISION_TYPE(COL_TYPE_TIMER_END, 0, 0, 0, 0, 0, FALSE, FALSE, FALSE, FALSE)// Timer stop (Peach's secret slide)

#define SURFACE_ICE                             COLLISION_TYPE(0, 0, SURFACE_CLASS_VERY_SLIPPERY, 0, 0, SOUND_TERRAIN_ICE, TRUE, FALSE, FALSE, FALSE)
#define SURFACE_SLOW                            COLLISION_TYPE_COMMON(COL_TYPE_SLOW, 0, 0, 0)

#define SURFACE_SUPER_SLIPPERY                  COLLISION_TYPE_COMMON(0, SURFACE_CLASS_SUPER_SLIPPERY, 0, SOUND_TERRAIN_GRASS)
#define SURFACE_VERY_SLIPPERY                   COLLISION_TYPE_COMMON(0, SURFACE_CLASS_VERY_SLIPPERY, 0, SOUND_TERRAIN_GRASS)
#define SURFACE_SLIPPERY                        COLLISION_TYPE_COMMON(0, SURFACE_CLASS_SLIPPERY, 0, SOUND_TERRAIN_GRASS)
#define SURFACE_NOT_SLIPPERY                    COLLISION_TYPE_COMMON(0, SURFACE_CLASS_NOT_SLIPPERY, 0, SOUND_TERRAIN_STONE)

#define SURFACE_NOISE_VERY_SLIPPERY             COLLISION_TYPE_COMMON(0, SURFACE_CLASS_VERY_SLIPPERY, 0, SOUND_TERRAIN_GRASS)
#define SURFACE_NOISE_SLIPPERY                  COLLISION_TYPE_COMMON(0, SURFACE_CLASS_SLIPPERY, 0, 0)
#define SURFACE_NOISE_DEFAULT                   COLLISION_TYPE_COMMON(0, 0, 0, SOUND_TERRAIN_GRASS)

#define SURFACE_HARD_VERY_SLIPPERY              SURFACE_VERY_SLIPPERY
#define SURFACE_HARD_SLIPPERY                   SURFACE_SLIPPERY
#define SURFACE_HARD_NOT_SLIPPERY               SURFACE_NOT_SLIPPERY

#define SURFACE_CAMERA_FREE_ROAM                COLLISION_TYPE(0, 0, 0, COL_TYPE_CAMERA_FREE_ROAM, 0, 0, FALSE, FALSE, FALSE, FALSE) // (THI and TTC)
#define SURFACE_BOSS_FIGHT_CAMERA               COLLISION_TYPE(0, 0, 0, COL_TYPE_BOSS_FIGHT_CAMERA, 0, 0, FALSE, FALSE, FALSE, FALSE) // (BoB and WF bosses)
#define SURFACE_CLOSE_CAMERA                    COLLISION_TYPE(0, 0, 0, COL_TYPE_CLOSE_CAMERA, 0, 0, FALSE, FALSE, FALSE, FALSE)
#define SURFACE_CAMERA_8_DIR                    COLLISION_TYPE(0, 0, 0, COL_TYPE_CAMERA_8_DIR, 0, 0, FALSE, FALSE, FALSE, FALSE) // Enables far camera for platforms, used in THI
#define SURFACE_CAMERA_MIDDLE                   COLLISION_TYPE(0, 0, 0, COL_TYPE_CAMERA_MIDDLE, 0, 0, FALSE, FALSE, FALSE, FALSE) // Used on the 4 pillars of SSL
#define SURFACE_CAMERA_ROTATE_RIGHT             COLLISION_TYPE(0, 0, 0, COL_TYPE_CAMERA_ROTATE_RIGHT, 0, 0, FALSE, FALSE, FALSE, FALSE) // (Bowser 1 & THI)
#define SURFACE_CAMERA_ROTATE_LEFT              COLLISION_TYPE(0, 0, 0, COL_TYPE_CAMERA_ROTATE_LEFT, 0, 0, FALSE, FALSE, FALSE, FALSE) // (BoB & TTM)
#define SURFACE_CAMERA_BOUNDARY                 COLLISION_TYPE(0, COL_TYPE_INTANGIBLE, 0, COL_TYPE_CAMERA_BOUNDARY, 0, 0, FALSE, FALSE, FALSE, FALSE)
#define SURFACE_NO_CAM_COLLISION                COLLISION_TYPE(0, 0, 0, 0, 0, 0, FALSE, FALSE, FALSE, TRUE)
#define SURFACE_NO_CAM_COL_VERY_SLIPPERY        COLLISION_TYPE(0, 0, SURFACE_CLASS_VERY_SLIPPERY, 0, 0, 0, FALSE, FALSE, FALSE, TRUE)
#define SURFACE_NO_CAM_COL_SLIPPERY             COLLISION_TYPE(0, 0, SURFACE_CLASS_SLIPPERY, COL_TYPE_CLOSE_CAMERA, 0, 0, FALSE, FALSE, FALSE, TRUE) // (CCM, PSS and TTM slides)
#define SURFACE_SWITCH                          COLLISION_TYPE(0, 0, SURFACE_CLASS_NOT_SLIPPERY, 0, 0, SOUND_TERRAIN_STONE, FALSE, FALSE, FALSE, TRUE) // Used by switches and Dorrie

#define SURFACE_VANISH_CAP_WALLS                COLLISION_TYPE(0, 0, 0, 0, 0, 0, FALSE, TRUE, FALSE, FALSE) // Vanish cap walls, pass through them with Vanish Cap

// Warps to ID of force parameter's second byte if set, otherwise
// defaults to ID 0xF3.
#define SURFACE_WARP                            COLLISION_TYPE(COL_TYPE_WARP, 0, 0, 0, 0, 0, FALSE, FALSE, FALSE, FALSE)
// second byte if set, otherwise warps to 0xF3 if it exists, otherwise
// defaults to ID 0xF1.
#define SURFACE_LOOK_UP_WARP                    COLLISION_TYPE(COL_TYPE_LOOK_UP_WARP, 0, 0, 0, 0, 0, FALSE, FALSE, FALSE, FALSE)

// Instant warp to other areas
#define SURFACE_INSTANT_WARP_1B                 COLLISION_TYPE(COL_TYPE_INSTANT_WARP_0, 0, 0, 0, 0, 0, FALSE, FALSE, FALSE, FALSE) // (WDW and endless stairs)
#define SURFACE_INSTANT_WARP_1C                 COLLISION_TYPE(COL_TYPE_INSTANT_WARP_1, 0, 0, 0, 0, 0, FALSE, FALSE, FALSE, FALSE) // (WDW)
#define SURFACE_INSTANT_WARP_1D                 COLLISION_TYPE(COL_TYPE_INSTANT_WARP_2, 0, 0, 0, 0, 0, FALSE, FALSE, FALSE, FALSE) // (DDD, SSL and TTM)
#define SURFACE_INSTANT_WARP_1E                 COLLISION_TYPE(COL_TYPE_INSTANT_WARP_3, 0, 0, 0, 0, 0, FALSE, FALSE, FALSE, FALSE) // (DDD, SSL and TTM)

#define SURFACE_TTM_VINES                       SURFACE_DEFAULT
#define SURFACE_WATER                           SURFACE_DEFAULT
#define SURFACE_TRAPDOOR                        SURFACE_DEFAULT
#define SURFACE_HARD                            SURFACE_DEFAULT
#define SURFACE_THI3_WALLKICK                   SURFACE_DEFAULT
#define SURFACE_WOBBLING_WARP                   SURFACE_DEFAULT // Pool warp (HMC & DDD)

// Deprecated by painting object pr
#define SURFACE_PAINTING_WOBBLE_A6              SURFACE_DEFAULT // (BoB Left)
#define SURFACE_PAINTING_WOBBLE_A7              SURFACE_DEFAULT // (BoB Middle)
#define SURFACE_PAINTING_WOBBLE_A8              SURFACE_DEFAULT // (BoB Right)
#define SURFACE_PAINTING_WOBBLE_A9              SURFACE_DEFAULT // (CCM Left)
#define SURFACE_PAINTING_WOBBLE_AA              SURFACE_DEFAULT // (CCM Middle)
#define SURFACE_PAINTING_WOBBLE_AB              SURFACE_DEFAULT // (CCM Right)
#define SURFACE_PAINTING_WOBBLE_AC              SURFACE_DEFAULT // (WF Left)
#define SURFACE_PAINTING_WOBBLE_AD              SURFACE_DEFAULT // (WF Middle)
#define SURFACE_PAINTING_WOBBLE_AE              SURFACE_DEFAULT // (WF Right)
#define SURFACE_PAINTING_WOBBLE_AF              SURFACE_DEFAULT // (JRB Left)
#define SURFACE_PAINTING_WOBBLE_B0              SURFACE_DEFAULT // (JRB Middle)
#define SURFACE_PAINTING_WOBBLE_B1              SURFACE_DEFAULT // (JRB Right)
#define SURFACE_PAINTING_WOBBLE_B2              SURFACE_DEFAULT // (LLL Left)
#define SURFACE_PAINTING_WOBBLE_B3              SURFACE_DEFAULT // (LLL Middle)
#define SURFACE_PAINTING_WOBBLE_B4              SURFACE_DEFAULT // (LLL Right)
#define SURFACE_PAINTING_WOBBLE_B5              SURFACE_DEFAULT // (SSL Left)
#define SURFACE_PAINTING_WOBBLE_B6              SURFACE_DEFAULT // (SSL Middle)
#define SURFACE_PAINTING_WOBBLE_B7              SURFACE_DEFAULT // (SSL Right)
#define SURFACE_PAINTING_WOBBLE_BB              SURFACE_DEFAULT // (DDD - Left), makes the painting wobble if touched
#define SURFACE_PAINTING_WOBBLE_BE              SURFACE_DEFAULT // (WDW Left)
#define SURFACE_PAINTING_WOBBLE_BF              SURFACE_DEFAULT // (WDW Middle)
#define SURFACE_PAINTING_WOBBLE_C0              SURFACE_DEFAULT // (WDW Right)
#define SURFACE_PAINTING_WOBBLE_C1              SURFACE_DEFAULT // (THI Tiny - Left)
#define SURFACE_PAINTING_WOBBLE_C2              SURFACE_DEFAULT // (THI Tiny - Middle)
#define SURFACE_PAINTING_WOBBLE_C3              SURFACE_DEFAULT // (THI Tiny - Right)
#define SURFACE_PAINTING_WOBBLE_C4              SURFACE_DEFAULT // (TTM Left)
#define SURFACE_PAINTING_WOBBLE_C5              SURFACE_DEFAULT // (TTM Middle)
#define SURFACE_PAINTING_WOBBLE_C6              SURFACE_DEFAULT // (TTM Right)
#define SURFACE_PAINTING_WOBBLE_CD              SURFACE_DEFAULT // (THI Huge - Left)
#define SURFACE_PAINTING_WOBBLE_CE              SURFACE_DEFAULT // (THI Huge - Middle)
#define SURFACE_PAINTING_WOBBLE_CF              SURFACE_DEFAULT // (THI Huge - Right)
#define SURFACE_PAINTING_WOBBLE_D0              SURFACE_DEFAULT // (HMC & COTMC - Left), makes the painting wobble if
#define SURFACE_PAINTING_WOBBLE_D1              SURFACE_DEFAULT // Painting wobble (Unused, HMC & COTMC - Middle)
#define SURFACE_PAINTING_WOBBLE_D2              SURFACE_DEFAULT // Painting wobble (Unused, HMC & COTMC - Right)

#define SURFACE_PAINTING_WARP_D3                SURFACE_DEFAULT // Painting warp (BoB Left)
#define SURFACE_PAINTING_WARP_D4                SURFACE_DEFAULT // Painting warp (BoB Middle)
#define SURFACE_PAINTING_WARP_D5                SURFACE_DEFAULT // Painting warp (BoB Right)
#define SURFACE_PAINTING_WARP_D6                SURFACE_DEFAULT // Painting warp (CCM Left)
#define SURFACE_PAINTING_WARP_D7                SURFACE_DEFAULT // Painting warp (CCM Middle)
#define SURFACE_PAINTING_WARP_D8                SURFACE_DEFAULT // Painting warp (CCM Right)
#define SURFACE_PAINTING_WARP_D9                SURFACE_DEFAULT // Painting warp (WF Left)
#define SURFACE_PAINTING_WARP_DA                SURFACE_DEFAULT // Painting warp (WF Middle)
#define SURFACE_PAINTING_WARP_DB                SURFACE_DEFAULT // Painting warp (WF Right)
#define SURFACE_PAINTING_WARP_DC                SURFACE_DEFAULT // Painting warp (JRB Left)
#define SURFACE_PAINTING_WARP_DD                SURFACE_DEFAULT // Painting warp (JRB Middle)
#define SURFACE_PAINTING_WARP_DE                SURFACE_DEFAULT // Painting warp (JRB Right)
#define SURFACE_PAINTING_WARP_DF                SURFACE_DEFAULT // Painting warp (LLL Left)
#define SURFACE_PAINTING_WARP_E0                SURFACE_DEFAULT // Painting warp (LLL Middle)
#define SURFACE_PAINTING_WARP_E1                SURFACE_DEFAULT // Painting warp (LLL Right)
#define SURFACE_PAINTING_WARP_E2                SURFACE_DEFAULT // Painting warp (SSL Left)
#define SURFACE_PAINTING_WARP_E3                SURFACE_DEFAULT // Painting warp (SSL Medium)
#define SURFACE_PAINTING_WARP_E4                SURFACE_DEFAULT // Painting warp (SSL Right)
#define SURFACE_PAINTING_WARP_E5                SURFACE_DEFAULT // Painting warp (Unused - Left)
#define SURFACE_PAINTING_WARP_E6                SURFACE_DEFAULT // Painting warp (Unused - Medium)
#define SURFACE_PAINTING_WARP_E7                SURFACE_DEFAULT // Painting warp (Unused - Right)
#define SURFACE_PAINTING_WARP_E8                SURFACE_DEFAULT // Painting warp (DDD - Left)
#define SURFACE_PAINTING_WARP_E9                SURFACE_DEFAULT // Painting warp (DDD - Middle)
#define SURFACE_PAINTING_WARP_EA                SURFACE_DEFAULT // Painting warp (DDD - Right)
#define SURFACE_PAINTING_WARP_EB                SURFACE_DEFAULT // Painting warp (WDW Left)
#define SURFACE_PAINTING_WARP_EC                SURFACE_DEFAULT // Painting warp (WDW Middle)
#define SURFACE_PAINTING_WARP_ED                SURFACE_DEFAULT // Painting warp (WDW Right)
#define SURFACE_PAINTING_WARP_EE                SURFACE_DEFAULT // Painting warp (THI Tiny - Left)
#define SURFACE_PAINTING_WARP_EF                SURFACE_DEFAULT // Painting warp (THI Tiny - Middle)
#define SURFACE_PAINTING_WARP_F0                SURFACE_DEFAULT // Painting warp (THI Tiny - Right)
#define SURFACE_PAINTING_WARP_F1                SURFACE_DEFAULT // Painting warp (TTM Left)
#define SURFACE_PAINTING_WARP_F2                SURFACE_DEFAULT // Painting warp (TTM Middle)
#define SURFACE_PAINTING_WARP_F3                SURFACE_DEFAULT // Painting warp (TTM Right)
#define SURFACE_TTC_PAINTING_1                  SURFACE_DEFAULT // Painting warp (TTC Left)
#define SURFACE_TTC_PAINTING_2                  SURFACE_DEFAULT // Painting warp (TTC Medium)
#define SURFACE_TTC_PAINTING_3                  SURFACE_DEFAULT // Painting warp (TTC Right)
#define SURFACE_PAINTING_WARP_F7                SURFACE_DEFAULT // Painting warp (SL Left)
#define SURFACE_PAINTING_WARP_F8                SURFACE_DEFAULT // Painting warp (SL Middle)
#define SURFACE_PAINTING_WARP_F9                SURFACE_DEFAULT // Painting warp (SL Right)
#define SURFACE_PAINTING_WARP_FA                SURFACE_DEFAULT // Painting warp (THI Huge - Left)
#define SURFACE_PAINTING_WARP_FB                SURFACE_DEFAULT // Painting warp (THI Huge - Middle)
#define SURFACE_PAINTING_WARP_FC                SURFACE_DEFAULT // Painting warp (THI Huge - Right)

#define SURFACE_IS_NEW_WATER(type) \
    (type.special == COL_TYPE_WATER || type.special == COL_TYPE_WATER_BOTTOM)

#define SURFACE_IS_QUICKSAND(type) \
    (type.special >= COL_TYPE_QUICKSAND && type.special <= COL_TYPE_INSTANT_MOVING_QUICKSAND)

#define SURFACE_IS_INSTANT_WARP(type) \
    (type.warpsAndLevel >= COL_TYPE_INSTANT_WARP_0 && type.warpsAndLevel < (COL_TYPE_INSTANT_WARP_0 + INSTANT_WARP_INDEX_STOP))

#define SURFACE_IS_WARP(type) \
    (type.warpsAndLevel == COL_TYPE_LOOK_UP_WARP || SURFACE_IS_INSTANT_WARP(type))

#define SURFACE_IS_UNSAFE(type) \
    (type.special == COL_TYPE_BURNING || SURFACE_IS_QUICKSAND(type) || SURFACE_IS_WARP(type))

enum SurfaceFlags {
    SURFACE_FLAGS_NONE = (0 << 0),            // 0x0000
    SURFACE_FLAG_DYNAMIC = (1 << 0),          // 0x0001
    SURFACE_FLAG_NO_CAM_COLLISION = (1 << 1), // 0x0002
};

// These are effectively unique "surface" types like those defined higher
// And they are used as collision commands to load certain functions
enum TerrainLoadCmd {
    TERRAIN_LOAD_VERTICES = 40, // Begins vertices list for collision triangles
    TERRAIN_LOAD_CONTINUE,   // Stop loading vertices but continues to load other collision commands
    TERRAIN_LOAD_END,        // End the collision list
    TERRAIN_LOAD_OBJECTS,    // Loads in certain objects for level start
    TERRAIN_LOAD_ENVIRONMENT, // Loads water/HMC gas
    TERRAIN_LOAD_TRIS,
};

// Terrain types defined by the level script command terrain_type (cmd_31)
enum TerrainType {
    TERRAIN_GRASS,
    TERRAIN_STONE,
    TERRAIN_SNOW,
    TERRAIN_SAND,
    TERRAIN_SPOOKY,
    TERRAIN_WATER,
    TERRAIN_SLIDE,
    TERRAIN_MASK = 0x7
};

// These collision commands are unique "surface" types like those defined higher

// Collision Data Routine Initiate
#define COL_INIT() TERRAIN_LOAD_VERTICES

// Collision Vertices Read Initiate
#define COL_VERTEX_INIT(vtxNum) vtxNum

// Collision Vertex
#define COL_VERTEX(x, y, z) x, y, z

// Hacky solution to store two s16´s for the surface type.
#define COL_TYPE_TO_2_S16(surfType) ((surfType >> 16) & 0xFFFF), (surfType & 0xFFFF)

// Collision Tris Initiate
#define COL_TRI_INIT(surfType, triNum) TERRAIN_LOAD_TRIS, COL_TYPE_TO_2_S16(surfType), triNum

// Collision Tri
#ifdef ALL_SURFACES_HAVE_FORCE
#define COL_TRI(v1, v2, v3) v1, v2, v3, 0
#else
#define COL_TRI(v1, v2, v3) v1, v2, v3
#endif

// Collision Tri With Special Params
#define COL_TRI_SPECIAL(v1, v2, v3, param) v1, v2, v3, param

// Collision Tris Stop Loading
#define COL_TRI_STOP() TERRAIN_LOAD_CONTINUE

// End Collision Data
#define COL_END() TERRAIN_LOAD_END

// Special Object Initiate
#define COL_SPECIAL_INIT(num) TERRAIN_LOAD_OBJECTS, num

// Water Boxes Initiate
#define COL_WATER_BOX_INIT(num) TERRAIN_LOAD_ENVIRONMENT, num

// Water Box
#define COL_WATER_BOX(id, x1, z1, x2, z2, y) id, x1, z1, x2, z2, y

#endif // SURFACE_TERRAINS_H
