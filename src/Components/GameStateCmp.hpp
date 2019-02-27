#pragma once
#include "Cmp.hpp"

class GameStateCmp : public Cmp
{
public:
    void init();
    bool sam_is_alive;
    bool level_two_key;
    bool level_three_key;
    std::string current_room;
};
