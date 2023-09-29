
/**
 * This file contains the initialization and behavior for red coins.
 * Behavior controls audio and the orange number spawned, as well as interacting with
 * the course's red coin star.
 */

#include "src/game/coin.h"

/**
 * Red coin initialization function. Sets the coin's hitbox and parent object.
 */
void bhv_red_coin_init(void) {
    spawn_coin_at_o(COIN_BHV_TYPE_RED, 0);
    obj_mark_for_deletion(o);
}

void collect_red_coin(){
    // ...increment the star's counter.
    o->parentObj->oHiddenStarTriggerCounter++;

    // Spawn the orange number counter, as long as it isn't the last coin.
    if (o->parentObj->oHiddenStarTriggerCounter != o->parentObj->oHiddenStarTriggerTotal) {
        // Cap visible count to 99
        if (o->parentObj->oHiddenStarTriggerCounter > 99) {
            spawn_orange_number(9, 28, 0, 0);
            spawn_orange_number(9, -28, 0, 0);
        }
        else if (o->parentObj->oHiddenStarTriggerCounter >= 10) {
            spawn_orange_number(o->parentObj->oHiddenStarTriggerCounter % 10, 28, 0, 0);
            spawn_orange_number(o->parentObj->oHiddenStarTriggerCounter / 10, -28, 0, 0);
        }
        else {
            spawn_orange_number(o->parentObj->oHiddenStarTriggerCounter, 0, 0, 0);
        }
    }

#ifdef JP_RED_COIN_SOUND
    // For JP version, play an identical sound for all coins.
    create_sound_spawner(SOUND_GENERAL_RED_COIN);
#else
    if (o->parentObj->oHiddenStarTriggerTotal - o->parentObj->oHiddenStarTriggerCounter > 7) {
        // Play the first red coin sound until it gets to the final 8
        play_sound(SOUND_MENU_COLLECT_RED_COIN, gGlobalSoundSource);
    }
    else {
        // On all versions but the JP version, each coin collected plays a higher noise.
        play_sound(SOUND_MENU_COLLECT_RED_COIN
                + (((u8) 7 - (o->parentObj->oHiddenStarTriggerTotal - o->parentObj->oHiddenStarTriggerCounter)) << 16),
                gGlobalSoundSource);
    }
#endif
}
