// Constants used to know which entity to generate from
// parsing the .json file.
// Also has constants for the soft grid-system implemented
// to know where to generate each entity. 

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;

const float TILE_WIDTH = 50.f; // 16x16 tiles scaled up by 3.125
const float TILE_HEIGHT = 50.f;
const int NUM_COLS = 24;	// Screen width divided by pixel width
const int NUM_ROWS = 16;	// Screen height divided by pixel height

// Tile values created by Tiled
const int SAM = 105;
const int WALL = 34;
const int KEY = 494;
const int CLOSET = 496;
const int DOOR = 484;
const int ENEMY = 376;
const int TORCH = 110;

// Custom Door values to tell the game where each door leads to
const int DOOR_ROOM_1_TO_2 = 48412;
const int DOOR_ROOM_2_TO_1 = 48421;
const int DOOR_ROOM_2_TO_3 = 48423;
const int DOOR_ROOM_3_TO_2 = 48432;
