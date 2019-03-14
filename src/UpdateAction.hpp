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

// Resetting game
const int RESET_GAME = 5;

// Toggle pause game
const int TOGGLE_PAUSE_GAME = 6;


// Game Logic actions
const int SAM_DEATH = 7;
// Used to trigger sound effects
const int KEY_PICKUP_EVENT = 8;
