#pragma once
#include "Cmp.hpp"

class GameStateCmp : public Cmp
{
public:
    void init();

    void saveGame();
    bool loadGame();

    bool sam_is_alive;
    bool level_two_key;
    bool level_three_key;

    std::string previous_room;
    std::string current_room;

    vec2 sam_position;
    bool hidden;

    vec2 cursor_pos;

    bool has_moved;
    bool has_pressed_H;

    bool is_game_paused;
    bool in_main_menu;

    // Used in EntityGenerator to decide whether to use sam position in game state
    // to spawn Sam
    bool is_game_loading;

    int held_item; // Used to regenerate held entities when switching rooms
    Entity* held_entity;


    int num_lit_cauldrons;

};
