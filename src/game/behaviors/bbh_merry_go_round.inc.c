
/**
 * Behavior for bhvMerryGoRound.
 * This is the merry-go-round in BBH.
 */

/**
 * This function handles the merry-go-round's music.
 * It starts the music when Mario enters the room around the
 * merry-go-round's enclosure, and ends the music when he's neither
 * in the enclosure nor in the room around it.
 */
static void handle_merry_go_round_music(void) {
    // If the music should play, play it and check whether it still should.
    // Otherwise, don't play it and check whether it should.
    if (o->oMerryGoRoundMusicShouldPlay == FALSE) {
        if (gMarioCurrentRoom == BBH_NEAR_MERRY_GO_ROUND_ROOM) {
            // Play the merry-go-round and BBH music at the same time
            play_secondary_music(SEQ_EVENT_MERRY_GO_ROUND, 45, 20, 200);
            // Set to TRUE
            o->oMerryGoRoundMusicShouldPlay++;
        }
    } else {
        // Get Mario's floor and floor surface type
        struct Surface *marioFloor = gMarioState->floor;
        CollisionType marioFloorType;

        if (marioFloor == NULL) {
            marioFloorType.asValue = SURFACE_DEFAULT;
        } else {
            marioFloorType = marioFloor->type;
        }

        // All floors in the merry-go-round's enclosure have a special type.
        if (marioFloorType.warpsAndLevel == COL_TYPE_MUSIC) {
            // If Mario is in the merry-go-round's enclosure, play only the merry-go-round music.
            play_secondary_music(SEQ_EVENT_MERRY_GO_ROUND, 0, 78, 50);
            gMarioOnMerryGoRound = TRUE;
        } else {
            // If Mario is not in the merry-go-round's enclosure,
            // i.e. he's around it, play both the merry-go-round music and the BBH music.
            play_secondary_music(SEQ_EVENT_MERRY_GO_ROUND, 45, 20, 200);
            gMarioOnMerryGoRound = FALSE;
        }

        // If Mario is not in the merry-go-round's area of the basement anymore,
        // stop playing the music.
        // If he is, play the creaking sound.
        if (
            // The merry-go-round is a dynamic surface.
            gMarioCurrentRoom != BBH_DYNAMIC_SURFACE_ROOM
            && gMarioCurrentRoom != BBH_NEAR_MERRY_GO_ROUND_ROOM) {
            func_80321080(300); //! Switch to BBH music? FIXME: Audio needs labelling
            o->oMerryGoRoundMusicShouldPlay = FALSE;
        } else {
            cur_obj_play_sound_1(SOUND_ENV_MERRY_GO_ROUND_CREAKING);
        }
    }
}

/**
 * Merry-go-round update function.
 */
void bhv_merry_go_round_loop(void) {
    // Surprisingly, the merry-go-round is what's responsible
    // for playing the howling wind sound in BBH.
    if (!o->oMerryGoRoundMarioIsOutside) {
        if (gMarioCurrentRoom == BBH_OUTSIDE_ROOM) {
            // Set to TRUE
            o->oMerryGoRoundMarioIsOutside++;
        }
    } else {
        play_sound(SOUND_AIR_HOWLING_WIND, gGlobalSoundSource);

        if (
            // There are objects outside BBH, such as corkboxes.
            // The howling wind should not stop when Mario stands on a cork box.
            //! @bug Interestingly, this means if Mario goes from outside
            // to a dynamic surface *inside* the mansion in a single frame,
            // the howling wind music will still play.
            gMarioCurrentRoom != BBH_OUTSIDE_ROOM && gMarioCurrentRoom != BBH_DYNAMIC_SURFACE_ROOM) {
            o->oMerryGoRoundMarioIsOutside = FALSE;
        }
    }

    // Rotate the merry-go-round and play appropriate music if it's not stopped.
    if (!o->oMerryGoRoundStopped) {
        o->oAngleVelYaw = 0x80;
        o->oMoveAngleYaw += o->oAngleVelYaw;
        o->oFaceAngleYaw += o->oAngleVelYaw;
        handle_merry_go_round_music();
    } else {
        o->oAngleVelYaw = 0;
        func_80321080(300); //! Switch to BBH music? FIXME: Audio needs labelling
    }
}
