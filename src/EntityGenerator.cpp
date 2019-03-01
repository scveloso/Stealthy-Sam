#include <iostream>
#include "EntityGenerator.hpp"
#include "TileConstants.hpp"
#include <string>
#include <vector>

using json = nlohmann::json;

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

void EntityGenerator::generateEntities(std::string room_path, Water* water)
{
	// Components
	DrawCmp drawCmp;
	TransformCmp transformCmp;
	InputCmp inputCmp;
	CollisionCmp collisionCmp;
	EnemyCmp enemyCmp;
	MovementCmp movementCmp;

	// Generate main player
	// Main player MUST be registered first to match the SAM_GUID constant declared in Component.hpp
	Entity* playerEntity = objectManager->makeEntity("Player");

	// Create text boxes if we're in room one:
	if (map_path("room_one.json") == room_path)
	{
		// Text boxes
		Entity* useWASD = objectManager->makeEntity(USE_WASD_TEXT_LABEL);
		drawCmp.add(useWASD, textures_path("text/usewasd.png"));
		transformCmp.add(useWASD, TEXT_POSITION, { 0.2, 0.2 }, 0.0);

		Entity* useEText = objectManager->makeEntity(USE_E_INTERACT_LABEL);
		drawCmp.add(useEText, textures_path("text/etointeract.png"));
		transformCmp.add(useEText, TEXT_POSITION, { 0.2, 0.2 }, 0.0);
		useEText->active = false; // E text initially invisible
	}

	// Text box if you're dead
	Entity* rToRestart = objectManager->makeEntity(USE_R_RESTART);
	drawCmp.add(rToRestart, textures_path("text/rtorestart.png"));
	transformCmp.add(rToRestart, TEXT_POSITION, { 0.2, 0.2 }, 0.0);
	rToRestart->active = false; // Died text initially invisible

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

				transformCmp.add(entity, { x, y }, { 2.5f, 2.0f }, 0.0);
				drawCmp.add(entity, textures_path("Dungeon/sam.png"));
				//drawCmp.add(entity, textures_path("sam/16.png"));
				inputCmp.add(entity);
				movementCmp.add(entity, 200.f, 0);
				collisionCmp.add(entity);
				vec2 s_position = transformCmp.getTransform(entity)->m_position;
				water->add_position(s_position);
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
			else if (val == TORCH)
			{
			  entity = objectManager->makeEntity("Torch");

				movementCmp.add(entity, 200.f, -0.1f);
			  transformCmp.add(entity, { x, y }, { 2.5f, 2.0f }, 0.0);
			  drawCmp.add(entity, textures_path("Dungeon/torch.png"));
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

				movementCmp.add(entity, 50.f, 0);
				transformCmp.add(entity, { x, y }, { 3.125f, 3.125f }, 0.0);
				drawCmp.add(entity, textures_path("Dungeon/ghost.png"));
				collisionCmp.add(entity);
				enemyCmp.add(entity, 100, 0);
			}

			x += TILE_WIDTH;
		}
	}

	initializeSystems(drawCmp, transformCmp, inputCmp, collisionCmp, enemyCmp, movementCmp, water);
}

// Set-up DrawSystem, InputSystem, CollisionSystem
void EntityGenerator::initializeSystems(DrawCmp dc, TransformCmp tc, InputCmp ic, CollisionCmp cc, EnemyCmp ec, MovementCmp mc, Water* water)
{
	drawSystem->init(*objectManager, dc, tc, gameState);
	inputSystem->init(*objectManager, ic, tc, cc, mc, gameState);
	collisionSystem->init(*objectManager, cc, tc, gameState);
	enemySystem->init(*objectManager, cc, tc, ec, mc);
	movementSystem->init(*objectManager, ic, tc, cc, mc, gameState);
	textSystem->init(*objectManager, gameState, water);
	lightSystem->init(*objectManager, gameState, tc, water);

	drawSystem->setup();
}
