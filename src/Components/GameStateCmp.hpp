#pragma once
#include "Cmp.hpp"

class GameStateCmp : public Cmp
{
public:
    void init();
    bool sam_is_alive;
    int keys;
    std::string current_room;
    vec2 sam_position;

    bool has_moved;
    bool has_pressed_E;

    int held_item; // Used to regenerate held entities when switching rooms
    Entity* held_entity;
};
