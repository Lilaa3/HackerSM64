#include "object_fields.h"
#include "object_constants.h"
#include "object_helpers.h"
#include "object_collision.h"
#include "obj_behaviors.h"
#include "object_list_processor.h"
#include "interaction.h"
#include "behavior_data.h"
#include "engine/behavior_script.h"
#include "engine/math_util.h"
#include "engine/surface_load.h"
#include "engine/surface_collision.h"
#include "sound_init.h"
#include "spawn_sound.h"
#include "farcall.h"
#include "audio/external.h"
#include "level_update.h"
#include "mario.h"
#include "save_file.h"
#include "game_init.h"
#include "obj_behaviors_2.h"
#include "level_misc_macros.h"
#include "emutest.h"
#include "segment2.h"
#include <string.h>
#include "coin.h"

Gfx coin_shadow_mesh[] = {
	gsSPVertex(vertex_shadow, 4, 0),
    gsSP2Triangles( 0,  2,  1, 0x0,  1,  2,  3, 0x0),
	gsSPEndDisplayList(),
};

Gfx mat_revert_coin[] = {
	gsSPTexture(0x0001, 0x0001, 0, G_TX_RENDERTILE, G_OFF),
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsSPSetGeometryMode(G_LIGHTING),
    gsSPEndDisplayList(),
};

Gfx mat_revert_shadow[] = {
	gsDPPipeSync(),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
};

// Loading a texture twice is CRINGE, please fix.
extern const Texture coin_seg3_texture_0_ia8[];
extern const Texture coin_seg3_texture_22_5_ia8[];
extern const Texture coin_seg3_texture_45_ia8[];
extern const Texture coin_seg3_texture_67_5_ia8[];
extern const Texture coin_seg3_texture_90_ia8[];

const Texture *coin_frames[] = {
    &coin_seg3_texture_0_ia8,
    &coin_seg3_texture_22_5_ia8,
    &coin_seg3_texture_45_ia8,
    &coin_seg3_texture_67_5_ia8,
    &coin_seg3_texture_90_ia8,

    &coin_seg3_texture_67_5_ia8,
    &coin_seg3_texture_45_ia8,
    &coin_seg3_texture_22_5_ia8,
};

const Gfx coin_dl_start[] = {
    gsDPPipeSync(),
    gsDPLoadSync(),
    gsSPTexture(32767, 32767, 0, G_TX_RENDERTILE, G_ON),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_IA, G_IM_SIZ_8b, 8, 0, G_TX_LOADTILE, 0, (G_TX_CLAMP | G_TX_NOMIRROR), 6, G_TX_NOLOD, (G_TX_CLAMP | G_TX_NOMIRROR), 6, G_TX_NOLOD),
    gsDPLoadSync(),
    gsDPLoadTile(G_TX_LOADTILE, 0, 0, 252, 252),
    gsDPPipeSync(),
    gsDPSetTile(G_IM_FMT_IA, G_IM_SIZ_8b, 8, 0, G_TX_RENDERTILE, 0, (G_TX_CLAMP | G_TX_NOMIRROR), 6, G_TX_NOLOD, (G_TX_CLAMP | G_TX_NOMIRROR), 6, G_TX_NOLOD),
    gsDPSetTileSize(0, 0, 0, 252, 252),

    gsSPClearGeometryMode(G_LIGHTING),
    gsSPSetGeometryMode(G_ZBUFFER),
    gsDPSetCombineMode(G_CC_MODULATEIA, G_CC_MODULATEIA),
    gsDPSetRenderMode(G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2),
    gsSPEndDisplayList(),
};

