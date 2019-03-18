#include "BossStrategy.hpp"

EnemyAction BossStrategy::handleAction(GameStateCmp* gameStateCmp) {
    switch (gameStateCmp->num_lit_cauldrons) {
        case 1:
            return MOVE_TO_TOP_RIGHT_QUAD;
        case 2:
            return MOVE_TO_BOTTOM_LEFT_QUAD;
        case 3:
            return MOVE_TO_BOTTOM_RIGHT_QUAD;
        case 4:
            // TODO: kill the boss
            return FADE_AWAY;
        default:
            return MAINTAIN_ACTION;
    }
}
