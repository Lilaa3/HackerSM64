
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