static Vtx coin_vertices[] = {
    // Yellow COIN_BHV_TYPE_YELLOW
    {{{   -32,      0,      0}, 0, {  4080,   4080}, {0xff, 0xff, 0x00, 0xff}}},
    {{{    32,      0,      0}, 0, {   -16,   4080}, {0xff, 0xff, 0x00, 0xff}}},
    {{{    32,     64,      0}, 0, {   -16,    -16}, {0xff, 0xff, 0x00, 0xff}}},
    {{{   -32,     64,      0}, 0, {  4080,    -16}, {0xff, 0xff, 0x00, 0xff}}},
    // Blue COIN_BHV_TYPE_BLUE
    {{{   -48,      0,      0}, 0, {  4080,   4080}, {0x80, 0x80, 0xff, 0xff}}},
    {{{    48,      0,      0}, 0, {   -16,   4080}, {0x80, 0x80, 0xff, 0xff}}},
    {{{    48,     96,      0}, 0, {   -16,    -16}, {0x80, 0x80, 0xff, 0xff}}},
    {{{   -48,     96,      0}, 0, {  4080,    -16}, {0x80, 0x80, 0xff, 0xff}}},
    // Red COIN_BHV_TYPE_RED
    {{{   -36,      0,      0}, 0, {  4080,   4080}, {0xff, 0x00, 0x00, 0xff}}},
    {{{    36,      0,      0}, 0, {   -16,   4080}, {0xff, 0x00, 0x00, 0xff}}},
    {{{    36,     72,      0}, 0, {   -16,    -16}, {0xff, 0x00, 0x00, 0xff}}},
    {{{   -36,     72,      0}, 0, {  4080,    -16}, {0xff, 0x00, 0x00, 0xff}}},

    // Flipped
    // Yellow COIN_BHV_TYPE_YELLOW
    {{{   -32,      0,      0}, 0, {   -16,   4080}, {0xff, 0xff, 0x00, 0xff}}},
    {{{    32,      0,      0}, 0, {  4080,   4080}, {0xff, 0xff, 0x00, 0xff}}},
    {{{    32,     64,      0}, 0, {  4080,    -16}, {0xff, 0xff, 0x00, 0xff}}},
    {{{   -32,     64,      0}, 0, {   -16,    -16}, {0xff, 0xff, 0x00, 0xff}}},
    // Blue COIN_BHV_TYPE_BLUE
    {{{   -48,      0,      0}, 0, {   -16,   4080}, {0x80, 0x80, 0xff, 0xff}}},
    {{{    48,      0,      0}, 0, {  4080,   4080}, {0x80, 0x80, 0xff, 0xff}}},
    {{{    48,     96,      0}, 0, {  4080,    -16}, {0x80, 0x80, 0xff, 0xff}}},
    {{{   -48,     96,      0}, 0, {   -16,    -16}, {0x80, 0x80, 0xff, 0xff}}},
    // Red COIN_BHV_TYPE_RED
    {{{   -36,      0,      0}, 0, {   -16,   4080}, {0xff, 0x00, 0x00, 0xff}}},
    {{{    36,      0,      0}, 0, {  4080,   4080}, {0xff, 0x00, 0x00, 0xff}}},
    {{{    36,     72,      0}, 0, {  4080,    -16}, {0xff, 0x00, 0x00, 0xff}}},
    {{{   -36,     72,      0}, 0, {   -16,    -16}, {0xff, 0x00, 0x00, 0xff}}},
};

extern s16 gMatStackIndex;
extern ALIGNED16 Mat4 gMatStack[32];
extern ALIGNED16 Mtx *gMatStackFixed[32];

u32 gBlueCoinSwitchState = BLUE_COIN_SWITCH_IDLE;

struct CoinInfo *spawn_coin(Vec3s pos, u32 type, u32 flags) {
    struct Area* area = gCurrentArea;
    struct CoinAreaData* coinData = &(gCurrentArea->coinData);

    struct CoinInfo *coin = &coinData->coins[coinData->head];
    struct CoinState *coinState = &coinData->coinStates[coinData->head];

    coin->pos[0] = pos[0];
    coin->pos[2] = pos[2];

    struct Surface *floor = NULL;

    if(flags & COIN_FLAG_SNAP_TO_GROUND){
        coin->pos[1] = (s16) (find_floor(coin->pos[0], pos[1] + 40, coin->pos[2], &floor));
    }
    else{
        coin->pos[1] = pos[1];
    }
    
    coin->type = type;

    coinState->render = TRUE;
    coinState->room = get_room_at_pos(coin->pos[0], coin->pos[1], coin->pos[2]);

    coinData->head += 1;
    return coin;
}

ALWAYS_INLINE struct CoinInfo *spawn_coin_relative(Vec3s pos, Vec3s offset, u32 type, u32 flags) {
    Vec3s relativePos;
    vec3s_sum(relativePos, pos, offset);
	return spawn_coin(relativePos, type, flags);
}

ALWAYS_INLINE struct CoinInfo *spawn_coin_relative_specific_o(struct Object *obj, Vec3f offset, u32 type, u32 flags) {
    Vec3s pos;
    s16 yaw = obj->oFaceAngleYaw;

    pos[0] = (s16) obj->oPosX + (offset[0] * coss(yaw) + offset[2] * sins(yaw));
    pos[1] = (s16) obj->oPosY + offset[1];
    pos[2] = (s16) obj->oPosZ + (offset[2] * coss(yaw) - offset[0] * sins(yaw));
	return spawn_coin(pos, type, flags);
}

