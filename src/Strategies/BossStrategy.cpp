#include <iostream>
#include "BossStrategy.hpp"

EnemyAction BossStrategy::handleAction(GameStateCmp* gameStateCmp, vec2 currentPosition) {
    switch (gameStateCmp->num_lit_cauldrons) {
        case 0:
            return BOSS_SHOOT_MISSILE;
        case 1:
            if (pos_within_range(currentPosition, BOSS_POSITION_TOP_RIGHT, 10)) {
                return BOSS_SHOOT_MISSILE;
            }
            return MOVE_TO_TOP_RIGHT_QUAD;
        case 2:
            if (pos_within_range(currentPosition, BOSS_POSITION_BOTTOM_LEFT, 10)) {
                return BOSS_SHOOT_MISSILE;
            }
            return MOVE_TO_BOTTOM_LEFT_QUAD;
        case 3:
            if (pos_within_range(currentPosition, BOSS_POSITION_BOTTOM_RIGHT, 10)) {
                return BOSS_SHOOT_MISSILE;
            }
            return MOVE_TO_BOTTOM_RIGHT_QUAD;
        case 4:
            return FADE_AWAY;
        default:
            return MAINTAIN_ACTION;
    }
}


bool pos_within_range(vec2 pos1, vec2 pos2, int within) {
    bool x_within = false;
    bool y_within = false;

    if (pos1.x > pos2.x) {
        if ((pos1.x - pos2.x) < within) {
            x_within = true;
        }
    } else {
        if ((pos2.x - pos1.x) < within) {
            x_within = true;
        }
    }

    if (pos1.y > pos2.y) {
        if ((pos1.y - pos2.y) < within) {
            y_within = true;
        }
    } else {
        if ((pos2.y - pos1.y) < within) {
            y_within = true;
        }
    }

    return x_within && y_within;
}