#pragma once
#include "Cmp.hpp"

class CollisionCmp : public Cmp
{
public:
	void add(Entity *entity);
	std::unordered_map<int, Collision*> getmap();
	Collision* getCollision(Entity *entity);
	~CollisionCmp();

    void remove(Entity *entity);

private:
	std::unordered_map<int, Collision*> collision_map;
};
