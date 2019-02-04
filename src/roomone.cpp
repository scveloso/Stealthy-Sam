// Header
#include "roomone.hpp"
#include "wall_types.h"
#include "constants.hpp"

// stlib
#include <string.h>
#include <cassert>
#include <sstream>
#include <iostream>
#include <math.h>

// Room initialization
// Walls in room 1 are laid out to have a smaller room, hallways and corners
bool RoomOne::init(vec2 screen) {
	if (m_sam.init())
	{
		Wall wall;
		if (wall.init(TALL_WALL))
		{
			// Set at the middle of the top edge of the screen, accounting for half of this wall's height (192)
			wall.set_position({ (screen.x / 2),  192 });
			m_walls.emplace_back(wall);
		}

		Wall wall2;
		if (wall2.init(WIDE_WALL))
		{
			// Set below the previous wall (384 height) accounting for half of this wall's height (19.2)
			wall2.set_position({ (screen.x / 2),  384 + 19.2 });
			m_walls.emplace_back(wall2);
		}

		Wall wall3;
		if (wall3.init(WIDE_WALL))
		{
			// Set left edge of the screen aligned with wall2
			wall3.set_position({ 100,  384 + 19.2 });
			m_walls.emplace_back(wall3);
		}

		Wall wall4;
		if (wall4.init(WIDE_WALL))
		{
			// Set right edge of the screen aligned with wall2
			wall4.set_position({ (screen.x) - 100,  384 + 19.2 });
			m_walls.emplace_back(wall4);
		}

		Wall wall5;
		if (wall5.init(TALL_WALL))
		{
			// Set at bottom edge of the screen
			wall5.set_position({ (screen.x / 2),  (screen.y) - 50 });
			m_walls.emplace_back(wall5);
		}

		Wall wall6;
		if (wall6.init(WIDE_WALL))
		{
			// Set directly above wall 5, shifted to the right by half of its width
			wall6.set_position({ (float) ((screen.x / 2) + (192 - 19.2)),  (screen.y) - 50 - (192) });
			m_walls.emplace_back(wall6);
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
bool RoomOne::update(float elapsed_ms, vec2 screen)
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