ALWAYS_INLINE struct CoinInfo *spawn_coin_specific_o(struct Object *obj, u32 type, u32 flags) {
    Vec3s pos;
    vec3f_to_vec3s(pos, &obj->oPosVec);
	return spawn_coin(pos, type, flags);
}

void lvl_clean_coins(){
    for(u32 i = 0; i < AREA_COUNT; i++){  
        struct Area* area = &gAreaData[i];
        struct CoinAreaData* coinData = &(area->coinData);
        coinData->head = 0;
    }
    gBlueCoinSwitchState = BLUE_COIN_SWITCH_IDLE;
}

s32 hidden_blue_coins_left(){
    s32 blueCoinsFound = 0;

    struct CoinAreaData* coinData = &(gCurrentArea->coinData);

    for(u32 i = 0; i < coinData->head; i++){
        if (coinData->coins[i].type == COIN_BHV_TYPE_BLUE)
            blueCoinsFound += 0x01;
    }

    return blueCoinsFound;
}

u32 coinAmounts[3] = {1, 5, 5};

ALWAYS_INLINE void delete_coin(struct CoinAreaData* coinData, s32 i){
    // Basically, put the coin furthest in the buffer into the no longer
    // needed coin slots.

    coinData->coins[i] = coinData->coins[coinData->head - 1];
    coinData->coinStates[i] = coinData->coinStates[coinData->head - 1];
    
    if (coinData->head)
        coinData->head--;
}

extern struct Object gMacroObjectDefaultParent;

void spawn_orange_number_at_coin(struct CoinInfo* coin, u8 num, f32 x){
    struct Object *orangeNumber = spawn_object_at_origin(&gMacroObjectDefaultParent, 0, MODEL_NUMBER, bhvOrangeNumber);
    obj_set_pos(orangeNumber, coin->pos[0] + x, coin->pos[1] + 25.f, coin->pos[2]);
    orangeNumber->oOrangeNumberOffset = x;
    orangeNumber->oHomeX = coin->pos[0];
    orangeNumber->oHomeZ = coin->pos[2];
    orangeNumber->oBehParams2ndByte = num;
}

void collect_red_coin(struct CoinInfo* coin){
    struct Object *hiddenRedCoinStar = cur_obj_nearest_object_with_behavior(bhvHiddenRedCoinStar);
    if (hiddenRedCoinStar == NULL)
        return;

    // ...increment the star's counter.
    hiddenRedCoinStar->oHiddenStarTriggerCounter++;
    u32 counter = hiddenRedCoinStar->oHiddenStarTriggerCounter;

    // Spawn the orange number counter, as long as it isn't the last coin.
    if (counter != hiddenRedCoinStar->oHiddenStarTriggerTotal) {
        // Cap visible count to 99
        if (counter > 99) {
            spawn_orange_number_at_coin(coin, 9, 28);
            spawn_orange_number_at_coin(coin, 9, -28);
            
        }
        else if (counter >= 10) {
            spawn_orange_number_at_coin(coin, counter % 10, 28);
            spawn_orange_number_at_coin(coin, counter / 10, -28);
        }
        else {
            spawn_orange_number_at_coin(coin, counter, 0);
        }
    }

#ifdef JP_RED_COIN_SOUND
    // For JP version, play an identical sound for all coins.
    create_sound_spawner(SOUND_GENERAL_RED_COIN);
#else
    if (hiddenRedCoinStar->oHiddenStarTriggerTotal - hiddenRedCoinStar->oHiddenStarTriggerCounter > 7) {
        // Play the first red coin sound until it gets to the final 8
        play_sound(SOUND_MENU_COLLECT_RED_COIN, gGlobalSoundSource);
    }
    else {
        // On all versions but the JP version, each coin collected plays a higher noise.
        play_sound(SOUND_MENU_COLLECT_RED_COIN
                + (((u8) 7 - (hiddenRedCoinStar->oHiddenStarTriggerTotal - counter)) << 16),
                gGlobalSoundSource);
    }
#endif
}

void collect_coin(struct CoinAreaData* coinData, s32 i){
    struct CoinInfo* coin = &(coinData->coins[i]);

    interact_coin_objectless(gMarioState, coinAmounts[coin->type]);

    if(coin->type == COIN_BHV_TYPE_RED){
        collect_red_coin(coin);
    }

    struct Object *sparkles = spawn_object_at_origin(&gMacroObjectDefaultParent, 0, MODEL_SPARKLES, bhvCoinSparklesSpawner);
    obj_set_pos(sparkles, coin->pos[0], coin->pos[1], coin->pos[2]);

    delete_coin(coinData, i);
}

