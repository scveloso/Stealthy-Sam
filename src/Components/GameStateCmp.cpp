#include "GameStateCmp.hpp"

void GameStateCmp::init() {
    sam_is_alive = true;
    keys = 0;
    current_room = ROOM_ONE_GUID;

    // Tutorial status
    has_moved = false;
    has_pressed_E = false;

    // Held entity
    held_item = -1;
}
