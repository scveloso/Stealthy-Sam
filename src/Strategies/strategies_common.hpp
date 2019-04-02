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
    BOSS_SHOOT_MISSILE = 9,
};

const vec2 BOSS_POSITION_TOP_RIGHT =  { 750.f, 335.f };
const vec2 BOSS_POSITION_BOTTOM_LEFT =  { 250.f, 650.f };
const vec2 BOSS_POSITION_BOTTOM_RIGHT = { 750.f, 650.f };

const int TIME_BETWEEN_MISSILES = 2000;
