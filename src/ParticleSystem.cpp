#include <Components/GameStateCmp.hpp>
#include "ParticleSystem.hpp"
#include "Components/Cmp.hpp"
#include "common.hpp"
#include "Strategies/strategies_common.hpp"
#include "TileConstants.hpp"
#include <iostream>
#include "UpdateAction.hpp"

// System to handle particles
//
// Has access to DrawCmp to know which texture to draw for an entity.
// Has access to GameStateCmp to allow other systems to know where Sam is.
// Has access to TransformCmp to know where everything is.
void ParticleSystem::init(ObjectManager* om, DrawCmp* dc, TransformCmp* tc, MovementCmp* mc, GameStateCmp* gameStateCmp, ParticleCmp* pc)
{
	objectManager = om;
	drawComponent = dc;
	transformComponent = tc;
	movementComponent = mc;
	gameState = gameStateCmp;
	particleComponent = pc;
}

Entity* ParticleSystem::update()
{
    for (auto& it : particleComponent->getmap()) {
        Entity* entity = objectManager->getEntity(it.first);
        if (!entity->active) continue;

        int lifetime = particleComponent->decrementLifetime(entity);
        if (lifetime <= 0) {
//            printf("trying to remove particle from transformCmp...\n");
//            transformComponent->remove(entity);
//            printf("trying to remove particle from draw...\n");
//            drawComponent->remove(entity);
//            printf("trying to remove particle from move...\n");
//            movementComponent->remove(entity);
//            printf("trying to remove particle from obj...\n");
//            objectManager->removeEntity(entity);
//            printf("trying to delete particle...\n");
//            free(entity);
//            printf("particle deleted\n");
            entity->active = false;
        }
    }

      std::vector<Entity*> torchEntities = objectManager->getEntitiesByLabel("Torch");
      for (auto& torchEntity : torchEntities)
      {
         if (torchEntity->active) {
            if (rand() % 100 == 0) {
                return torchEntity;

            }
         }
      }

      std::vector<Entity*> cauldronEntities = objectManager->getEntitiesByLabel("Cauldron");
      for (auto& cauldronEntity : cauldronEntities)
      {
        if (!cauldronEntity->active) continue;
        if (rand() % 200 == 0) {
            return cauldronEntity;
        }
      }

        return NULL;

}

std::pair<std::string, Draw*> ParticleSystem::spawnSmoke(Entity* entity) {
    std::ostringstream out;
    out << SMOKE_LABEL_PREFIX << numSmoke++;
    Entity* newSmoke = objectManager->makeEntity(out.str());

    vec2 pos = transformComponent->getTransform(entity)->m_position;

    int width = entity->label == TORCH_LABEL ? 5:25;
    int y_offset = entity->label == TORCH_LABEL ? 15:TILE_WIDTH;

    movementComponent->add(newSmoke, 25, 0);
    movementComponent->setMovementDirection(newSmoke, UP);
    newSmoke->ui = true;
    drawComponent->add(newSmoke, textures_path("Dungeon/smoke_particle_new.png"));

    float offset = (rand() % width) + 1;
    float offset_m = rand() % 2 == 0 ? 1.f : -1.f;
    vec2 spawnPos = {pos.x + (offset * offset_m) , pos.y - y_offset};
    transformComponent->add(newSmoke, spawnPos, {3.f,3.f}, 0);

    particleComponent->add(newSmoke);

    return std::make_pair(out.str(), drawComponent->getDrawByLabel(out.str()));
}
