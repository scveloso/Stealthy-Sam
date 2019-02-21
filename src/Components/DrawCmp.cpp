#include "DrawCmp.hpp"

void DrawCmp::add(Entity *entity, const char* file)
{
	Draw *draw = new Draw();

	if (!draw->texture.is_valid())
	{
		if (!draw->texture.load_from_file(file))
		{
			fprintf(stderr, "Failed to load turtle texture!\n");
		}
	}

	//draw->texture = texture;

	printf("LOADED TEXTURE %d\n", draw->texture.height);

	draw_list.emplace_back(std::make_pair(entity, draw));
}

std::vector<std::pair<Entity*, Draw*>>  DrawCmp::getmap()
{
	return draw_list;
}

Draw* DrawCmp::getDraw(Entity* entity)
{
	return draw_list.at(entity->id).second;
}