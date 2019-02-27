#include <iostream>
#include "EntityGenerator.hpp"
#include "TileConstants.hpp"
#include <string>

using json = nlohmann::json;

EntityGenerator::EntityGenerator(ObjectManager* om, CollisionSystem* cs, DrawSystem* ds,
	EnemySystem* es, InputSystem* is, MovementSystem* ms, GameStateCmp* gs)
{
	objectManager = om;
	collisionSystem = cs;
	drawSystem = ds;
	enemySystem = es;
	inputSystem = is;
	movementSystem = ms;
	gameState = gs;
}

void EntityGenerator::generateEntities(std::string room_path, Water m_water)
{
	// Components
	DrawCmp drawCmp;
	TransformCmp transformCmp;
	InputCmp inputCmp;
	CollisionCmp collisionCmp;
	EnemyCmp enemyCmp;

	std::cout << "Starting to make entities" << std::endl;

	// Generate main player
	// Main player MUST be registered first to match the SAM_GUID constant declared in Component.hpp
	Entity* playerEntity = objectManager->makeEntity("Player");

	// // Create text boxes if we're in room one:
	// if (map_path("room_one.json") == room_path)
	// {
	// 	// Text boxes
	// 	Entity* useWASD = objectManager->makeEntity(USE_WASD_TEXT_LABEL);
	// 	drawCmp.add(useWASD, textures_path("text/usewasd.png"));
	// 	inputCmp.add(useWASD);
	// 	transformCmp.add(useWASD, { 300, 150 }, { 0.2, 0.2 }, 0.0);
	//
	// 	// Pass coordinates to shader
	// 	vec2 tp = transformCmp.getTransform(useWASD)->m_position;
	// 	m_water.add_text(tp);
	// 	if (useWASD->active) {
	// 		m_water.removeText = 0;
	// 	}
	//
	// 	Entity* useEText = objectManager->makeEntity(USE_E_INTERACT_LABEL);
	// 	drawCmp.add(useEText, textures_path("text/etointeract.png"));
	// 	inputCmp.add(useEText);
	// 	transformCmp.add(useEText, { 300, 150 }, { 0.2, 0.2 }, 0.0);
	// 	// TODO: Reimplement text stuff
	// 	//keyE = useEText;
	//
	// 	// Initially the E text box isn't there until we move
	// 	useEText->active = false;
	// }
	//
	//
	// // Text box if you're dead
	// Entity* rToRestart = objectManager->makeEntity(USE_R_RESTART);
	// drawCmp.add(rToRestart, textures_path("text/rtorestart.png"));
	// transformCmp.add(rToRestart, { 300, 150 }, { 0.2, 0.2 }, 0.0);
	//
	// // Initially the you died textbox isn't there until you're dead
	// rToRestart->active = false;

	std::cout << "Finished text stuff" << std::endl;

	// Read JSON map file
	std::ifstream data(room_path);
	json map = json::parse(data);
	json layers = map["layers"];

	// Go through layers
	for (json::iterator layer_it = layers.begin(); layer_it != layers.end(); ++layer_it)
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

			// Generate main player
			if (val == SAM)
			{
				entity = objectManager->getEntity(SAMS_GUID);

				transformCmp.add(entity, { x, y }, { 1.125f, 1.5f }, 0.0);
				drawCmp.add(entity, textures_path("Dungeon/sam.png"));
				//drawCmp.add(entity, textures_path("sam/16.png"));
				inputCmp.add(entity);
				collisionCmp.add(entity);
				vec2 s_position = transformCmp.getTransform(entity)->m_position;
				m_water.add_position(s_position);

			}
			else if (val == WALL)
			{
				entity = objectManager->makeEntity("Wall");

				transformCmp.add(entity, { x, y }, { 3.125f, 3.125f }, 0.0);
				drawCmp.add(entity, textures_path("Dungeon/wall_mid.png"));
				collisionCmp.add(entity);
			}
			else if (val == KEY)
            {
                entity = objectManager->makeEntity("Key");

                transformCmp.add(entity, { x, y }, { 3.125f, 3.125f }, 0.0);
                drawCmp.add(entity, textures_path("Dungeon/key.png"));
                collisionCmp.add(entity);
            }
			else if (val == CLOSET)
			{
				entity = objectManager->makeEntity("Closet");

				transformCmp.add(entity, { x, y }, { 3.125f, 3.125f }, 0.0);
				drawCmp.add(entity, textures_path("Dungeon/chest_closed.png"));
				collisionCmp.add(entity);

				// Make interactable areas around the closet
				entity = objectManager->makeEntity("ClosetArea");
				transformCmp.add(entity, { x + (TILE_WIDTH / 2), y }, { 3.125f, 3.125f }, 0.0);
				drawCmp.add(entity, textures_path("Dungeon/interactable_area.png"));
				collisionCmp.add(entity);

				entity = objectManager->makeEntity("ClosetArea");
				transformCmp.add(entity, { x - (TILE_WIDTH / 2), y }, { 3.125f, 3.125f }, 0.0);
				drawCmp.add(entity, textures_path("Dungeon/interactable_area.png"));
				collisionCmp.add(entity);

				entity = objectManager->makeEntity("ClosetArea");
				transformCmp.add(entity, { x, y + (TILE_HEIGHT / 2) }, { 3.125f, 3.125f }, 0.0);
				drawCmp.add(entity, textures_path("Dungeon/interactable_area.png"));
				collisionCmp.add(entity);

				entity = objectManager->makeEntity("ClosetArea");
				transformCmp.add(entity, { x, y - (TILE_HEIGHT / 2) }, { 3.125f, 3.125f }, 0.0);
				drawCmp.add(entity, textures_path("Dungeon/interactable_area.png"));
				collisionCmp.add(entity);
			}
			else if (val == DOOR_ROOM_1_TO_2)
			{
				entity = objectManager->makeEntity("DoorRoom1To2");

				transformCmp.add(entity, { x, y }, { 1.5625f, 1.5625f }, 0.0);
				drawCmp.add(entity, textures_path("Dungeon/door.png"));
				collisionCmp.add(entity);
			}
			else if (val == DOOR_ROOM_2_TO_1)
			{
				entity = objectManager->makeEntity("DoorRoom2To1");

				transformCmp.add(entity, { x, y }, { 1.5625f, 1.5625f }, 0.0);
				drawCmp.add(entity, textures_path("Dungeon/door.png"));
				collisionCmp.add(entity);
			}
			else if (val == DOOR_ROOM_2_TO_3)
			{
				entity = objectManager->makeEntity("DoorRoom2To3");

				transformCmp.add(entity, { x, y }, { 1.5625f, 1.5625f }, 0.0);
				drawCmp.add(entity, textures_path("Dungeon/door.png"));
				collisionCmp.add(entity);
			}
			else if (val == DOOR_ROOM_3_TO_2)
			{
				entity = objectManager->makeEntity("DoorRoom3To2");

				transformCmp.add(entity, { x, y }, { 1.5625f, 1.5625f }, 0.0);
				drawCmp.add(entity, textures_path("Dungeon/door.png"));
				collisionCmp.add(entity);
			}
			else if (val == ENEMY)
			{
				entity = objectManager->makeEntity("Enemy");

				transformCmp.add(entity, { x, y }, { 3.125f, 3.125f }, 0.0);
				drawCmp.add(entity, textures_path("Dungeon/ghost.png"));
				collisionCmp.add(entity);
				enemyCmp.add(entity, 100, 0);
			}

			x += TILE_WIDTH;
		}
	}

	std::cout << "Finished making entities" << std::endl;

	initializeSystems(drawCmp, transformCmp, inputCmp, collisionCmp, enemyCmp);
}

// Set-up DrawSystem, InputSystem, CollisionSystem
void EntityGenerator::initializeSystems(DrawCmp dc, TransformCmp tc, InputCmp ic, CollisionCmp cc, EnemyCmp ec)
{
	drawSystem->init(*objectManager, dc, tc, gameState);
	inputSystem->init(*objectManager, ic, tc, cc, gameState);
	collisionSystem->init(*objectManager, cc, tc, gameState);
	enemySystem->init(*objectManager, cc, tc, ec);
	movementSystem->init(*objectManager, ic, tc, cc, gameState);

	drawSystem->setup();
}