// Culls the coin if the console check returns positive and is far away. Returns false if the coin should be skipped.
ALWAYS_INLINE s32 coin_render_distance_cull(struct CoinInfo* coin){
    if (!(gEmulator & NO_CULLING_EMULATOR_BLACKLIST)){
        return TRUE;
    }
    f32 cameraX = gPlayerCameraState->pos[0];
    f32 cameraY = gPlayerCameraState->pos[1];
    f32 cameraZ = gPlayerCameraState->pos[2];
    f32 x = coin->pos[0];
    f32 y = coin->pos[1];
    f32 z = coin->pos[2];
    f32 distance = sqr(x - cameraX) + sqr(y - cameraY)+ sqr(z - cameraZ);
    return (distance < sqr(COIN_RENDER_DISTANCE)); // Compiler will handle the sqr, no need for another define
}

// Culls the coin depending on its room. Returns false if the coin should be skipped.
ALWAYS_INLINE s32 coin_room_cull(struct CoinState* coinState){
    s32 room = coinState->room;
    if (room < 0 || gMarioCurrentRoom == room)
        return TRUE;

    s32 gDoorAdjecentRoom0 = gDoorAdjacentRooms[gMarioCurrentRoom].forwardRoom;
    s32 gDoorAdjecentRoom1 = gDoorAdjacentRooms[gMarioCurrentRoom].backwardRoom;
    return (gDoorAdjecentRoom0 == room || gDoorAdjecentRoom1 == room);
}

#define CULLING_ON_EMULATOR // Active for testing

extern struct CameraFOVStatus sFOVState;
extern f32 sAspectRatio;

// Removed behavior
ALWAYS_INLINE s32 coin_frustum_cull(struct CoinInfo* coin){
    return TRUE;
}

// Checks for the blue coin switch state and deactivates the render if needed.
// Returns false if the coin should be skipped.
ALWAYS_INLINE s32 coin_hidden_blue_logic(struct CoinState* coinState){
    if (gBlueCoinSwitchState & BLUE_COIN_SWITCH_ACTIVE){
        coinState->render = !(gBlueCoinSwitchState & BLUE_COIN_SWITCH_BLINKING);
        return TRUE;
    }
    else{
        coinState->render = FALSE;
        return FALSE;
    }
}

// Checks for the blue coin switch state and deactivates the render if needed. Returns false if the coin should be skipped.
ALWAYS_INLINE s32 coin_hitbox_intersects_with_mario(struct CoinInfo* coin){
    f32 dya_bottom = coin->pos[1] - 128;

    f32 marioX = gMarioObject->oPosX;
    f32 marioY = gMarioObject->oPosY;
    f32 marioZ = gMarioObject->oPosZ;

    f32 dyb_bottom = marioY - gMarioObject->hitboxDownOffset;
    f32 dx = coin->pos[0] - marioX;
    f32 dz = coin->pos[2] - marioZ;
    f32 collisionRadius = 100 + gMarioObject->hurtboxRadius;
    f32 lateraldistance = sqr(dx) + sqr(dz);

    if (sqr(collisionRadius) > lateraldistance) {
        f32 dya_top = 128 + 64 + dya_bottom;
        f32 dyb_top = gMarioObject->hurtboxHeight + dyb_bottom;

        if (!(dya_bottom > dyb_top || dya_top < dyb_bottom)) {
            return TRUE;
        }
    }

    return FALSE;
}

s32 gRenderableCoins;

void lvl_process_coin(struct CoinAreaData* coinData, u32 index){
    gCurrentObject = &gMacroObjectDefaultParent;

    struct CoinInfo *coin = &coinData->coins[index];
    struct CoinState *coinState = &coinData->coinStates[index];

    if (!coin_room_cull(coinState) || !coin_render_distance_cull(coin)){
        coinState->render = FALSE;
        return;
    }

    coinState->render = coin_frustum_cull(coin);
    if (coin->type == COIN_BHV_TYPE_BLUE && !coin_hidden_blue_logic(coinState)){
        return;
    }

    if (coin_hitbox_intersects_with_mario(coin)){
        collect_coin(coinData, index);
        coinState->render = FALSE;
    }
}

void lvl_process_coins(struct CoinAreaData* coinData){ 
    gRenderableCoins = 0;

    for(register u32 i = 0; i < coinData->head; i++){
        lvl_process_coin(coinData, i);
        if (coinData->coinStates[i].render)
            gRenderableCoins += 1;
    }

	print_text_fmt_int(150, 20, "Coin Count %d", coinData->head);
}

