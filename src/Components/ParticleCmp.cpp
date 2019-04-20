#include "ParticleCmp.hpp"

void ParticleCmp::add(Entity *entity) {
	particle_map[entity->id] = 0;
}

void ParticleCmp::remove(Entity *entity) {
	particle_map.erase(entity->id);
}

int ParticleCmp::getLifetime(Entity* entity) {
    return particle_map[entity->id];
}

void ParticleCmp::resetLifetime(Entity* entity) {
    particle_map[entity->id] = PARTICLE_LIFETIME;
}

int ParticleCmp::decrementLifetime(Entity* entity) {
    particle_map[entity->id]--;
    return particle_map[entity->id];
}

std::unordered_map<int, int> ParticleCmp::getmap()
{
	return particle_map;
}
