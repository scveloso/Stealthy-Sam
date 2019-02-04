// Header
#include "roomtwo.hpp"
#include "wall_types.h"
#include "constants.hpp"

// stlib
#include <string.h>
#include <cassert>
#include <sstream>
#include <iostream>
#include <math.h>

// Room initialization
// Walls in room two make up an "island" in the middle of the screen
bool RoomTwo::init(vec2 screen) {
	if (m_sam.init())
	{
		Wall wall;
		if (wall.init(WIDE_WALL))
		{
			// Set at the middle of the screen, offset by its 1.5x height upwards (-57.6)
			wall.set_position({ (screen.x / 2),  float((screen.y / 2) - 57.6) });
			m_walls.emplace_back(wall);
		}

    Wall wall2;
		if (wall2.init(WIDE_WALL))
		{
			// Set at the middle of the screen, offset by its half of height upwards (-19.2)
			wall2.set_position({ (screen.x / 2),  float((screen.y / 2) - 19.2) });
			m_walls.emplace_back(wall2);
		}

    Wall wall3;
		if (wall3.init(WIDE_WALL))
		{
			// Set at the middle of the screen, offset by half of its height downwards (+19.2)
			wall3.set_position({ (screen.x / 2),  float((screen.y / 2) + 19.2) });
			m_walls.emplace_back(wall3);
		}

    Wall wall4;
		if (wall4.init(WIDE_WALL))
		{
			// Set at the middle of the screen, offset by its 1.5x height downwards (+57.6)
			wall4.set_position({ (screen.x / 2),  float((screen.y / 2) + 57.6) });
			m_walls.emplace_back(wall4);
		}

		Closet closet1;
		if (closet1.init())
		{
			closet1.set_position({ (screen.x / 2) - 250, (screen.y / 2) - 250 });
			m_closets.emplace_back(closet1);
		}

		return true;
	}
	fprintf(stderr, "Failed to spawn wall(s)\n");
	return false;
}

// Update our game world
bool RoomTwo::update(float elapsed_ms, vec2 screen)
{
  // Updating all entities
	m_sam.update(elapsed_ms, m_walls, screen);

	// Spawn enemies here
	const size_t MAX_ENEMIES = 4;

	// Spawn_enemy(x,y,patrol_length_x, patrol_length_y)
	// You can only choose patrol in one direction
	if (m_enemies.size() < MAX_ENEMIES) {
		spawn_enemy(200.f, 475.f, 500.f, 0.f);
		spawn_enemy(200.f, 575.f, 300.f, 0.f);
		spawn_enemy(200.f, 650.f, 300.f, 0.f);

		spawn_enemy(850.f, 200.f, 0.f, 300.f);
	}

	for (auto& enemy : m_enemies) {
		enemy.update(elapsed_ms);
	}

	return true;
}
