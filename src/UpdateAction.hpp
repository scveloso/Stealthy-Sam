// Constants returned by Systems to world.cpp
// if the Systems need to change world state.
//
// Can add stuff here like change rooms, end game, etc.
const int NO_CHANGE = 0;

// Room changes
const int CHANGE_TO_ROOM_ONE_ACTION = 1;
const int ROOM_TWO = 2;
const int ROOM_THREE = 3;
const int ROOM_FOUR = 4;

const int START_NEW_GAME = 5;

// Toggle pause game
const int TOGGLE_PAUSE_GAME = 6;
const int LOAD_GAME = 7;

// Game Logic actions
const int SAM_DEATH = 8;
const int GAME_WIN = 10;
const int SHOOT_MISSILE = 11;
const int TUTORIAL_LEVEL = 12;
const int SPAWN_SMOKE = 13;
