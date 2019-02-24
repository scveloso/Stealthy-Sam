#include "GameStateCmp.hpp"

void GameStateCmp::init() {
    sam_is_alive = true;
    keys = 0;
    current_room = ROOM_ONE_GUID;
}
