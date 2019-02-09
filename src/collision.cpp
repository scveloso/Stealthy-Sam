#include "collision.hpp"

Collision::Collision(vec2 scale, Texture texture)
{

	hw = (scale.x) * (texture.width / 2);
	hh = (scale.y) * (texture.height / 2);
}

bool Collision::collides_with_wall(vec2 new_position, const Wall& wall)
{

	// Grab object's edges
	// (Note: we don't want the edges of the texture but rather the edges of the actual sprite)
	float object_x1 = (new_position.x - (hw * 0.25));
	float object_x2 = (new_position.x + (hw * 0.25));
	float object_y1 = new_position.y - (hh * 0.75);
	float object_y2 = new_position.y + (hh * 0.5);

	// Grab wall's edges
	float wall_x1 = wall.get_left_edge();
	float wall_x2 = wall.get_right_edge();
	float wall_y1 = wall.get_top_edge();
	float wall_y2 = wall.get_bottom_edge();

	// Collision case 1: top right corner will be inside the wall
	if (object_x2 >= wall_x1 && object_x2 <= wall_x2 &&
		object_y1 >= wall_y1 && object_y1 <= wall_y2)
	{
		return true;
	}

	// Collision case 2: top left corner will be inside the wall
	if (object_x1 >= wall_x1 && object_x1 <= wall_x2 &&
		object_y1 >= wall_y1 && object_y1 <= wall_y2)
	{
		return true;
	}

	// Collision case 3: bottom right corner will be inside the wall
	if (object_x2 >= wall_x1 && object_x2 <= wall_x2 &&
		object_y2 >= wall_y1 && object_y2 <= wall_y2)
	{
		return true;
	}

	// Collision case 4: bottom left corner will be inside the wall
	if (object_x1 >= wall_x1 && object_x1 <= wall_x2 &&
		object_y2 >= wall_y1 && object_y2 <= wall_y2)
	{
		return true;
	}

	// Collision case 5: prevent fat object from going through thin wall from the bottom
	if (object_x1 <= wall_x1 && object_x2 >= wall_x2 &&
		object_y1 <= wall_y2 && object_y2 >= wall_y2)
	{
		return true;
	}

	// Collision case 6: prevent fat object from going through thin wall from the top
	if (object_x1 <= wall_x1 && object_x2 >= wall_x2 &&
		object_y2 >= wall_y1 && object_y1 <= wall_y1)
	{
		return true;
	}

	// Collision case 7: prevent tall object from going through short wall from the left
	if (object_y1 <= wall_y1 && object_y2 >= wall_y2 &&
		object_x2 >= wall_x1 && object_x1 <= wall_x1)
	{
		return true;
	}

	// Collision case 8: prevent tall object from going through short wall from the right
	if (object_y1 <= wall_y1 && object_y2 >= wall_y2 &&
		object_x1 <= wall_x2 && object_x2 >= wall_x2)
	{
		return true;
	}

	return false;
}
bool Collision::collides_with_screen_edge(vec2 new_position, vec2 screen)
{

	// Grab object's edges
	// (Note: we don't want the edges of the texture but rather the edges of the actual sprite)
	float object_x1 = (new_position.x - (hw * 0.6));
	float object_x2 = (new_position.x + (hw * 0.6));
	float object_y1 = new_position.y - (hh * 1.2);
	float object_y2 = new_position.y + (hh);

	return (object_x2 >= screen.x || object_x1 <= 0 || object_y1 <= 0 || object_y2 >= screen.y);
}
bool Collision::is_movement_interrupted(vec2 new_position, std::vector<Wall> m_walls, vec2 screen)
{
	auto wall_it = m_walls.begin();
	while (wall_it != m_walls.end())
	{
		if (collides_with_wall(new_position, *wall_it) ||
			collides_with_screen_edge(new_position, screen))
		{
			return true;
		}
		else
			++wall_it;
	}
	return false;
}
