#pragma once
#include "Cmp.hpp"

class GameStateCmp : public Cmp
{
public:
    void init();
    bool sam_is_alive;
    int keys;
    std::string current_room;

    bool has_moved;
    bool has_pressed_E;
};
