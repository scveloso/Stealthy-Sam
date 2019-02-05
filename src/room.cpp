#include "room.hpp"
#include "constants.hpp"
#include <iostream>

// Draw all room entities
// http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/
void Room::draw(const mat3& projection)
{
  // Drawing walls
	for (auto& wall : m_walls)
			wall.draw(projection);

	// Draw interactables
	for (auto& interactable : m_closets)
		interactable.draw(projection);

	// Draw doors
	for (auto& door : m_doors)
		door.draw(projection);

	// Drawing Sam
	m_sam->draw(projection);

	// Drawing enemies
	for (auto& enemy : m_enemies) {
		enemy.draw(projection);
	}
}

// Spawn an enemy at the given coordinates with its patrol
bool Room::spawn_enemy(float posx, float posy, float patrol_x, float patrol_y)
{
	Enemy enemy;
	if (enemy.init())
	{
		enemy.set_position({ posx, posy });
		enemy.set_patrol_length_x(patrol_x);
		enemy.set_patrol_length_y(patrol_y);
		m_enemies.emplace_back(enemy);
		return true;
	}
	//fprintf(stderr, "Failed to spawn enemy");
	return false;
}

// On key event
void Room::on_key(int key, int action, int mod)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_A:
			m_sam->direction *= LEFT;
			break;
		case GLFW_KEY_D:
			m_sam->direction *= RIGHT;
			break;
		case GLFW_KEY_S:
			m_sam->direction *= DOWN;
			break;
		case GLFW_KEY_W:
			m_sam->direction *= UP;
			break;
		case GLFW_KEY_E:
			m_sam->interact_in_front(m_closets);
			break;
		default:
			break;
		}
	}

	if (action == GLFW_RELEASE && m_sam->direction != NO_DIRECTION)
	{
		switch (key)
		{
		case GLFW_KEY_A:
			m_sam->direction /= LEFT;
			break;
		case GLFW_KEY_D:
			m_sam->direction /= RIGHT;
			break;
		case GLFW_KEY_S:
			m_sam->direction /= DOWN;
			break;
		case GLFW_KEY_W:
			m_sam->direction /= UP;
			break;

		default:
			break;
		}
	}

	// Resetting game
	if (action == GLFW_RELEASE && key == GLFW_KEY_R)
	{
		m_sam->destroy();
		m_sam->init();
	}
}
