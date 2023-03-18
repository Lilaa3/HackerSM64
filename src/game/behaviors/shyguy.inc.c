
/**
 * Hitbox for the shyguys.
 */
static struct ObjectHitbox sShyguyHitbox = {
    /* interactType:      */ INTERACT_BOUNCE_TOP,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 3,
    /* health:            */ 0,
    /* numLootCoins:      */ 2,
    /* radius:            */ 72,
    /* height:            */ 50,
    /* hurtboxRadius:     */ 42,
    /* hurtboxHeight:     */ 40,
};


/**
 * Attack handler for the shyguys.
 */
static u8 sShyguyAttackHandler[6] = {
    /* ATTACK_PUNCH:                 */ ATTACK_HANDLER_KNOCKBACK,
    /* ATTACK_KICK_OR_TRIP:          */ ATTACK_HANDLER_KNOCKBACK,
    /* ATTACK_FROM_ABOVE:            */ ATTACK_HANDLER_SQUISHED,
    /* ATTACK_GROUND_POUND_OR_TWIRL: */ ATTACK_HANDLER_SQUISHED,
    /* ATTACK_FAST_ATTACK:           */ ATTACK_HANDLER_KNOCKBACK,
    /* ATTACK_FROM_BELOW:            */ ATTACK_HANDLER_KNOCKBACK,
};

/**
 * Initialization function for shyguy.
 */
void bhv_shyguy_init(void) {
    struct Surface *floor;

    o->oDeathSound = SOUND_OBJ_ENEMY_DEATH_LOW;

    obj_set_hitbox(o, &sShyguyHitbox);

    o->oDrawingDistance = 1.53 * 100.f * 200.f;
    o->oDamageOrCoinValue = 2;

    o->oGravity = -8.0f / 3.0f;

    o->oHomeY = find_floor(o->oPosX, o->oPosY, o->oPosZ, &floor);

}

/**
 * Enter the jump action and set initial y velocity.
 */
static void shyguy_begin_jump(void) {
    cur_obj_play_sound_2(SOUND_OBJ_GOOMBA_ALERT);

    o->oAction = GOOMBA_ACT_JUMP;
    o->oForwardVel = 0.0f;
    o->oVelY = 50.0f / 3.0f;
}

/**
 * Walk around randomly occasionally jumping. If mario comes within range,
 * chase him.
 */
static void shyguy_act_walk(void) {
    treat_far_home_as_mario(1000.0f);

    obj_forward_vel_approach(o->oGoombaRelativeSpeed, 0.4f);

    // If walking fast enough, play footstep sounds
    if (o->oGoombaRelativeSpeed > 4.0f / 3.0f) {
        cur_obj_play_sound_at_anim_range(2, 17, SOUND_OBJ_GOOMBA_WALK);
    }

    //! By strategically hitting a wall, steep slope, or another shyguy, we can
    //  prevent the shyguy from turning back toward home for a while (shyguy
    //  chase extension)
    //! It seems theoretically possible to get 2-3 shyguys to repeatedly touch
    //  each other and move arbitrarily far from their home, but it's
    //  extremely precise and chaotic in practice, so probably can't be performed
    //  for nontrivial distances
    if (o->oGoombaTurningAwayFromWall) {
        o->oGoombaTurningAwayFromWall = obj_resolve_collisions_and_turn(o->oGoombaTargetYaw, 0x200);
    } else {
        // If far from home, walk toward home.
        if (o->oDistanceToMario >= 25000.0f) {
            o->oGoombaTargetYaw = o->oAngleToMario;
            o->oGoombaWalkTimer = random_linear_offset(20, 30);
        }

        if (!(o->oGoombaTurningAwayFromWall =
                  obj_bounce_off_walls_edges_objects(&o->oGoombaTargetYaw))) {
            if (o->oDistanceToMario < 1300.0f) {
                if (o->oOpacity == 0){
                    o->oOpacity = 1;
                    spawn_object_relative(0, 0, 50, 0, o, MODEL_EXCLAMATION_MARK, bhvOrangeNumber);
                }

                // If close to mario, begin chasing him. If not already chasing
                // him, jump first

                if (o->oGoombaRelativeSpeed <= 2.0f) {
                    shyguy_begin_jump();
                }

                o->oGoombaTargetYaw = o->oAngleToMario;
                o->oGoombaRelativeSpeed = 20.0f;
            } else {
                o->oOpacity = 0;
                // If mario is far away, walk at a normal pace, turning randomly
                // and occasionally jumping

                o->oGoombaRelativeSpeed = 4.0f / 3.0f;

                if (o->oGoombaWalkTimer != 0) {
                    o->oGoombaWalkTimer--;
                } else {
                    if (random_u16() & 3) {
                        o->oGoombaTargetYaw = obj_random_fixed_turn(0x2000);
                        o->oGoombaWalkTimer = random_linear_offset(100, 100);
                    } else {
                        shyguy_begin_jump();
                        o->oGoombaTargetYaw = obj_random_fixed_turn(0x6000);
                    }
                }
            }
        }

        cur_obj_rotate_yaw_toward(o->oGoombaTargetYaw, 0x800);
    }
}

