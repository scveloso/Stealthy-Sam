#include <iostream>
#include "EntityGenerator.hpp"
#include "TileConstants.hpp"
#include <string>

using json = nlohmann::json;

void EntityGenerator::generateEntities(ObjectManager* objectManager, DrawCmp drawCmp, TransformCmp transformCmp,
InputCmp inputCmp, CollisionCmp cc, EnemyCmp ec, std::string room_path, Water m_water)
{
	// Generate main player
	// Main player MUST be registered first to match the SAM_GUID constant declared in Component.hpp
	Entity* playerEntity = objectManager->makeEntity("Player");

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
				cc.add(entity);
				vec2 s_position = transformCmp.getTransform(entity)->m_position;
				m_water.add_position(s_position);

			}
			else if (val == WALL)
			{
				entity = objectManager->makeEntity("Wall");

				transformCmp.add(entity, { x, y }, { 3.125f, 3.125f }, 0.0);
				drawCmp.add(entity, textures_path("Dungeon/wall_mid.png"));
				cc.add(entity);
			}
			else if (val == KEY)
            {
                entity = objectManager->makeEntity("Key");

                transformCmp.add(entity, { x, y }, { 3.125f, 3.125f }, 0.0);
                drawCmp.add(entity, textures_path("Dungeon/key.png"));
                cc.add(entity);
            }
			else if (val == CLOSET)
			{
				entity = objectManager->makeEntity("Closet");

				transformCmp.add(entity, { x, y }, { 3.125f, 3.125f }, 0.0);
				drawCmp.add(entity, textures_path("Dungeon/chest_closed.png"));
				cc.add(entity);

				// Make interactable areas around the closet
				entity = objectManager->makeEntity("ClosetArea");
				transformCmp.add(entity, { x + (TILE_WIDTH / 2), y }, { 3.125f, 3.125f }, 0.0);
				drawCmp.add(entity, textures_path("Dungeon/interactable_area.png"));
				cc.add(entity);

				entity = objectManager->makeEntity("ClosetArea");
				transformCmp.add(entity, { x - (TILE_WIDTH / 2), y }, { 3.125f, 3.125f }, 0.0);
				drawCmp.add(entity, textures_path("Dungeon/interactable_area.png"));
				cc.add(entity);

				entity = objectManager->makeEntity("ClosetArea");
				transformCmp.add(entity, { x, y + (TILE_HEIGHT / 2) }, { 3.125f, 3.125f }, 0.0);
				drawCmp.add(entity, textures_path("Dungeon/interactable_area.png"));
				cc.add(entity);

				entity = objectManager->makeEntity("ClosetArea");
				transformCmp.add(entity, { x, y - (TILE_HEIGHT / 2) }, { 3.125f, 3.125f }, 0.0);
				drawCmp.add(entity, textures_path("Dungeon/interactable_area.png"));
				cc.add(entity);
			}
			else if (val == DOOR_ROOM_1_TO_2)
			{
				entity = objectManager->makeEntity("DoorRoom1To2");

				transformCmp.add(entity, { x, y }, { 1.5625f, 1.5625f }, 0.0);
				drawCmp.add(entity, textures_path("Dungeon/door.png"));
				cc.add(entity);
			}
			else if (val == DOOR_ROOM_2_TO_1)
			{
				entity = objectManager->makeEntity("DoorRoom2To1");

				transformCmp.add(entity, { x, y }, { 1.5625f, 1.5625f }, 0.0);
				drawCmp.add(entity, textures_path("Dungeon/door.png"));
				cc.add(entity);
			}
			else if (val == DOOR_ROOM_2_TO_3)
			{
				entity = objectManager->makeEntity("DoorRoom2To3");

				transformCmp.add(entity, { x, y }, { 1.5625f, 1.5625f }, 0.0);
				drawCmp.add(entity, textures_path("Dungeon/door.png"));
				cc.add(entity);
			}
			else if (val == DOOR_ROOM_3_TO_2)
			{
				entity = objectManager->makeEntity("DoorRoom3To2");

				transformCmp.add(entity, { x, y }, { 1.5625f, 1.5625f }, 0.0);
				drawCmp.add(entity, textures_path("Dungeon/door.png"));
				cc.add(entity);
			}
			else if (val == ENEMY)
			{
				entity = objectManager->makeEntity("Enemy");

				transformCmp.add(entity, { x, y }, { 3.125f, 3.125f }, 0.0);
				drawCmp.add(entity, textures_path("Dungeon/ghost.png"));
				cc.add(entity);
				ec.add(entity, 100, 0);
			}

			x += TILE_WIDTH;
		}
	}

  std::cout << "Finished generating entities" << std::endl;

}
