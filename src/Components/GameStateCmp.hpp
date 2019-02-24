#pragma once
#include "Component.hpp"

class GameStateCmp : public Cmp
{
public:
    void init();
    bool sam_is_alive;
    int keys;
    std::string current_room;
};