/**
 * This action occurs when either the shyguy attacks mario normally, or mario
 * attacks a huge shyguy with an attack that doesn't kill it.
 */
static void shyguy_act_attacked_mario(void) {
    shyguy_begin_jump();
    o->oGoombaTargetYaw = o->oAngleToMario;
    o->oGoombaTurningAwayFromWall = FALSE;
}

/**
 * Move until landing, and rotate toward target yaw.
 */
static void shyguy_act_jump(void) {
    obj_resolve_object_collisions(NULL);

    //! If we move outside the shyguy's drawing radius the frame it enters the
    //  jump action, then it will keep its velY, but it will still be counted
    //  as being on the ground.
    //  Next frame, the jump action will think it has already ended because it is
    //  still on the ground.
    //  This puts the shyguy back in the walk action, but the positive velY will
    //  make it hop into the air. We can then trigger another jump.
    if (o->oMoveFlags & OBJ_MOVE_MASK_ON_GROUND) {
        o->oAction = GOOMBA_ACT_WALK;
    } else {
        cur_obj_rotate_yaw_toward(o->oGoombaTargetYaw, 0x800);
    }
}

/**
 * Update function for goomba.
 */
void bhv_shyguy_loop(void) {
    f32 animSpeed;

    if (obj_update_standard_actions(1)) {
        cur_obj_update_floor_and_walls();

        if ((animSpeed = (o->oForwardVel * 0.4f)) < 1.0f) {
            animSpeed = 1.0f;
        }
        cur_obj_init_animation_with_accel_and_sound(GOOMBA_ANIM_DEFAULT, animSpeed);

        switch (o->oAction) {
            case GOOMBA_ACT_WALK:
                shyguy_act_walk();
                break;
            case GOOMBA_ACT_ATTACKED_MARIO:
                shyguy_act_attacked_mario();
                break;
            case GOOMBA_ACT_JUMP:
                shyguy_act_jump();
                break;
        }
        if (obj_handle_attacks(&sShyguyHitbox, GOOMBA_ACT_ATTACKED_MARIO,
                               sShyguyAttackHandler)
                               && (o->oAction != GOOMBA_ACT_ATTACKED_MARIO)) {
            if (o->parentObj != o)
                o->parentObj->oShyguySpawnerCount -= 1;

            spawn_object_relative(0, 0, 120, 0, o, MODEL_BOB_SHYGUY_MASK, bhvShyGuyMask);
        }

        cur_obj_move_standard(-78);
    } else {
        o->oAnimState = GOOMBA_ANIM_STATE_EYES_CLOSED;
    }
}

void bhv_shyguy_spawner_init(void) {

}

void bhv_shyguy_spawner_loop(void) {
    if (o->oShyguySpawnerCount == 0){
        if (o->oTimer > 2 * 30 && (lateral_dist_between_objects(gMarioObject, o) > 800.f || !(gMarioState->flags & MARIO_SHYGUY_MASK))){
            spawn_object_relative(0, 0, 0, 0, o, MODEL_BOB_SHYGUY, bhvShyGuy);
            o->oShyguySpawnerCount += 1;
        }
    }
    else{
        o->oTimer = 0;
    }
}

#define ESTALACTITE_Y_OFFSET 5.f * 100.f
void bhv_estalactite_init(){
    o->oDrawingDistance = 3 * 100.f * 200.f;
    o->oPosY -= ESTALACTITE_Y_OFFSET;
    o->header.gfx.angle[0] = random_u16();
}

struct ObjectHitbox sEstalactiteHitbox = {
    /* interactType:      */ INTERACT_DAMAGE,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 2,
    /* health:            */ 0,
    /* numLootCoins:      */ 3,
    /* radius:            */ 1.f * 100 - 10,
    /* height:            */ 7.f * 100 - 10,
    /* hurtboxRadius:     */ 1.f * 100,
    /* hurtboxHeight:     */ 7.f * 100,
};

