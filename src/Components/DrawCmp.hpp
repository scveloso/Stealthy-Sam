#pragma once
#include "Cmp.hpp"

class DrawCmp : public Cmp
{
public:
	void add(Entity *entity, const char* file);
	std::unordered_map<int, Draw *> getmap();
	Draw* getDraw(Entity *entity);
private:
	std::unordered_map<int, Draw*> draw_map;
};
