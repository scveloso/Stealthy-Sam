#pragma once
#include "Component.hpp"

class InputCmp : public Cmp
{
public:
	void add(Entity *entity);
	std::vector<Entity*> getmap();
private:
	std::vector<Entity*> input_list;

};