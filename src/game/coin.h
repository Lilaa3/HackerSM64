#ifndef COIN_H
#define COIN_H
#include <ultra64.h>
#include "sm64.h"
#include "rendering_graph_node.h"
#include "mario_misc.h"
#include "skybox.h"
#include "engine/math_util.h"
#include "camera.h"
#include "envfx_snow.h"
#include "level_geo.h"
#include "n64/PR/gbi.h"
#include "game/area.h"
#include "config/config_graphics.h"

#define COIN_RADIUS (0.64f * 100)
#define COIN_SHADOW_RADIUS (COIN_RADIUS / 3)
#define COIN_CULLING_RADIUS COIN_RADIUS
#define COIN_RENDER_DISTANCE (COIN_RADIUS * 1000)

struct CoinState{
    u8 render:1;
    s8 room:7;
};

struct CoinInfo{
    u8 type;
    Vec3s pos;
};

#define COIN_ARRAY_SIZE 200
struct CoinAreaData
{
    ALIGNED16 struct CoinState coinStates[COIN_ARRAY_SIZE];
    ALIGNED16 struct CoinInfo coins[COIN_ARRAY_SIZE];
    u32 head;
};

enum CoinSpawnFlags{
    COIN_FLAG_SNAP_TO_GROUND = 1 << 0,
};

enum CoinType{
    COIN_BHV_TYPE_YELLOW,
    COIN_BHV_TYPE_BLUE,
    COIN_BHV_TYPE_RED,
};

enum BlueCoinSwitchState{
    BLUE_COIN_SWITCH_IDLE = 0x00,
    BLUE_COIN_SWITCH_ACTIVE = 0x01,
    BLUE_COIN_SWITCH_BLINKING = 0x02,
};

extern u32 coinAmounts[];

extern u32 blueCoinSwitchActiveState;

void render_coins();
void lvl_process_coins(struct CoinAreaData* coinData);
void lvl_process_coin(struct CoinAreaData* coinData, u32 index);
void lvl_clean_coins();

s32 coin_render_distance_cull(struct CoinInfo* coin);
s32 coin_room_cull(struct CoinState* coinState);
s32 coin_frustum_cull(struct CoinInfo* coin);
s32 coin_hidden_blue_logic(struct CoinState* coinState);
s32 coin_hitbox_intersects_with_mario(struct CoinInfo* coin);

//Utility functions
void collect_coin(struct CoinAreaData* coinData, s32 i);

//Coin spawning functions
struct CoinInfo *spawn_coin(Vec3s pos, u32 type, u32 flags);

struct CoinInfo *spawn_coin_relative(Vec3s pos, Vec3s offset, u8 type, u16 flags);
struct CoinInfo *spawn_coin_relative_specific_o(struct Object *obj, Vec3f offset, u8 type, u16 flags);
struct CoinInfo *spawn_coin_specific_o(struct Object *obj, u8 type, u16 flags);

#define spawn_coin_relative_o(offset, type, flags) \
    spawn_coin_relative_specific_o(gCurrentObject, offset, type, flags)

#define spawn_coin_at_o(type, flags) \
    spawn_coin_specific_o(gCurrentObject, type, flags)

//Search functions
s32 hidden_blue_coins_left();
s32 redCoinsLeft();

#endif
