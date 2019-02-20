#pragma once
#include "Component.hpp"

class DrawCmp : public Component
{
public:
	void add(Entity entity, const char* file);
	std::vector< std::pair<Entity, Draw*>> getmap();
	Draw* getDraw(Entity entity);
private:
	std::vector< std::pair<Entity, Draw*>> draw_list;
};