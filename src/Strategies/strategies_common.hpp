#include <Components/GameStateCmp.hpp>
enum EnemyAction {
    MAINTAIN_ACTION = 0,
    PATROL = 1,
    CHASE_SAM = 2,
    RETURN_TO_PATROL = 3,
    CHASE_TORCH = 4,

    // Boss actions
    MOVE_TO_TOP_RIGHT_QUAD = 5,
    MOVE_TO_BOTTOM_LEFT_QUAD = 6,
    MOVE_TO_BOTTOM_RIGHT_QUAD = 7,
    FADE_AWAY = 8,
};

enum x { a, b};