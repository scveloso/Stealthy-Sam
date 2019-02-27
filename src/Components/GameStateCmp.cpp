#include "GameStateCmp.hpp"

void GameStateCmp::init() {
    sam_is_alive = true;
    level_two_key = false;
    level_three_key = false;
    current_room = ROOM_ONE_GUID;
}
