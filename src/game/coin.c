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

static const Vtx coin_vertices[] = {
    // Yellow
    {{{   -32,      0,      0}, 0, {   -16,   4080}, {0xff, 0xff, 0x00, 0xff}}},
    {{{    32,      0,      0}, 0, {  4080,   4080}, {0xff, 0xff, 0x00, 0xff}}},
    {{{    32,     64,      0}, 0, {  4080,    -16}, {0xff, 0xff, 0x00, 0xff}}},
    {{{   -32,     64,      0}, 0, {   -16,    -16}, {0xff, 0xff, 0x00, 0xff}}},
    // Blue
    {{{   -48,      0,      0}, 0, {   -16,   4080}, {0x80, 0x80, 0xff, 0xff}}},
    {{{    48,      0,      0}, 0, {  4080,   4080}, {0x80, 0x80, 0xff, 0xff}}},
    {{{    48,     96,      0}, 0, {  4080,    -16}, {0x80, 0x80, 0xff, 0xff}}},
    {{{   -48,     96,      0}, 0, {   -16,    -16}, {0x80, 0x80, 0xff, 0xff}}},
    // Red
    {{{   -36,      0,      0}, 0, {   -16,   4080}, {0xff, 0x00, 0x00, 0xff}}},
    {{{    36,      0,      0}, 0, {  4080,   4080}, {0xff, 0x00, 0x00, 0xff}}},
    {{{    36,     72,      0}, 0, {  4080,    -16}, {0xff, 0x00, 0x00, 0xff}}},
    {{{   -36,     72,      0}, 0, {   -16,    -16}, {0xff, 0x00, 0x00, 0xff}}},
};

extern s16 gMatStackIndex;
extern ALIGNED16 Mat4 gMatStack[32];
extern ALIGNED16 Mtx *gMatStackFixed[32];

u32 blueCoinSwitchActiveState = BLUE_COIN_SWITCH_IDLE;

struct CoinInfo *spawn_coin(Vec3s pos, u32 type, u32 flags) {
    struct Area* area = gCurrentArea;
    struct CoinAreaData* coinData = &(gCurrentArea->coinData);

    struct CoinInfo *coin = &coinData->coins[coinData->head];
    struct CoinState *coinState = &coinData->coinStates[coinData->head];
    coinData->head += 1;

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

    return coin;
}

ALWAYS_INLINE struct CoinInfo *spawn_coin_relative(Vec3s pos, Vec3s offset, u8 type, u16 flags) {
    Vec3s relativePos;
    vec3s_sum(relativePos, pos, offset);
	return spawn_coin(relativePos, type, flags);
}

ALWAYS_INLINE struct CoinInfo *spawn_coin_relative_specific_o(struct Object *obj, Vec3s offset, u8 type, u16 flags) {
    Vec3s pos;
    pos[0] = obj->oPosX + offset[0] * coss(obj->oFaceAngleYaw) - offset[2] * sins(obj->oFaceAngleYaw);
    pos[2] = obj->oPosZ + offset[0] * sins(obj->oFaceAngleYaw) + offset[2] * coss(obj->oFaceAngleYaw);
    pos[1] = obj->oPosY + offset[1];

	return spawn_coin(pos, type, flags);
}

ALWAYS_INLINE struct CoinInfo *spawn_coin_specific_o(struct Object *obj, u8 type, u16 flags) {
    Vec3s pos;
    vec3f_to_vec3s(pos, &obj->oPosVec);
	return spawn_coin(pos, type, flags);
}

void lvl_clean_coins(){
    for(int i = 0; i < AREA_COUNT; i++){  
        struct Area* area = &gAreaData[i];
        struct CoinAreaData* coinData = &(area->coinData);

        for(int j = 0; j < COIN_ARRAY_SIZE; j++){
            struct CoinInfo *coin = &coinData->coins[j];
            struct CoinState *coinState = &coinData->coinStates[j];

            vec3_zero(coin->pos);
            coin->type = COIN_BHV_TYPE_YELLOW;
            coinState->render = FALSE;
            coinState->room = -1;
        }

        coinData->head = 0;
    }
    blueCoinSwitchActiveState = BLUE_COIN_SWITCH_IDLE;
}

