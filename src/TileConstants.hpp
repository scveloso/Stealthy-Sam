const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;

const float TILE_WIDTH = 50.f; // 16x16 tiles scaled up by 3.125
const float TILE_HEIGHT = 50.f;
const int NUM_COLS = 24;	// Screen width divided by pixel width
const int NUM_ROWS = 16;	// Screen height divided by pixel height

// Tile values created by Tiled
const int SAM = 61;
const int TOP_WALL = 313;
const int TOP_LEFT_CORNER = 312;
const int TOP_RIGHT_CORNER = 314;
const int LEFT_WALL = 318;
const int RIGHT_WALL = 320;
const int BOTTOM_LEFT_CORNER = 324;
const int BOTTOM_RIGHT_CORNER = 326;
const int BOTTOM_WALL = 325;
const int BLACK_TILE = 345;
const int CHEST = 18;
const int ENEMY = 132;
const int DOOR_ROOM_1_TO_2 = 1345;
const int DOOR_ROOM_2_TO_1 = 1346;
const int DOOR_ROOM_2_TO_3 = 1347;
const int DOOR_ROOM_3_TO_2 = 1348;
const int CLOSET = 381;
const int FLOOR_TILE = 76;
const int SMALL_TABLE_TL = 2116;
const int SMALL_TABLE_TR = 2117;
const int SMALL_TABLE_BL = 2148;
const int SMALL_TABLE_BR = 2149;
const int BIG_TABLE_TL = 2144;
const int BIG_TABLE_TOP_EDGE = 2145;
const int BIG_TABLE_TR = 2147;
const int BIG_TABLE_LEFT_EDGE = 2208;
const int BIG_TABLE_RIGHT_EDGE = 2179;
const int BIG_TABLE_FILL = 2177;
const int BIG_TABLE_BL = 2240;
const int BIG_TABLE_BR = 2243;
const int BIG_TABLE_BOT_EDGE = 2241;
const int BIG_TABLE_LEFT_LEG = 2272;
const int BIG_TABLE_RIGHT_LEG = 2275;
const int BIG_TABLE_SHADOW = 2273;
const int RIGHT_COUNTER_BR = 2005;
const int RIGHT_COUNTER_BL = 2004;
const int RIGHT_COUNTER_BML = 1391;
const int RIGHT_COUNTER_BMR = 1392;
const int RIGHT_COUNTER_RIGHT_CORNER = 1360;
const int RIGHT_COUNTER_LEFT_CORNER = 1359;
const int RIGHT_COUNTER_TR = 1264;
const int RIGHT_COUNTER_TL = 1263;
const int RIGHT_COUNTER_ML = 1295;
const int RIGHT_COUNTER_MR = 1296;
const int LEFT_COUNTER_BR = 1422;
const int LEFT_COUNTER_BL = 1421;
const int LEFT_COUNTER_BML = 1389;
const int LEFT_COUNTER_BMR = 1390;
const int LEFT_COUNTER_RIGHT_CORNER = 1358;
const int LEFT_COUNTER_LEFT_CORNER = 1357;
const int LEFT_COUNTER_TL = 1261;
const int LEFT_COUNTER_TR = 1262;
const int LEFT_COUNTER_ML = 1293;
const int LEFT_COUNTER_MR = 1294;
const int CHAIR_TL = 1990;
const int CHAIR_ML = 2022;
const int CHAIR_BL = 2054;
const int CHAIR_TR = 1991;
const int CHAIR_MR = 2023;
const int CHAIR_BR = 2055;
const int LEFT_CHAIR_TL = 2026;
const int LEFT_CHAIR_TR = 2027;
const int LEFT_CHAIR_BL = 2058;
const int LEFT_CHAIR_BR = 2059;
const int RIGHT_CHAIR_TL = 2124;
const int RIGHT_CHAIR_TR = 2029;
const int RIGHT_CHAIR_BL = 2156;
const int RIGHT_CHAIR_BR = 2061;
const int DISH_CAB_TL = 2036;
const int DISH_CAB_TR = 2039;
const int DISH_CAB_BL = 2070;
const int DISH_CAB_BR = 2071;
const int MAT_TL = 1862;
const int MAT_TR = 1863;
const int MAT_BL = 1894;
const int MAT_BR = 1895;
const int CAB_TL = 3206;
const int CAB_TR = 3207;
const int CAB_BL = 3268;
const int CAB_BR = 3269;
const int CLOCK_TL = 3424;
const int CLOCK_TR = 3425;
const int CLOCK_BL = 3456;
const int CLOCK_BR = 3457;
const int LIGHT_LEFT = 2226;
const int LIGHT_RIGHT = 2227;
const int SIDE_CAB_T = 2986;
const int SIDE_CAB_M = 3018;
const int SIDE_CAB_ML = 3050;
const int SIDE_CAB_L = 3082;
const int KEY = 3806;


//const int SAM = 105;
//const int WALL = 34;
//const int CLOSET = 496;
//const int DOOR = 484;
//const int ENEMY = 376;

// Doors
const int DOOR_ROOM_1_TO_2 = 48412;
const int DOOR_ROOM_2_TO_1 = 48421;
const int DOOR_ROOM_2_TO_3 = 48423;
const int DOOR_ROOM_3_TO_2 = 48432;