OSTime lastCoinRender = 0x00;

#define COIN_FPS 15
#define FRAME_FROM_TIMER(fps) (gGlobalTimer / (30 / fps))
#define GET_FRAME_NUMBER(frames, fps) FRAME_FROM_TIMER(fps) % ARRAY_COUNT(frames)
#define GET_TEXTURE_FRAME(frames, fps) frames[GET_FRAME_NUMBER(frames, fps)]

void render_coins() {
    struct Area* area = gCurrentArea;
    struct CoinAreaData* coinData = &(area->coinData);

    print_text_fmt_int(150, 45, "Renderable %d", gRenderableCoins);

    //Allocate display list for the material, coins and their respective propreties.
    Gfx *coinDL = alloc_display_list(((sizeof(Gfx) * 3) * gRenderableCoins) + (sizeof(Gfx) * 4));
    Gfx *coinDLHead = coinDL;
    Gfx *shadowDL = alloc_display_list((sizeof(Gfx) * 3) * gRenderableCoins + (sizeof(Gfx) * 5));
    Gfx *shadowDLHead = shadowDL;

    Vec3f floorNormal = {0, 0, 0};
    Vec3f shadowSize = {COIN_SHADOW_RADIUS, COIN_SHADOW_RADIUS, COIN_SHADOW_RADIUS};

    //Set materials
    gDPSetTextureImage(coinDLHead++, G_IM_FMT_IA, G_IM_SIZ_8b, 64, GET_TEXTURE_FRAME(coin_frames, COIN_FPS))
    gSPDisplayList(coinDLHead++, coin_dl_start);
    gSPDisplayList(shadowDLHead++, dl_shadow_circle);
    

    Vtx *vertices = coin_vertices;
    if (GET_FRAME_NUMBER(coin_frames, COIN_FPS) >= 5){
        print_text(30, 30, "Flipped?");
        vertices += ARRAY_COUNT(coin_vertices) / 2;
    }

    gSPSetGeometryMode(shadowDLHead++, G_ZBUFFER);
    gDPSetRenderMode(shadowDLHead++, G_RM_AA_ZB_XLU_DECAL, G_RM_AA_ZB_XLU_DECAL2);

    Mat4 coinMat;
    for (u32 i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            coinMat[i][j] = gCameraTransform[j][i];
        }
        coinMat[i][3] = 0.0f;
    }
    Mat4 shadowMat;

    for(u32 coinIndex = 0; coinIndex < coinData->head; coinIndex++){
        if(!coinData->coinStates[coinIndex].render) // Sweet sweet cache hits >:D
            continue;
        struct CoinInfo coin = coinData->coins[coinIndex];

        f32 *pos = coinMat[3];
        vec3s_to_vec3f(pos, coin.pos);
        Mtx *mtx = alloc_display_list(sizeof(*mtx)); mtxf_to_mtx(mtx, coinMat); // To fixed point
        gSPMatrix(coinDLHead++, mtx, G_MTX_NOPUSH | G_MTX_LOAD);

        gSPVertex(coinDLHead++, vertices + (coin.type * 4), 4, 0);
        gSP2Triangles(coinDLHead++, 0,  1,  2, 0x0,  0,  2,  3, 0x0);
 
        // Shadow logic
        struct Surface *floor = NULL;
        pos[1] = find_floor(coin.pos[0], coin.pos[1], coin.pos[2], &floor);
        if(floor != NULL){
            f32 distanceToGround = (coin.pos[1] -  pos[1]);

            gDPSetEnvColor(shadowDLHead++, 0, 0, 0, 150 - CLAMP(distanceToGround, 0, 150));

            surface_normal_to_vec3f(floorNormal, floor);
            mtxf_shadow(shadowMat, floorNormal, pos, shadowSize, 0);
            Mtx *shadowMtx = alloc_display_list(sizeof(*shadowMtx));
            mtxf_to_mtx(shadowMtx, shadowMat);

            gSPMatrix(shadowDLHead++, shadowMtx, G_MTX_MODELVIEW | G_MTX_NOPUSH | G_MTX_LOAD);
            gSPDisplayList(shadowDLHead++, coin_shadow_mesh);
        }
    }

    gSPDisplayList(shadowDLHead++, mat_revert_shadow);
    gSPEndDisplayList(shadowDLHead);
    geo_append_display_list(shadowDL, LAYER_TRANSPARENT_DECAL); 

    gSPDisplayList(coinDLHead++, mat_revert_coin);
    gSPEndDisplayList(coinDLHead);
    geo_append_display_list(coinDL, LAYER_ALPHA); 
}
