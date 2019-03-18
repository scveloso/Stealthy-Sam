#pragma once
#include "Cmp.hpp"

class GameStateCmp : public Cmp
{
public:
    void init();
    bool sam_is_alive;
    bool level_two_key;
    bool level_three_key;

    std::string previous_room;
    std::string current_room;

    vec2 sam_position;

    bool has_moved;
    bool has_pressed_E;

    int held_item; // Used to regenerate held entities when switching rooms
    Entity* held_entity;


    int num_lit_cauldrons;
};