s32 hidden_blue_coins_left(){
    s32 blueCoinsFound = 0;

    struct Area* area = gCurrentArea;
    struct CoinInfo* coinData = &(area->coinData);

    //for(int i = 0; i < coinData->coinHead; i++){
    //    if(is_coin_flag_true(i, COIN_FLAG_HIDDEN_BLUE, coinData)){
    //        blueCoinsFound += 0x01;
    //    }
    //}

    return blueCoinsFound;
}

u32 coinAmounts[3] = {1, 5, 5};

extern struct Object gMacroObjectDefaultParent;
ALWAYS_INLINE void collect_coin(struct CoinAreaData* coinData, s32 i){
    struct CoinInfo* coin = &(coinData->coins[i]);

    u32 coinAmount = coinAmounts[coin->type];
    gMarioState->numCoins += coinAmount;
    gMarioState->healCounter += 4 * coinAmount;
#ifdef BREATH_METER
    gMarioState->breathCounter += (4 * coinAmount);
#endif

#ifdef X_COIN_STAR
    if (COURSE_IS_MAIN_COURSE(gCurrCourseNum) && gMarioState->numCoins - coinAmount < X_COIN_STAR
        && gMarioState->numCoins >= X_COIN_STAR) {
        bhv_spawn_star_no_level_exit(STAR_BP_ACT_100_COINS);
    }
#endif
#if ENABLE_RUMBLE
    if (coinAmount >= 2) {
        queue_rumble_data(5, 80);
    }
#endif
    struct Object *sparkles = spawn_object_at_origin(&gMacroObjectDefaultParent, 0, MODEL_SPARKLES, bhvCoinSparklesSpawner);
    obj_set_pos(sparkles, coin->pos[0], coin->pos[1], coin->pos[2]);
    
    if(coin->type == COIN_BHV_TYPE_RED){
        struct Object *hiddenRedCoinStar = cur_obj_nearest_object_with_behavior(bhvHiddenRedCoinStar);
        if (hiddenRedCoinStar != NULL)
        {
            hiddenRedCoinStar->oHiddenStarTriggerCounter++;

            if (hiddenRedCoinStar->oHiddenStarTriggerCounter < 8) {
                spawn_object_relative(hiddenRedCoinStar->oHiddenStarTriggerCounter, coin->pos[0], coin->pos[1] + 25.f, coin->pos[2], &gMacroObjectDefaultParent, MODEL_NUMBER, bhvOrangeNumber);
            }

            play_sound(SOUND_MENU_COLLECT_RED_COIN + (((u8) hiddenRedCoinStar->oHiddenStarTriggerCounter - 1) << 16), gGlobalSoundSource);
        }
    }

    // Basically, put the coin furthest in the buffer into the no longer
    // needed coin slots.

    // Now you might be wondering
    // "but lilaaaaaaaaaaaaaaaaaaaaa what happens if that coin and this coin are the same"
    // Thats no problem, that just means the stupid coin will be copied once, then the head
    // will be reduced all the same.

    coinData->coins[i] = coinData->coins[coinData->head - 1];
    coinData->coinStates[i] = coinData->coinStates[coinData->head - 1];
    
    if (coinData->head)
        coinData->head--;

}

Gfx coinColours[3][3] = {   {   gsDPSetPrimColor(0, 0, 255, 209, 0, 255), //Yellows
                                gsDPSetEnvColor(207, 83, 0, 255),
                                gsSPEndDisplayList(),         },
                        
                            {   gsDPSetPrimColor(0, 0, 0, 133, 255, 255), //Blues
                                gsDPSetEnvColor(46, 0, 171, 255),
                                gsSPEndDisplayList(),         },

                            {   gsDPSetPrimColor(0, 0, 255, 0, 0, 255), //Reds
                                gsDPSetEnvColor(118, 0, 40, 255),
                                gsSPEndDisplayList(),         },};