void bhv_estalactite_loop(){
    f32 floorHeight;
    f32 latDistToMario;
    struct Surface *floor;

    obj_set_hitbox(o, &sEstalactiteHitbox);
    switch (o->oAction){
        case 1:
            floorHeight = find_floor(o->oPosX, o->oPosY, o->oPosZ, &floor);
            if (!approach_f32_symmetric_bool(&o->oPosY, floorHeight, 120.f)){
                cur_obj_shake_screen(SHAKE_POS_SMALL);
                cur_obj_play_sound_2(SOUND_GENERAL_GRINDEL_ROLL);
                spawn_object_relative(0, 0, 0, 0, o, MODEL_NONE, bhvMistCircParticleSpawner);
                o->oAction = 2;
            }
        break;
        case 2:
            #define WAIT 3 * 30
            cur_obj_wait_then_blink(WAIT - 30, WAIT);

            if (o->oTimer >= WAIT){
                o->oPosY = o->oHomeY - ESTALACTITE_Y_OFFSET;
                o->oAction = 0;
            }
        break;
        default:
            latDistToMario = lateral_dist_between_objects(gMarioObject, o); 
            if (o->oTimer >= 2 * 30 && latDistToMario < 300.f){
                o->oAction = 1;
            }
        break;
    }

    if (o->oInteractStatus & INT_STATUS_INTERACTED) {
        o->oInteractStatus = INT_STATUS_NONE;
    }
}

struct ObjectHitbox sScubbaguyHitbox = {
    /* interactType:      */ INTERACT_DAMAGE,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 2,
    /* health:            */ 0,
    /* numLootCoins:      */ 3,
    /* radius:            */ 1.5f * 100 - 10,
    /* height:            */ 2.f * 100 - 10,
    /* hurtboxRadius:     */ 1.f * 100,
    /* hurtboxHeight:     */ 1.f * 100,
};

void bhv_scubbaguy_init(){
    cur_obj_init_animation(0x00);
    o->oPosY += 150;
    o->oHomeY = o->oPosY;
    scubbaguy_change_dir();
}

void scubbaguy_change_dir(){
    o->oTimer = 0;
    o->oForwardVel = 0;

    Vec3f d;
    vec3f_diff(d, &o->oScubbaGuyTargetVec, &o->oPosVec);
    o->oScubbaGuyTargetYaw = atan2s(d[2], d[0]);
}

void scubbaguy_change_dir_rand(){
    o->oScubbaGuyTargetX = o->oHomeX + (random_float() * 1000.f - 500.f);
    o->oScubbaGuyTargetY = o->oHomeY + (random_float() * 1000.f - 500.f);
    o->oScubbaGuyTargetZ = o->oHomeZ + (random_float() * 1000.f - 500.f);
    scubbaguy_change_dir();
}

void scubbaguy_change_dir_mario(){
    vec3f_copy(&o->oScubbaGuyTargetVec, &gMarioState->marioObj->oPosVec);
    scubbaguy_change_dir();
}

void bhv_scubbaguy_loop(){
    o->oPosY = o->oHomeY;

    obj_set_hitbox(o, &sScubbaguyHitbox);

    cur_obj_update_floor_and_walls();

    o->oForwardVel = approach_f32_asymptotic(o->oForwardVel, 20.0f, 0.5f);

    Vec3f d;
    vec3f_diff(d, &o->oScubbaGuyTargetVec, &o->oPosVec);

    o->oMoveAngleYaw = approach_s16_symmetric(o->oMoveAngleYaw, o->oScubbaGuyTargetYaw, 0x400);
    o->oFaceAngleYaw = approach_s16_symmetric(o->oFaceAngleYaw, o->oMoveAngleYaw, 0x300);

    if (o->oDistanceToMario < 1000.f && gMarioState->pos[1] <= gMarioState->waterLevel){
        if (o->oTimer >= 15)
            scubbaguy_change_dir_mario();
        
        if (o->oAction == 0){
            cur_obj_play_sound_2(SOUND_OBJ_GOOMBA_ALERT);
            spawn_object_relative(0, 0, 50, 0, o, MODEL_EXCLAMATION_MARK, bhvOrangeNumber);
            o->oAction = 1;
        }
    }
    else{
        o->oAction = 0;
        if (o->oTimer >= 3 * 30){
            scubbaguy_change_dir_rand();
        }
        else if (obj_bounce_off_walls_edges_objects(&o->oMoveAngleYaw)){
            scubbaguy_change_dir_rand();
        }
    }

    if (o->header.gfx.animInfo.animFrame == 15){
        cur_obj_play_sound_2(SOUND_ACTION_SWIM_FAST);
    }

    cur_obj_move_standard(-78);

    if (o->oInteractStatus & INT_STATUS_INTERACTED) {
        o->oInteractStatus = INT_STATUS_NONE;
    }
}

