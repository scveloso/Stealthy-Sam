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

void DrawCmp::remove(Entity* entity) {
    int i = 0;
    for (auto& it : draw_vec) {
        if (it.first->id == entity->id) {
					delete it.second;
          draw_vec.erase(draw_vec.begin()+i);
          return;
        }
        i++;
	}
}

DrawCmp::~DrawCmp() {
	//printf("DRAWCMP DESTRUCTOR \n");

	for (auto& it : draw_vec) {
		delete it.second;
	}
	draw_vec.clear();

	// 281 MB to 261MB
}

void DrawCmp::addGhost(Entity *entity, const char* default_file, const char* chase_file)
{
	Draw *draw = new Draw();

	if (!draw->texture.is_valid())
	{
		if (!draw->texture.load_from_file(default_file))
		{
			fprintf(stderr, "Failed to load texture!\n");
		}
	}

    if (!draw->chase.is_valid())
    {
        if (!draw->chase.load_from_file(chase_file))
        {
            fprintf(stderr, "Failed to load texture!\n");
        }
    }

	draw_vec.emplace_back(std::make_pair(entity, draw));
}


void DrawCmp::addFull(Entity *entity, const char* file1, const char* file2, const char* file3
              	    , const char* file4, const char* file5, const char* file6, const char* file7
              	    , const char* file8, const char* file9)
{
	Draw *draw = new Draw();

	if (!draw->texture.is_valid())
	{
		if (!draw->texture.load_from_file(file1))
		{
			fprintf(stderr, "Failed to load texture!\n");
		}
	}

	if (!draw->up.is_valid())
    {
        if (!draw->up.load_from_file(file2))
        {
            fprintf(stderr, "Failed to load texture!\n");
        }
    }

    if (!draw->down.is_valid())
    {
        if (!draw->down.load_from_file(file3))
        {
            fprintf(stderr, "Failed to load texture!\n");
        }
    }

    if (!draw->stepup1.is_valid())
    {
        if (!draw->stepup1.load_from_file(file4))
        {
            fprintf(stderr, "Failed to load texture!\n");
        }
    }

    if (!draw->stepup2.is_valid())
    {
        if (!draw->stepup2.load_from_file(file5))
        {
            fprintf(stderr, "Failed to load texture!\n");
        }
    }

    if (!draw->stepdown1.is_valid())
    {
        if (!draw->stepdown1.load_from_file(file6))
        {
            fprintf(stderr, "Failed to load texture!\n");
        }
    }

    if (!draw->stepdown2.is_valid())
    {
        if (!draw->stepdown2.load_from_file(file7))
        {
            fprintf(stderr, "Failed to load texture!\n");
        }
    }

    if (!draw->stepside1.is_valid())
    {
        if (!draw->stepside1.load_from_file(file8))
        {
            fprintf(stderr, "Failed to load texture!\n");
        }
    }

    if (!draw->stepside2.is_valid())
    {
        if (!draw->stepside2.load_from_file(file9))
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

Draw* DrawCmp::getDrawByLabel(std::string label)
{
    for (auto& it : draw_vec)
    {
        Entity* entity = it.first;
        if (entity->label == label) {
            return it.second;
        }
    }

    return nullptr;
}
