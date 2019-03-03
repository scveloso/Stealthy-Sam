#pragma once
#include "Cmp.hpp"

class DrawCmp : public Cmp
{
public:
	void add(Entity *entity, const char* file);
	std::vector<std::pair<Entity*, Draw *>> getmap();
private:
	std::vector<std::pair<Entity*, Draw *>> draw_vec;
};
