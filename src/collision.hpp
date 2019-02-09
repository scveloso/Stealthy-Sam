#include "common.hpp"
#include "wall.hpp"

#include <vector>

class Collision 
{
	public:
		Collision(vec2 object_scale, Texture object_Texture);

		bool collides_with_wall(vec2 new_position, const Wall& wall);
		bool collides_with_screen_edge(vec2 new_position, vec2 screen);
		bool is_movement_interrupted(vec2 new_position, std::vector<Wall> m_walls, vec2 screen);
		float hh;
		float hw;
};