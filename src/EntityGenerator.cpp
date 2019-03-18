#include <iostream>
#include "EntityGenerator.hpp"
#include "TileConstants.hpp"
#include "SpawnPoints.hpp"
#include <string>
#include <vector>

using json = nlohmann::json;

// Class to generate entities on game start/restart, and on room changes
//
// First, generateEntities() is called on a .json map made by us, generating
// entities for each layer
// Then, handleTextBoxEntities() is called to generate text box entities
// After, handleHeldItem() is called to generate Sam's held item on room changes
// Lastly, initializeSystems() is called to add all the components to their relevant systems
EntityGenerator::EntityGenerator(ObjectManager* om, CollisionSystem* cs, DrawSystem* ds,
								 EnemySystem* es, InputSystem* is, MovementSystem* ms, TextSystem* ts, LightSystem* ls, GameStateCmp* gs)
{
	objectManager = om;
	collisionSystem = cs;
	drawSystem = ds;
	enemySystem = es;
	inputSystem = is;
	movementSystem = ms;
	textSystem = ts;
	lightSystem = ls;
	gameState = gs;
}

// Parse .json file to generate entities
void EntityGenerator::generateEntities(std::string room_path, Water* water)
{
	// Components
	DrawCmp drawCmp;
	TransformCmp transformCmp;
	InputCmp inputCmp;
	CollisionCmp collisionCmp;
	EnemyCmp enemyCmp;
	MovementCmp movementCmp;
	ItemCmp itemCmp;

	// Generate main player
	// Main player MUST be registered first to match the SAM_GUID constant declared in Component.hpp
	Entity* playerEntity = objectManager->makeEntity("Player");


	// Read JSON map file
	std::ifstream data(room_path);
	json map = json::parse(data);
	json layers = map["layers"];


	for (json::reverse_iterator layer_it = layers.rbegin(); layer_it != layers.rend(); ++layer_it)
	{
		json tiles = (*layer_it)["data"];

		float y = TILE_HEIGHT / 2;
		float x = TILE_WIDTH / 2;

		// Go through all tiles in this layer
		for (json::iterator tile_it = tiles.begin(); tile_it != tiles.end(); ++tile_it)
		{
			if (x > SCREEN_WIDTH)
			{
				x = TILE_WIDTH / 2;
				y += TILE_HEIGHT;
			}

			// Read tile value
			int val = (*tile_it).get<int>();

			Entity* entity;

			switch (val) {
				case TORCH: {
					entity = objectManager->makeEntity("Torch");

					movementCmp.add(entity, 400.f, -0.5f);
					transformCmp.add(entity, {x, y}, {2.5f, 2.0f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/torch.png"));
					collisionCmp.add(entity);

                    itemCmp.add(entity);
					break;
				}
				case TOP_LEFT_CORNER: {

					entity = objectManager->makeEntity("Wall");

					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/top_left_corner.png"));
					collisionCmp.add(entity);
					break;
				}
				case DOOR_TL: {
					entity = objectManager->makeEntity("Special_Door");

					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/DOOR_TL.png"));
					collisionCmp.add(entity);
					break;
				}
				case DOOR_TR: {
					entity = objectManager->makeEntity("Special_Door");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/DOOR_TR.png"));
					collisionCmp.add(entity);
					break;
				}
				case DOOR_ML: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/DOOR_ML.png"));
					collisionCmp.add(entity);
					break;
				}
				case DOOR_MR: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/DOOR_MR.png"));
					collisionCmp.add(entity);
					break;
				}
				case DOOR_BL: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/DOOR_BL.png"));
					collisionCmp.add(entity);
					break;
				}
				case DOOR_BR: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/DOOR_BR.png"));
					collisionCmp.add(entity);
					break;
				}
				case OPEN_DOOR_TL: {
					entity = objectManager->makeEntity("Open_Door");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/OPEN_DOOR_TL.png"));
					collisionCmp.add(entity);
					break;
				}
				case OPEN_DOOR_TR: {
					entity = objectManager->makeEntity("Open_Door");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/OPEN_DOOR_TR.png"));
					collisionCmp.add(entity);
					break;
				}
				case OPEN_DOOR_BL: {
					entity = objectManager->makeEntity("Open_Door");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/OPEN_DOOR_BL.png"));
					collisionCmp.add(entity);
					break;
				}
				case OPEN_DOOR_BR: {
					entity = objectManager->makeEntity("Open_Door");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/OPEN_DOOR_BR.png"));
					collisionCmp.add(entity);
					break;
				}
				case TOP_RIGHT_CORNER: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/top_right_corner.png"));
					collisionCmp.add(entity);
					break;
				}
				case BOTTOM_LEFT_CORNER: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/bottom_left_corner.png"));
					collisionCmp.add(entity);
					break;
				}
				case BOTTOM_RIGHT_CORNER: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/bottom_right_corner.png"));
					collisionCmp.add(entity);
					break;
				}
				case TOP_WALL: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/top_wall.png"));
					collisionCmp.add(entity);
					break;
				}
				case BOTTOM_WALL: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/bottom_wall.png"));
					collisionCmp.add(entity);
					break;
				}
				case LEFT_WALL: {
					entity = objectManager->makeEntity("Wall");

					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/left_wall.png"));
					collisionCmp.add(entity);
					break;
				}
				case RIGHT_WALL: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/right_wall.png"));
					collisionCmp.add(entity);
					break;
				}
				case BLACK_TILE: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/black_tile.png"));
					collisionCmp.add(entity);
					break;
				}
				case FLOOR_TILE: {
					entity = objectManager->makeEntity("Floor_Tile");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/floor_tile.png"));
					collisionCmp.add(entity);
					break;
				}
				case UNLIT_CAULDRON: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/UNLIT_CAULDRON.png"));
					collisionCmp.add(entity);
					break;
				}
				case LIT_CAULDRON: {
					entity = objectManager->makeEntity("Cauldron");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/LIT_CAULDRON.png"));
					collisionCmp.add(entity);
					entity->active = false;
					break;
				}
				case PILLAR_TOP: {
					entity = objectManager->makeEntity("PILLAR_TOP");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/PILLAR_TOP.png"));
					collisionCmp.add(entity);
					break;
				}
				case PILLAR_MID: {
					entity = objectManager->makeEntity("PILLAR_MID");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/PILLAR_MID.png"));
					collisionCmp.add(entity);
					break;
				}
				case PILLAR_BOT: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/PILLAR_BOT.png"));
					collisionCmp.add(entity);
					break;
				}
				case CLOSET: {
					entity = objectManager->makeEntity("Closet");

					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/chest_closed.png"));
					collisionCmp.add(entity);

					// Make interactable areas around the closet
					entity = objectManager->makeEntity("ClosetArea");
					transformCmp.add(entity, {x + (TILE_WIDTH / 2), y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/interactable_area.png"));
					collisionCmp.add(entity);

					entity = objectManager->makeEntity("ClosetArea");
					transformCmp.add(entity, {x - (TILE_WIDTH / 2), y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/interactable_area.png"));
					collisionCmp.add(entity);

					entity = objectManager->makeEntity("ClosetArea");
					transformCmp.add(entity, {x, y + (TILE_HEIGHT / 2)}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/interactable_area.png"));
					collisionCmp.add(entity);

					entity = objectManager->makeEntity("ClosetArea");
					transformCmp.add(entity, {x, y - (TILE_HEIGHT / 2)}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/interactable_area.png"));
					collisionCmp.add(entity);
					break;
				}
				case BIG_TABLE_TL: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/BIG_TABLE_TL.png"));
					collisionCmp.add(entity);
					break;
				}
				case BIG_TABLE_TR: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/BIG_TABLE_TR.png"));
					collisionCmp.add(entity);
					break;
				}
				case BIG_TABLE_TOP_EDGE: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/BIG_TABLE_TOP_EDGE.png"));
					collisionCmp.add(entity);
					break;
				}
				case BIG_TABLE_LEFT_EDGE: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/BIG_TABLE_LEFT_EDGE.png"));
					collisionCmp.add(entity);
					break;
				}
				case BIG_TABLE_RIGHT_EDGE: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/BIG_TABLE_RIGHT_EDGE.png"));
					collisionCmp.add(entity);
					break;
				}
				case BIG_TABLE_BL: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/BIG_TABLE_BL.png"));
					collisionCmp.add(entity);
					break;
				}
				case BIG_TABLE_BR: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/BIG_TABLE_BR.png"));
					collisionCmp.add(entity);
					break;
				}
				case BIG_TABLE_BOT_EDGE: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/BIG_TABLE_BOT_EDGE.png"));
					collisionCmp.add(entity);
					break;
				}
				case BIG_TABLE_LEFT_LEG: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/BIG_TABLE_LEFT_LEG.png"));
					collisionCmp.add(entity);
					break;
				}
				case BIG_TABLE_RIGHT_LEG: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/BIG_TABLE_RIGHT_LEG.png"));
					collisionCmp.add(entity);
					break;
				}
				case BIG_TABLE_SHADOW: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/BIG_TABLE_SHADOW.png"));
					collisionCmp.add(entity);
					break;
				}
				case BIG_TABLE_FILL: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/BIG_TABLE_FILL.png"));
					collisionCmp.add(entity);
					break;
				}
				case SMALL_TABLE_BL: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/SMALL_TABLE_BL.png"));
					collisionCmp.add(entity);
					break;
				}
				case SMALL_TABLE_BR: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/SMALL_TABLE_BR.png"));
					collisionCmp.add(entity);
					break;
				}
				case SMALL_TABLE_TL: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/SMALL_TABLE_TL.png"));
					collisionCmp.add(entity);
					break;
				}
				case SMALL_TABLE_TR: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/SMALL_TABLE_TR.png"));
					collisionCmp.add(entity);
					break;
				}
				case CHAIR_TL: {
					entity = objectManager->makeEntity("Chair");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/CHAIR_TL.png"));
					collisionCmp.add(entity);
					break;
				}
				case CHAIR_TR: {
					entity = objectManager->makeEntity("Chair");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/CHAIR_TR.png"));
					collisionCmp.add(entity);
					break;
				}
				case CHAIR_ML: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/CHAIR_ML.png"));
					collisionCmp.add(entity);
					break;
				}
				case CHAIR_MR: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/CHAIR_MR.png"));
					collisionCmp.add(entity);
					break;
				}
				case CHAIR_BL: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/CHAIR_BL.png"));
					collisionCmp.add(entity);
					break;
				}
				case CHAIR_BR: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/CHAIR_BR.png"));
					collisionCmp.add(entity);
					break;
				}
				case LEFT_CHAIR_TL: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/LEFT_CHAIR_TL.png"));
					collisionCmp.add(entity);
					break;
				}
				case LEFT_CHAIR_TR: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/LEFT_CHAIR_TR.png"));
					collisionCmp.add(entity);
					break;
				}
				case LEFT_CHAIR_BR: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/LEFT_CHAIR_BR.png"));
					collisionCmp.add(entity);
					break;
				}
				case LEFT_CHAIR_BL: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/LEFT_CHAIR_BL.png"));
					collisionCmp.add(entity);
					break;
				}
				case RIGHT_CHAIR_TR: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/RIGHT_CHAIR_TR.png"));
					collisionCmp.add(entity);
					break;
				}
				case RIGHT_CHAIR_TL: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/RIGHT_CHAIR_TL.png"));
					collisionCmp.add(entity);
					break;
				}
				case RIGHT_CHAIR_BR: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/RIGHT_CHAIR_BR.png"));
					collisionCmp.add(entity);
					break;
				}
				case RIGHT_CHAIR_BL: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/RIGHT_CHAIR_BL.png"));
					collisionCmp.add(entity);
					break;
				}
				case DISH_CAB_TL: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/DISH_CAB_TL.png"));
					collisionCmp.add(entity);
					break;
				}
				case DISH_CAB_TR: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/DISH_CAB_TR.png"));
					collisionCmp.add(entity);
					break;
				}
				case DISH_CAB_BL: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/DISH_CAB_BL.png"));
					collisionCmp.add(entity);
					break;
				}
				case DISH_CAB_BR: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/DISH_CAB_BR.png"));
					collisionCmp.add(entity);
					break;
				}
				case MAT_BL: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/MAT_BL.png"));
					collisionCmp.add(entity);
					break;
				}
				case MAT_TL: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/MAT_TL.png"));
					collisionCmp.add(entity);
					break;
				}
				case MAT_BR: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/MAT_BR.png"));
					collisionCmp.add(entity);
					break;
				}
				case MAT_TR: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/MAT_TR.png"));
					collisionCmp.add(entity);
					break;
				}
				case CAB_TL: {
					entity = objectManager->makeEntity("Closet");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/CAB_TL.png"));
					collisionCmp.add(entity);
					break;
				}
				case CAB_TR: {
					entity = objectManager->makeEntity("Closet");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/CAB_TR.png"));
					collisionCmp.add(entity);
					break;
				}
				case CAB_BL: {
					entity = objectManager->makeEntity("Closet");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/CAB_BL.png"));
					collisionCmp.add(entity);
					break;
				}
				case CAB_BR: {
					entity = objectManager->makeEntity("Closet");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/CAB_BR.png"));
					collisionCmp.add(entity);
					break;
				}
				case SIDE_CAB_T: {
					entity = objectManager->makeEntity("Closet");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/SIDE_CAB_T.png"));
					collisionCmp.add(entity);
					break;
				}
				case SIDE_CAB_M: {
					entity = objectManager->makeEntity("Closet");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/SIDE_CAB_M.png"));
					collisionCmp.add(entity);
					break;
				}
				case SIDE_CAB_ML: {
					entity = objectManager->makeEntity("Closet");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/SIDE_CAB_ML.png"));
					collisionCmp.add(entity);
					break;
				}
				case SIDE_CAB_L: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/SIDE_CAB_L.png"));
					collisionCmp.add(entity);
					break;
				}
				case LIGHT_LEFT: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/LIGHT_LEFT.png"));
					collisionCmp.add(entity);
					break;
				}
				case LIGHT_RIGHT: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/LIGHT_RIGHT.png"));
					collisionCmp.add(entity);
					break;
				}
				case CLOCK_TL: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/CLOCK_TL.png"));
					collisionCmp.add(entity);
					break;
				}
				case CLOCK_TR: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/CLOCK_TR.png"));
					collisionCmp.add(entity);
					break;
				}
				case CLOCK_BL: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/CLOCK_BL.png"));
					collisionCmp.add(entity);
					break;
				}
				case CLOCK_BR: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/CLOCK_BR.png"));
					collisionCmp.add(entity);
					break;
				}
				case LEFT_COUNTER_BL: {
					entity = objectManager->makeEntity("Counter");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/LEFT_COUNTER_BL.png"));
					collisionCmp.add(entity);
					break;
				}
				case LEFT_COUNTER_BML: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/LEFT_COUNTER_BML.png"));
					collisionCmp.add(entity);
					break;
				}
				case LEFT_COUNTER_BMR: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/LEFT_COUNTER_BMR.png"));
					collisionCmp.add(entity);
					break;
				}
				case LEFT_COUNTER_BR: {
					entity = objectManager->makeEntity("Counter");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/LEFT_COUNTER_BR.png"));
					collisionCmp.add(entity);
					break;
				}
				case LEFT_COUNTER_LEFT_CORNER: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/LEFT_COUNTER_LEFT_CORNER.png"));
					collisionCmp.add(entity);
					break;
				}
				case LEFT_COUNTER_ML: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/LEFT_COUNTER_MID_LEFT.png"));
					collisionCmp.add(entity);
					break;
				}
				case LEFT_COUNTER_MR: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/LEFT_COUNTER_MID_RIGHT.png"));
					collisionCmp.add(entity);
					break;
				}
				case LEFT_COUNTER_RIGHT_CORNER: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/LEFT_COUNTER_RIGHT_CORNER.png"));
					collisionCmp.add(entity);
					break;
				}
				case LEFT_COUNTER_TL: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/LEFT_COUNTER_TOP_LEFT.png"));
					collisionCmp.add(entity);
					break;
				}
				case LEFT_COUNTER_TR: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/LEFT_COUNTER_TOP_RIGHT.png"));
					collisionCmp.add(entity);
					break;
				}
				case RIGHT_COUNTER_BL: {
					entity = objectManager->makeEntity("Counter");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/RIGHT_COUNTER_BL.png"));
					collisionCmp.add(entity);
					break;
				}
				case RIGHT_COUNTER_BML: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/RIGHT_COUNTER_BML.png"));
					collisionCmp.add(entity);
					break;
				}
				case RIGHT_COUNTER_BMR: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/RIGHT_COUNTER_BMR.png"));
					collisionCmp.add(entity);
					break;
				}
				case RIGHT_COUNTER_BR: {
					entity = objectManager->makeEntity("Counter");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/RIGHT_COUNTER_BR.png"));
					collisionCmp.add(entity);
					break;
				}
				case RIGHT_COUNTER_LEFT_CORNER: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/RIGHT_COUNTER_LEFT_CORNER.png"));
					collisionCmp.add(entity);
					break;
				}
				case RIGHT_COUNTER_ML: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/RIGHT_COUNTER_ML.png"));
					collisionCmp.add(entity);
					break;
				}
				case RIGHT_COUNTER_MR: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/RIGHT_COUNTER_MR.png"));
					collisionCmp.add(entity);
					break;
				}
				case RIGHT_COUNTER_RIGHT_CORNER: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/RIGHT_COUNTER_RIGHT_CORNER.png"));
					collisionCmp.add(entity);
					break;
				}
				case RIGHT_COUNTER_TL: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/RIGHT_COUNTER_TL.png"));
					collisionCmp.add(entity);
					break;
				}
				case RIGHT_COUNTER_TR: {
					entity = objectManager->makeEntity("Wall");


					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/RIGHT_COUNTER_TR.png"));
					collisionCmp.add(entity);
					break;
				}
				case KEY: {
					entity = objectManager->makeEntity("Key");


					if (gameState->current_room == ROOM_TWO_GUID && !gameState->level_two_key)
					{
						std::cout << "Level 2 key is still active" << std::endl;
						transformCmp.add(entity, { x, y }, { 3.125f, 3.125f }, 0.0);
						drawCmp.add(entity, textures_path("Dungeon/KEY.png"));
						collisionCmp.add(entity);
					}
					else if (gameState->current_room == ROOM_THREE_GUID && !gameState->level_three_key)
					{
						std::cout << "Level 3 key is still active" << std::endl;
						transformCmp.add(entity, { x, y }, { 3.125f, 3.125f }, 0.0);
						drawCmp.add(entity, textures_path("Dungeon/KEY.png"));
						collisionCmp.add(entity);
					}
					break;
				}
				case DOOR_ROOM_1_TO_2: {
					entity = objectManager->makeEntity("DoorRoom1To2");


					transformCmp.add(entity, {x, y}, {1.5625f, 1.5625f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/door.png"));
					collisionCmp.add(entity);
					break;
				}
				case DOOR_ROOM_2_TO_1: {
					entity = objectManager->makeEntity("DoorRoom2To1");


					transformCmp.add(entity, {x, y}, {1.5625f, 1.5625f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/door.png"));
					collisionCmp.add(entity);
					break;
				}
				case DOOR_ROOM_2_TO_3: {
					entity = objectManager->makeEntity("DoorRoom2To3");


					transformCmp.add(entity, {x, y}, {1.5625f, 1.5625f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/door.png"));
					collisionCmp.add(entity);
					break;
				}
				case DOOR_ROOM_3_TO_2: {
					entity = objectManager->makeEntity("DoorRoom3To2");


					transformCmp.add(entity, {x, y}, {1.5625f, 1.5625f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/door.png"));
					collisionCmp.add(entity);
					break;
				}
				case DOOR_ROOM_1_TO_4: {
					entity = objectManager->makeEntity("DoorRoom1To4");


					transformCmp.add(entity, {x, y}, {1.5625f, 1.5625f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/door.png"));
					collisionCmp.add(entity);
					break;
				}
				case ENEMY: {
					entity = objectManager->makeEntity(ENEMY_LABEL);

					movementCmp.add(entity, 50.f, 0);
					transformCmp.add(entity, {x, y}, {3.125f, 3.125f}, 0.0);
					drawCmp.add(entity, textures_path("Dungeon/ghost.png"));
					collisionCmp.add(entity);
					enemyCmp.add(entity, 100, 0);
					break;
				}
				case NULL_TILE: {
					break;
				}
				default:
					printf("no match found for %d\n", val);
			}

			x += TILE_WIDTH;
		}
	}

	// Add Player entity to components
	// Spawn Sam depending on previous room and current room, or depending on save file loaded
	// from game state
	if (gameState->is_game_loading) {
		transformCmp.add(playerEntity, gameState->sam_position, {2.5f, 2.0f}, 0.0);
		if (gameState->hidden) {
			transformCmp.getTransform(playerEntity)->visible = false;
		}
		gameState->is_game_loading = false;
	}
	else if (gameState->previous_room == "")
	{
		transformCmp.add(playerEntity, SPAWN_ROOM_ONE_START, {2.5f, 2.0f}, 0.0);
	}
	else if (gameState->previous_room == ROOM_ONE_GUID)
	{
		if (gameState->current_room == ROOM_TWO_GUID)
		{
			transformCmp.add(playerEntity, SPAWN_ROOM_ONE_TO_TWO, {2.5f, 2.0f}, 0.0);
		}
		else if (gameState->current_room == ROOM_FOUR_GUID)
		{
			transformCmp.add(playerEntity, SPAWN_ROOM_ONE_TO_FOUR, {2.5f, 2.0f}, 0.0);
		}
	}
	else if (gameState->previous_room == ROOM_TWO_GUID)
	{
		if (gameState->current_room == ROOM_ONE_GUID)
		{
			transformCmp.add(playerEntity, SPAWN_ROOM_TWO_TO_ONE, {2.5f, 2.0f}, 0.0);
		}
		else if (gameState->current_room == ROOM_THREE_GUID)
		{
			transformCmp.add(playerEntity, SPAWN_ROOM_TWO_TO_THREE, {2.5f, 2.0f}, 0.0);
		}
	}
	else if (gameState->previous_room == ROOM_THREE_GUID)
	{
		if (gameState->current_room == ROOM_TWO_GUID)
		{
			transformCmp.add(playerEntity, SPAWN_ROOM_THREE_TO_TWO, {2.5f, 2.0f}, 0.0);
		}
	}
	else if (gameState->previous_room == ROOM_FOUR_GUID)
	{
		if (gameState->current_room == ROOM_ONE_GUID)
		{
			transformCmp.add(playerEntity, SPAWN_ROOM_FOUR_TO_ONE, {2.5f, 2.0f}, 0.0);
		}
	}
	drawCmp.addFull(playerEntity, textures_path("Dungeon/sam.png"), textures_path("Dungeon/sam_back.png"),
	textures_path("Dungeon/sam_front.png"), textures_path("Dungeon/sam_backstep1.png"),
	textures_path("Dungeon/sam_backstep2.png"), textures_path("Dungeon/sam_frontstep1.png"),
	textures_path("Dungeon/sam_frontstep2.png"), textures_path("Dungeon/sam_step1.png")
	, textures_path("Dungeon/sam_step2.png"));
	inputCmp.add(playerEntity);
	movementCmp.add(playerEntity, 200.f, 0);
	collisionCmp.add(playerEntity);


	if (gameState->current_room == ROOM_FOUR_GUID)
	{
		Entity* entity = objectManager->makeEntity(BOSS_GUID);

		movementCmp.add(entity, 50.f, 0);
		transformCmp.add(entity, {400.f, 400.f}, {3.125f, 3.125f}, 0.0);
		drawCmp.add(entity, textures_path("Dungeon/boss.png"));
		collisionCmp.add(entity);
		enemyCmp.add(entity, 100, 0);
		enemyCmp.getmap()[entity->id]->type = BOSS_ENEMY_TYPE;
	}
	// Proceed to handle the text box entities
	generateTextBoxEntities(room_path, drawCmp, transformCmp, inputCmp, collisionCmp, enemyCmp, movementCmp, itemCmp, water);
}

// Separate call to generate text box entitities
void EntityGenerator::generateTextBoxEntities(std::string room_path, DrawCmp dc, TransformCmp tc, InputCmp ic, CollisionCmp cc, EnemyCmp ec, MovementCmp mc, ItemCmp itc, Water* water)
{
	// Create text boxes if we're in room one:
	if (map_path("level_one.json") == room_path)
	{
		// Text boxes
		Entity* useWASD = objectManager->makeEntity(USE_WASD_TEXT_LABEL);
		dc.add(useWASD, textures_path("text/usewasd.png"));
		tc.add(useWASD, TEXT_POSITION, { 0.2, 0.2 }, 0.0);

		Entity* useEText = objectManager->makeEntity(USE_E_INTERACT_LABEL);
		dc.add(useEText, textures_path("text/etointeract.png"));
		tc.add(useEText, TEXT_POSITION, { 0.2, 0.2 }, 0.0);
		useEText->active = false; // E text initially invisible
	}

	// Text box if you're dead
	Entity* rToRestart = objectManager->makeEntity(USE_R_RESTART);
	dc.add(rToRestart, textures_path("text/ptorestart.png"));
	tc.add(rToRestart, TEXT_POSITION, { 0.2, 0.2 }, 0.0);
	rToRestart->active = false; // Died text initially invisible

	// Proceed to handle held item, if applicable
	handleHeldItem(dc, tc, ic, cc, ec, mc, itc, water);
}

// When room changes, Sam's held item is deleted but we still want to generate
// it in the new room
void EntityGenerator::handleHeldItem(DrawCmp dc, TransformCmp tc, InputCmp ic, CollisionCmp cc, EnemyCmp ec, MovementCmp mc, ItemCmp itc, Water* water)
{
	// Check GameStateCmp to see if any held entities from other rooms should be generated in this room
	if (gameState->held_item != -1) {
		if (gameState->held_item == TORCH) {
			Entity* entity = objectManager->makeEntity("Torch");

			entity->active = false; // Sam is holding it
			mc.add(entity, 400.f, -0.5f);
			tc.add(entity, { 0, 0 }, { 2.5f, 2.0f }, 0.0);
			dc.add(entity, textures_path("Dungeon/torch.png"));
			cc.add(entity);
			itc.add(entity);
			gameState->held_entity = entity;
		}
	}

	// Done generating entities, proceed to initialize systems
	initializeSystems(dc, tc, ic, cc, ec, mc, itc, water);
}

// Set-up DrawSystem, InputSystem, CollisionSystem, etc.
void EntityGenerator::initializeSystems(DrawCmp dc, TransformCmp tc, InputCmp ic, CollisionCmp cc, EnemyCmp ec, MovementCmp mc, ItemCmp itc, Water* water)
{
	drawSystem->init(*objectManager, dc, tc, mc, gameState);
	inputSystem->init(*objectManager, ic, tc, cc, mc, ec, itc, gameState);
	collisionSystem->init(*objectManager, cc, tc, itc, gameState);
	enemySystem->init(*objectManager, tc, ec, mc, itc, gameState);
	movementSystem->init(*objectManager, tc, cc, mc, gameState);
	textSystem->init(*objectManager, gameState, water);
	lightSystem->init(*objectManager, gameState, tc, water);

	drawSystem->setup();
}
