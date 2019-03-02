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

	draw_map[entity->id] = draw;
}

std::unordered_map<int, Draw *> DrawCmp::getmap()
{
	return draw_map;
}

Draw* DrawCmp::getDraw(Entity* entity)
{
	return draw_map.at(entity->id);
}
