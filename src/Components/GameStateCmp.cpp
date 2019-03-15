#include "GameStateCmp.hpp"

void GameStateCmp::init() {
    sam_is_alive = true;
    level_two_key = false;
    level_three_key = false;
    previous_room = "";
    current_room = ROOM_ONE_GUID;

    // Tutorial status
    has_moved = false;
    has_pressed_E = false;

    // Held entity
    held_item = -1;
    held_entity = NULL;

    num_lit_cauldrons = 0;
}