#define MESH bob_dl_Geo_area_2_001_mesh_layer_5_vtx_0

extern Vtx MESH[];

#define WATER_VTX_COUNT ARRAY_COUNT(MESH)
#define HEIGHT 20.3745 * 100.f

void bhv_area_controller_init(){
    Vtx* waterVtx = segmented_to_virtual(MESH);
    for(s32 i = 0; i < WATER_VTX_COUNT; i++){
        waterVtx[i].n.flag = waterVtx[i].n.ob[1];
    }
}

void bhv_area_controller_loop(){
    Vtx* waterVtx = segmented_to_virtual(MESH);

    o->oTimer += 255;

    for(s32 i = 0; i < WATER_VTX_COUNT; i++){
        s32 xPlusZ = waterVtx[i].n.ob[0] * waterVtx[i].n.ob[2];
        f32 value = sins(o->oTimer + (xPlusZ / 16)) * 30.f;
        waterVtx[i].n.ob[1] = waterVtx[i].n.flag + value;
    }
}

struct ObjectHitbox sBuoyGuyHitbox = {
    /* interactType:      */ INTERACT_BOUNCE_TOP,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 2,
    /* health:            */ 0,
    /* numLootCoins:      */ 3,
    /* radius:            */ 1.5f * 100 - 10,
    /* height:            */ 2.f * 100 - 10,
    /* hurtboxRadius:     */ 1.f * 100,
    /* hurtboxHeight:     */ 1.f * 100,
};

void bhv_shy_guy_buoy_init(){
    cur_obj_init_animation(0x00);
    obj_set_hitbox(o, &sBuoyGuyHitbox);
    o->oNumLootCoins = 4;
}

void bhv_shy_guy_buoy_loop(){
    o->oFaceAngleYaw = approach_s16_symmetric(o->oFaceAngleYaw, o->oAngleToMario, 0x300);;

    if (o->oInteractStatus & INT_STATUS_INTERACTED) {
        spawn_mist_particles();
        obj_spawn_loot_yellow_coins(o, (o->oNumLootCoins > 0), 20.0f);
        o->oNumLootCoins -= 1;
        cur_obj_play_sound_1(SOUND_OBJ_GOOMBA_ALERT);
        o->oInteractStatus = INT_STATUS_NONE;
    }
}

extern const Collision pillar_collision[];
extern const Collision pillar_ice_collision[];

void bhv_pillar_init(){
    u32 *collisionData;
    if (GET_BPARAM1(o->oBehParams) == 2)
        collisionData = segmented_to_virtual(pillar_ice_collision);
    else
        collisionData = segmented_to_virtual(pillar_collision);

    o->collisionData = collisionData;
}

void bhv_pillar_loop(){
    load_object_collision_model();
}

Gfx *geo_pillar_puzzle(s32 callContext, struct GraphNode *node, UNUSED void *context) {
    Gfx *gfxHead = NULL;
    Gfx *gfx;
    if (callContext == GEO_CONTEXT_RENDER) {
        struct GraphNodeGenerated *currentGraphNode = (struct GraphNodeGenerated *) node;
        struct Object *obj = gCurGraphNodeObjectNode;
        s32 parameter = currentGraphNode->parameter;

        if (parameter == 0){
            parameter = GET_BPARAM1(obj->oBehParams);
        }

        gfxHead = alloc_display_list(3 * sizeof(Gfx));
        gfx = gfxHead;
        SET_GRAPH_NODE_LAYER(currentGraphNode->fnNode.node.flags, LAYER_OPAQUE);

        if (gMarioState->flags & MARIO_SHYGUY_MASK)
        {
            switch (parameter)
            {
                case 1:
                    gDPSetEnvColor(gfx++, 60, 255, 0, 255);
                    break;
                case 2:
                    gDPSetEnvColor(gfx++, 220, 60, 0, 255);
                    break;
                case 3:
                    gDPSetEnvColor(gfx++, 0, 60, 175, 255);
                    break;
            }
        }
        else{
            gDPSetEnvColor(gfx++, 255, 255, 255, 255);
        }
        

        gSPEndDisplayList(gfx);
    }

    return gfxHead;
}