// Culls the coin if the console check returns positive and is far away. Returns false if the coin should be skipped.
ALWAYS_INLINE s32 coin_render_distance_cull(struct CoinInfo* coin){
    f32 distance = 0;
    //vec3f_get_dist_squared(&coin->pos, gPlayerCameraState->pos, &distance);
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
//#define CULLING_ON_EMULATOR // Active for testing
// Culls the coin using frustum culling. Returns false if the coin should be skipped.
// Same stuff as obj_is_in_view
ALWAYS_INLINE s32 coin_frustum_cull(struct CoinInfo* coin){
    Vec3f cameraToObject = {0, 0, 0};
    for (register u32 i = 0; i < 3; i++) {
        cameraToObject[i] = ((gCameraTransform[0][i] * coin->pos[0])
                        + (gCameraTransform[1][i] * coin->pos[1])
                        + (gCameraTransform[2][i] * coin->pos[2])
                        +  gCameraTransform[3][i]);
    }
    
    f32 cameraToObjectDepth = cameraToObject[2];

    #define VALID_DEPTH_MIDDLE (-20100.f / 2.f)
    #define VALID_DEPTH_RANGE (19900 / 2.f)
    if (absf(cameraToObjectDepth - VALID_DEPTH_MIDDLE) >= VALID_DEPTH_RANGE + COIN_CULLING_RADIUS) {
        return FALSE;
    }

#ifndef CULLING_ON_EMULATOR
    // If an emulator is detected, skip any other culling.
    if(!(gEmulator & NO_CULLING_EMULATOR_BLACKLIST)){
        return TRUE;
    }
#endif

#ifdef VERTICAL_CULLING
    f32 vScreenEdge = -cameraToObjectDepth * gCurGraphNodeCamFrustum->halfFovVertical;

    // Unlike with horizontal culling, we only check if the object is bellow the screen
    // to prevent shadows from being culled.
    if (cameraToObject[1] < -vScreenEdge - cullingRadius) {
        return FALSE;
    }

#endif
    
    f32 hScreenEdge = -cameraToObjectDepth * gCurGraphNodeCamFrustum->halfFovHorizontal;

    if (absf(cameraToObject[0]) > hScreenEdge + COIN_CULLING_RADIUS) {
        return FALSE;
    }
    return TRUE;
}

// Checks for the blue coin switch state and deactivates the render if needed. 
// Returns false if the coin should be skipped.
ALWAYS_INLINE s32 coin_hidden_blue_logic(struct CoinState* coinState){
    if (blueCoinSwitchActiveState != BLUE_COIN_SWITCH_BLINKING){
        coinState->render = FALSE;

        if (blueCoinSwitchActiveState == BLUE_COIN_SWITCH_IDLE) 
            return FALSE;
    }
    return TRUE;
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
    struct CoinInfo *coin = &coinData->coins[index];
    struct CoinState *coinState = &coinData->coinStates[index];

    if (!coin_room_cull(coinState) || !coin_render_distance_cull(coin)){
        coinState->render = FALSE;
        return;
    }

    coinState->render = coin_frustum_cull(coin);
    if (coin->type == COIN_BHV_TYPE_BLUE && !coin_hidden_blue_logic(coinState))
        return;

    if (coin_hitbox_intersects_with_mario(coin)){
        collect_coin(coinData, index);
        coinState->render = FALSE;
    }
    else if (coinState->render){
        gRenderableCoins += coinState->render;
    }
}

void lvl_process_coins(struct CoinAreaData* coinData){ 
    gRenderableCoins = 0;

    for(register u32 i = 0; i < coinData->head; i++){
        lvl_process_coin(coinData, i);
    }

	print_text_fmt_int(150, 20, "Coin Count %d", coinData->head);
}

OSTime lastCoinRender = 0x00;

#define FRAME_FROM_TIMER(fps) (gGlobalTimer / (30 / fps))
#define GET_TEXTURE_FRAME(frames, fps) frames[FRAME_FROM_TIMER(fps) % ARRAY_COUNT(frames)]

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
    gDPSetTextureImage(coinDLHead++, G_IM_FMT_IA, G_IM_SIZ_8b, 64, GET_TEXTURE_FRAME(coin_frames, 15))
    gSPDisplayList(coinDLHead++, coin_dl_start);
    gSPDisplayList(shadowDLHead++, dl_shadow_circle);

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

        Mtx *mtx = alloc_display_list(sizeof(*mtx));

        vec3s_to_vec3f(coinMat[3], coin.pos); // Only slightly cringe.
        mtxf_to_mtx(mtx, coinMat);
        gSPMatrix(coinDLHead++, mtx, G_MTX_NOPUSH | G_MTX_LOAD);

        gSPVertex(coinDLHead++, coin_vertices + coin.type * 4, 4, 0);
        gSP2Triangles(coinDLHead++, 0,  1,  2, 0x0,  0,  2,  3, 0x0);
 
        f32 *pos = coinMat[3];
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
