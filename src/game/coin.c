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
#include "coin.h"

Vtx coin_Coin_mesh_vtx_0[8] = {
	{{ {-3, 32, -31}, 0, {93, 92}, {129, 0, 246, 0} }},
	{{ {-3, -32, -31}, 0, {106, 4066}, {129, 0, 246, 0} }},
	{{ {-8, -32, 31}, 0, {4003, 4068}, {129, 0, 246, 0} }},
	{{ {-8, 32, 31}, 0, {3990, 94}, {129, 0, 246, 0} }},
	{{ {-3, 32, -31}, 0, {93, 92}, {127, 0, 10, 0} }},
	{{ {-8, 32, 31}, 0, {3990, 94}, {127, 0, 10, 0} }},
	{{ {-8, -32, 31}, 0, {4003, 4068}, {127, 0, 10, 0} }},
	{{ {-3, -32, -31}, 0, {106, 4066}, {127, 0, 10, 0} }},
};

Gfx coin_shadow_mesh[] = {
	gsSPVertex(coin_Coin_mesh_vtx_0 + 0, 8, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
	gsSPEndDisplayList(),
};

Gfx mat_revert_coin[] = {
	gsSPTexture(0x0001, 0x0001, 0, G_TX_RENDERTILE, G_OFF),
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsSPSetGeometryMode(G_LIGHTING),
    gsSPEndDisplayList(),
};

Gfx mat_revert_coin_shadow[] = {
	gsDPPipeSync(),
	gsSPGeometryMode(0, G_LIGHTING),
	gsSPEndDisplayList(),
};

extern s16 gMatStackIndex;
extern ALIGNED16 Mat4 gMatStack[32];
extern ALIGNED16 Mtx *gMatStackFixed[32];

u32 blueCoinSwitchActiveState = BLUE_COIN_SWITCH_IDLE;

extern u32 sLevelsWithRooms[];

struct CoinInfo *spawn_coin(Vec3f pos, u32 type, u32 flags) {
    struct Area* area = gCurrentArea;
    struct CoinAreaData* coinData = &(gCurrentArea->coinData);

    struct CoinInfo *coin = &coinData->coins[coinData->head];
    struct CoinState *coinState = &coinData->coinStates[coinData->head];
    coinData->head++;

    coin->pos[0] = pos[0];
    coin->pos[2] = pos[2];

    struct Surface *floor = NULL;

    if(flags & COIN_FLAG_SNAP_TO_GROUND){
        coin->pos[1] = find_floor(coin->pos[0], pos[1] + 40, coin->pos[2], &floor);
    }
    else{
        coin->pos[1] = pos[1] + 32;
    }
    
    coin->type = type;

    coinState->render = TRUE;
    coinState->room = -1;
    if (is_item_in_array(gCurrLevelNum, sLevelsWithRooms)) {
        find_room_floor(coin->pos[0], coin->pos[1], coin->pos[2], &floor);

        if (floor != NULL) {
            coinState->room = floor->room;
        }
    }

    return coin;
}

void lvl_clean_coins(){
    for(int i = 0; i < AREA_COUNT; i++){  
        struct Area* area = &gAreaData[i];
        struct CoinAreaData* coinData = &(area->coinData);

        for(int j = 0; j < COIN_ARRAY_SIZE; j++){
            struct CoinInfo *coin = &coinData->coins[coinData->head];
            struct CoinState *coinState = &coinData->coinStates[coinData->head];

            vec3_zero(coin->pos);
            coin->type = COIN_BHV_TYPE_YELLOW;
            coinState->render = FALSE;
            coinState->room = -1;
        }
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
    spawn_object_at_origin(NULL, 0, MODEL_SPARKLES, bhvCoinSparklesSpawner);

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

    // WE no longer need this shit coin, get fucking rid of it

    // Basically, put the coin furthest in the buffer into the no longer
    // needed coin slots.

    // Now you might be wondering
    // "but acerolaaa what happens if that coin and this coin are the same"
    // Thats no problem, that just means the stupid coin will be copied once, then the head
    // will be reduced all the same.

    coinData->coins[i] = coinData->coins[coinData->head - 1];
    coinData->coinStates[i] = coinData->coinStates[coinData->head - 1];
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
    return (distance > sqr(COIN_RENDER_DISTANCE)); // Compiler will handle the sqr, no need for another define
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
    f32 dya_bottom = coin->pos[1] - 125;

    f32 marioX = gMarioObject->oPosX;
    f32 marioY = gMarioObject->oPosY;
    f32 marioZ = gMarioObject->oPosZ;

    f32 dyb_bottom = marioY - gMarioObject->hitboxDownOffset;
    f32 dx = coin->pos[0] - marioX;
    f32 dz = coin->pos[2] - marioZ;
    f32 collisionRadius = 100 + gMarioObject->hurtboxRadius;
    f32 lateraldistance = sqr(dx) + sqr(dz);

    if (sqr(collisionRadius) > lateraldistance) {
        f32 dya_top = 125 + dya_bottom;
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

    coinState->render = (coin_frustum_cull(coin) == FALSE);

    if (coin->type == COIN_BHV_TYPE_BLUE && !coin_hidden_blue_logic(coinState))
        return;

    if (coin_hitbox_intersects_with_mario(coin))
        collect_coin(coinData, index);

    if (coinState->render)
        gRenderableCoins += 1;
}

void lvl_process_coins(struct CoinAreaData* coinData){ 
    gRenderableCoins = 0;

    for(register u32 i = 0; i <= coinData->head; i++){
        lvl_process_coin(coinData, i);
    }

	print_text_fmt_int(180, 36, "Coin count %d", coinData->head);
}

OSTime lastCoinRender = 0x00;

//Called before rendering global particles. Render coins
void render_coins() {
    struct Area* area = gCurrentArea;
    struct CoinAreaData* coinData = &(area->coinData);

    //Allocate display list for the material, coins and their respective propreties.
    Gfx *coinDL = alloc_display_list(((sizeof(Gfx) * 3) * gRenderableCoins) + (sizeof(Gfx) * 3));
    Gfx *coinDLHead = coinDL;
    Gfx *shadowDL = alloc_display_list(((sizeof(Gfx) * 5) * gRenderableCoins) + (sizeof(Gfx) * 3));
    Gfx *shadowDLHead = shadowDL;

    Mat4 mtxFloat;
    Vec3f pos = {0, 0, 0};
    Vec3f shadowScale = {1.f, 1.f, 1.f};
    Vec3f floorNormal = {0, 0, 0};
    	
    //Set coin materials
    // gSPDisplayList(coinDLHead++, mat_coin);
    gSPDisplayList(shadowDLHead++, dl_shadow_circle);

    for(u32 i = 0; i < coinData->head; i++){
        if(!coinData->coinStates[i].render) 
            continue;

        struct CoinInfo *coin = &coinData->coins[i];

        //Set env and prim colour (Color variations to be implemented.)
        gSPDisplayList(coinDLHead++, coinColours[coin->type]);

        vec3s_to_vec3f(pos, coin->pos);

        //Calculate transformation matrice and update the mat stack.
        mtxf_billboard(mtxFloat, gMatStack[gMatStackIndex], pos, shadowScale, gCurGraphNodeCamera->roll);
        
        Mtx *mtx = alloc_display_list(sizeof(*mtx));
        mtxf_to_mtx(mtx, mtxFloat);
        
        gSPMatrix(coinDLHead++, mtx, G_MTX_NOPUSH | G_MTX_LOAD);
        //gSPDisplayList(coinDLHead++, coin_mesh); // lol add this back please

        // Shadow logic
        struct Surface *floor = NULL;
        f32 originalY = coin->pos[1];

        coin->pos[1] = find_floor(coin->pos[0], originalY, coin->pos[2], &floor);
        f32 distanceToGround = (originalY - coin->pos[1]);
        if(floor != NULL){
            if(distanceToGround < 1.f){
                distanceToGround = 1.f;
            }
            if(distanceToGround > 300.f){
                distanceToGround = 300.f;
            }

            gDPSetEnvColor(shadowDLHead++, 0, 0, 0, distanceToGround);

            surface_normal_to_vec3f(floorNormal, floor);
            mtxf_shadow(mtxFloat, floorNormal, pos, shadowScale, 0);
            Mtx *mtx2 = alloc_display_list(sizeof(*mtx2));
            mtxf_to_mtx(mtx2, mtxFloat);

            gSPMatrix(shadowDLHead++, mtx2, G_MTX_NOPUSH | G_MTX_LOAD);
            gSPDisplayList(coinDLHead++, coin_shadow_mesh);
        }
    }

    //Revert render settings
    gSPDisplayList(shadowDLHead++, mat_revert_coin);
    gSPDisplayList(coinDLHead++, mat_revert_coin_shadow);

    //Finish display list.
    gSPEndDisplayList(shadowDLHead);
    gSPEndDisplayList(coinDLHead);

    geo_append_display_list(coinDL, LAYER_OPAQUE); 
    geo_append_display_list(shadowDL, LAYER_TRANSPARENT); 
}
