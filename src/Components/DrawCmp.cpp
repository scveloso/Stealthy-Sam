#include "DrawCmp.hpp"

void DrawCmp::add(Entity *entity, const char* file)
{
	Draw *draw = new Draw();

	if (!draw->texture.is_valid())
	{
		if (!draw->texture.load_from_file(file))
		{
			fprintf(stderr, "Failed to load texture!\n");
		}
	}

	draw_vec.emplace_back(std::make_pair(entity, draw));
}

std::vector<std::pair<Entity*, Draw *>> DrawCmp::getmap()
{
	return draw_vec;
}
