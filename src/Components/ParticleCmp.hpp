#pragma once
#include "Cmp.hpp"

class ParticleCmp : public Cmp
{
public:
	void add(Entity* entity);
	void remove(Entity* entity);
	int getLifetime(Entity* entity);
	int decrementLifetime(Entity* entity);
	void resetLifetime(Entity* entity);

	// Map of <entity id , remaining lifetime>
	std::unordered_map<int, int> getmap();
private:
    std::unordered_map<int, int> particle_map;
};

const int PARTICLE_LIFETIME = 100;
