#pragma once

#include "strategies_common.hpp"

class BossStrategy {
public:
    static EnemyAction handleAction(GameStateCmp* gameStateCmp, vec2 currentPosition);
};


bool pos_within_range(vec2 pos1, vec2 pos2, int within);
