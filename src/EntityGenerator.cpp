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
								 EnemySystem* es, InputSystem* is, MovementSystem* ms,
                                 TextSystem* ts, LightSystem* ls, GameStateCmp* gs,
                                 MissileSystem* missileSys)
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
	missileSystem = missileSys;
}

// Parse .json file to generate entities
// void EntityGenerator::generateEntities(std::string room_path, Water* water)
void EntityGenerator::generateEntities(std::string room_path, Light* light, EnemyCone* enemy, Text* text, Effect effect)
{
	// Components

	// Generate main player
	// Main player MUST be registered first to match the SAM_GUID constant declared in Component.hpp
	Entity* playerEntity = objectManager->makeEntity(SAM_GUID);


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
					enemyCmp.add(entity, 100, 0, x, y);
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
		else if (gameState->current_room == TUTORIAL_LEVEL_GUID)
		{
			transformCmp.add(playerEntity, SPAWN_TUTORIAL, {2.5f, 2.0f}, 0.0);
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
	textures_path("Dungeon/sam_frontstep2.png"), textures_path("Dungeon/sam_step1.png"),
	textures_path("Dungeon/sam_step2.png"));
	inputCmp.add(playerEntity);
	movementCmp.add(playerEntity, 200.f, 0);
	collisionCmp.add(playerEntity);


	if (gameState->current_room == ROOM_FOUR_GUID)
	{
		Entity* entity = objectManager->makeEntity(BOSS_GUID);

		movementCmp.add(entity, 50.f, 0);
		transformCmp.add(entity, {400.f, 400.f}, {3.125f, 3.125f}, 0.0);
		// TODO: we should really change this to make it more specific for the boss, but this will do
        drawCmp.addFull(entity, textures_path("Dungeon/boss.png"), textures_path("Dungeon/boss_attack.png"),
                        textures_path("Dungeon/boss_running.png"), textures_path("Dungeon/boss.png"),
                        textures_path("Dungeon/boss.png"), textures_path("Dungeon/boss.png"),
                        textures_path("Dungeon/boss.png"), textures_path("Dungeon/boss.png"),
                        textures_path("Dungeon/boss.png"));
		collisionCmp.add(entity);
		enemyCmp.add(entity, 100, 0, 400.f, 400.f);
		enemyCmp.getmap()[entity->id]->type = BOSS_ENEMY_TYPE;
	}

	// Key counter
	Entity* key_UI = objectManager->makeEntity("key_UI");
	drawCmp.add(key_UI, textures_path("Dungeon/key.png"));
	transformCmp.add(key_UI, {50,50}, {3,3}, 0.0);
	key_UI->ui = true;

	// Key counter
	Entity* key_0_2 = objectManager->makeEntity("key0_UI");
	drawCmp.add(key_0_2, textures_path("text/0_2.png"));
	transformCmp.add(key_0_2, {50,90}, {.7,.7}, 0.0);

	// Key counter
	Entity* key_1_2 = objectManager->makeEntity("key1_UI");
	drawCmp.add(key_1_2, textures_path("text/1_2.png"));
	transformCmp.add(key_1_2, {55,90}, {.7,.7}, 0.0);
	key_1_2->active = false;

	// Key counter
	Entity* key_2_2 = objectManager->makeEntity("key2_UI");
	drawCmp.add(key_2_2, textures_path("text/2_2.png"));
	transformCmp.add(key_2_2, {50,90}, {.7,.7}, 0.0);
	key_2_2->active = false;

	Entity* T1_Welcome = objectManager->makeEntity(T1_WELCOME);
	drawCmp.add(T1_Welcome,textures_path("text/Tutorial/T1_Welcome.png"));
	transformCmp.add(T1_Welcome,{600,700},{.8,.8},0.0);
	T1_Welcome->active = false;
	T1_Welcome->ui = true;
	gameState->textArray[0] = T1_Welcome;

	Entity* T2_Movement = objectManager->makeEntity("T2_Movement");
	drawCmp.add(T2_Movement,textures_path("text/Tutorial/T2_Movement.png"));
	transformCmp.add(T2_Movement,{600,700},{.8,.8},0.0);
	T2_Movement->active = false;
	T2_Movement->ui = true;
	gameState->textArray[1] = T2_Movement;

    Entity* T3_Torch = objectManager->makeEntity("T3_Torch");
    drawCmp.add(T3_Torch,textures_path("text/Tutorial/T3_Torch.png"));
    transformCmp.add(T3_Torch,{600,700},{.8,.8},0.0);
    T3_Torch->active = false;
    T3_Torch->ui = true;
    gameState->textArray[2] = T3_Torch;

    Entity* T4_Throw = objectManager->makeEntity("T4_Throw");
    drawCmp.add(T4_Throw,textures_path("text/Tutorial/T4_Throw.png"));
    transformCmp.add(T4_Throw,{600,700},{.8,.8},0.0);
    T4_Throw->active = false;
    T4_Throw->ui = true;
    gameState->textArray[3] = T4_Throw;

    Entity* T5_Menu = objectManager->makeEntity("T5_Menu");
    drawCmp.add(T5_Menu,textures_path("text/Tutorial/T5_Menu.png"));
    transformCmp.add(T5_Menu,{600,700},{.8,.8},0.0);
    T5_Menu->active = false;
    T5_Menu->ui = true;
    gameState->textArray[4] = T5_Menu;

    Entity* T5_Death = objectManager->makeEntity("T6_Death");
    drawCmp.add(T5_Death,textures_path("text/Tutorial/T6_Death.png"));
    transformCmp.add(T5_Death,{600,700},{.8,.8},0.0);
    T5_Death->active = false;
    T5_Death->ui = true;
    gameState->textArray[5] = T5_Death;

    Entity* T7_Chest = objectManager->makeEntity("T7_Chest");
    drawCmp.add(T7_Chest,textures_path("text/Tutorial/T7_Chest.png"));
    transformCmp.add(T7_Chest,{600,700},{.8,.8},0.0);
    T7_Chest->active = false;
    T7_Chest->ui = true;
    gameState->textArray[5] = T7_Chest;

    Entity* T8_torchTip = objectManager->makeEntity("T8_torchTip");
    drawCmp.add(T8_torchTip,textures_path("text/Tutorial/T8_torchTip.png"));
    transformCmp.add(T8_torchTip,{600,700},{.8,.8},0.0);
    T8_torchTip->active = false;
    T8_torchTip->ui = true;
    gameState->textArray[6] = T8_torchTip;

    Entity* T9_Save = objectManager->makeEntity("T9_Save");
    drawCmp.add(T9_Save,textures_path("text/Tutorial/T9_Save.png"));
    transformCmp.add(T9_Save,{600,700},{.8,.8},0.0);
    T9_Save->active = false;
    T9_Save->ui = true;
    gameState->textArray[7] = T9_Save;

    Entity* T10_Help = objectManager->makeEntity("T10_Help");
    drawCmp.add(T10_Help,textures_path("text/Tutorial/T10_Help.png"));
    transformCmp.add(T10_Help,{600,700},{.8,.8},0.0);
    T10_Help->active = false;
    T10_Help->ui = true;
    gameState->textArray[8] = T10_Help;

    Entity* T11_Startgame = objectManager->makeEntity("T11_Startgame");
    drawCmp.add(T11_Startgame,textures_path("text/Tutorial/T11_Startgame.png"));
    transformCmp.add(T11_Startgame,{600,700},{.8,.8},0.0);
    T11_Startgame->active = false;
    T11_Startgame->ui = true;
    gameState->textArray[9] = T11_Startgame;

    if (gameState->level_two_key && gameState ->level_three_key)
    {
        key_1_2->ui = false;
        key_1_2->active = false;
        key_2_2->ui = true;
        key_2_2->active = true;
    }
    else if (gameState->level_two_key || gameState->level_three_key)
    {
        key_0_2->ui = false;
        key_0_2->active = false;
        key_1_2->ui = true;
        key_1_2->active = true;
    }
    else
    {
        key_0_2->ui = true;
        key_0_2->active = true;
    }

	// Pause Screen
	Entity* pause_screen = objectManager->makeEntity(PAUSE_SCREEN);
	drawCmp.add(pause_screen, textures_path("text/game_paused.png"));
	transformCmp.add(pause_screen, {600,400}, {1,1}, 0.0);
  pause_screen->active = false;
	pause_screen->ui = true;

	// Tutorial/Controls Screen
	Entity* tutorial_screen = objectManager->makeEntity(TUTORIAL_SCREEN);
	drawCmp.add(tutorial_screen, textures_path("text/game_controls.png"));
	transformCmp.add(tutorial_screen, {600,400}, {1,1}, 0.0);
	tutorial_screen->active = false;
	tutorial_screen->ui = true;

	// Main Menu
	Entity* main_menu = objectManager->makeEntity(MAIN_MENU);
	transformCmp.add(main_menu, {600,400}, {1,1}, 0.0);
	std::ifstream save_file(saves_path("save_file.json"));
	if (save_file.fail()) {
		drawCmp.add(main_menu, textures_path("text/game_main_menu_no_save.png"));
	} else {
		drawCmp.add(main_menu, textures_path("text/game_main_menu_with_save.png"));
	}
	main_menu->active = gameState->in_main_menu;
	main_menu->ui = true;

	// "Game Saved" alert
	Entity* game_saved = objectManager->makeEntity(GAME_SAVED_ALERT);
	drawCmp.add(game_saved, textures_path("text/game_saved.png"));
	transformCmp.add(game_saved, {1080,25}, {1,1}, 0.0);
  	game_saved->active = false;
	game_saved->ui = true;

	// "Game Loaded" alert
	Entity* game_loaded = objectManager->makeEntity(GAME_LOADED_ALERT);
	drawCmp.add(game_loaded, textures_path("text/game_loaded.png"));
	transformCmp.add(game_loaded, {1080,25}, {1,1}, 0.0);
	game_loaded->active = gameState->is_game_loading;
	game_loaded->ui = true;

	Entity* game_victory = objectManager->makeEntity(VICTORY_SCREEN);
	drawCmp.add(game_victory,textures_path("text/game_victory.png"));
	transformCmp.add(game_victory,{600,400},{1,1},0.0);
	game_victory->active = false;
	game_victory->ui = true;

	// Game death alert
	Entity* game_death = objectManager->makeEntity(GAME_DEATH_ALERT);
	drawCmp.add(game_death, textures_path("text/game_death.png"));
	transformCmp.add(game_death, {790,25}, {1,1}, 0.0);
  	game_death->active = false;
	game_death->ui = true;



    // Proceed to handle held item, if applicable
	//handleHeldItem(dc, tc, ic, cc, ec, mc, itc, light, enemy, text);


	// Check GameStateCmp to see if any held entities from other rooms should be generated in this room
	if (gameState->held_item != -1) {
		if (gameState->held_item == TORCH) {
			Entity* entity = objectManager->makeEntity("Torch");

			entity->active = false; // Sam is holding it
			movementCmp.add(entity, 400.f, -0.5f);
			transformCmp.add(entity, { 0, 0 }, { 2.5f, 2.0f }, 0.0);
			drawCmp.add(entity, textures_path("Dungeon/torch.png"));
			collisionCmp.add(entity);
			itemCmp.add(entity);
			gameState->held_entity = entity;
		}
	}

	// Loading complete
	gameState->is_game_loading = false;

	// Done generating entities, proceed to initialize systems
	//initializeSystems(drawCmp, tc, ic, cc, ec, mc, itemCmp, light, enemy, text);

	drawSystem->		init(		objectManager, &drawCmp		, &transformCmp	, &movementCmp	, gameState);
	inputSystem->		init(		objectManager, inputCmp		, &transformCmp	, &collisionCmp	, &movementCmp, enemyCmp, itemCmp, gameState);
	collisionSystem->	init(		objectManager, &collisionCmp	, &transformCmp	, itemCmp		, gameState);
	enemySystem->		init(		objectManager, &transformCmp, enemyCmp		, &movementCmp	, itemCmp, gameState);
	movementSystem->	init(		objectManager, &transformCmp, &collisionCmp	, &movementCmp	, gameState);
	textSystem->		init(		objectManager, gameState	, text			, light			, enemy);
	lightSystem->		init(		objectManager, gameState	, &transformCmp	, light			, enemy);
    missileSystem->		init(		objectManager, &collisionCmp, &movementCmp	, gameState		, &drawCmp, &transformCmp);

	drawSystem->setup(effect);
}
