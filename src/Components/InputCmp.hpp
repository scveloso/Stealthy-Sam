#pragma once
#include "Cmp.hpp"

class InputCmp : public Cmp
{
public:
	void add(Entity* entity);
	std::unordered_map<int, Entity *> getmap();
private:
    std::unordered_map<int, Entity*> input_map;
};
