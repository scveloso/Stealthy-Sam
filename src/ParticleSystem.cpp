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

void ParticleSystem::clearDeadParticles() {
	for (auto& it : particleComponent->getmap()) {
			Entity* entity = objectManager->getEntity(it.first);
			if (!entity->active) continue;

			int lifetime = particleComponent->decrementLifetime(entity);
			if (lifetime <= 0) {
				 printf("trying to remove particle from transformCmp...\n");
				 transformComponent->remove(entity);
				 printf("trying to remove particle from draw...\n");
				 drawComponent->remove(entity);
				 printf("trying to remove particle from move...\n");
				 movementComponent->remove(entity);
				 printf("trying to remove particle from particle...\n");
				 particleComponent->remove(entity);
				 printf("trying to remove particle from obj...\n");
				 objectManager->removeEntity(entity);
				 printf("trying to delete particle...\n");
				 //delete entity;
				 printf("particle deleted\n");
					//entity->active = false;
			}
	}
}

Entity* ParticleSystem::update()
{
    for (auto& it : particleComponent->getmap()) {
        Entity* entity = objectManager->getEntity(it.first);
        if (!entity->active) continue;

        int lifetime = particleComponent->decrementLifetime(entity);
        if (lifetime <= 0) {
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
        if (rand() % 50 == 0) {
            return cauldronEntity;
        }
      }

        return NULL;

}

void ParticleSystem::spawnSmoke(Entity* entity) {
	for (int i = 0; i < 7; i++) {
		std::ostringstream out;
		out << SMOKE_LABEL_PREFIX << entity->id << i;

		Entity* particleEntity = objectManager->getEntityByLabel(out.str());

		if (particleComponent->getLifetime(particleEntity) <= 0) {
			particleEntity->active = true;

			// Set random position
			vec2 pos = transformComponent->getTransform(entity)->m_position;
	    int width = entity->label == TORCH_LABEL ? 5:25;
	    int y_offset = entity->label == TORCH_LABEL ? 15:TILE_WIDTH;
			float scale = entity->label == TORCH_LABEL ? 1.5f : 3.f;
			float offset = (rand() % width) + 1;
	    float offset_m = rand() % 2 == 0 ? 1.f : -1.f;
			vec2 spawnPos = {pos.x + (offset * offset_m) , pos.y - y_offset};
			transformComponent->setPosition(particleEntity, spawnPos);
			particleComponent->resetLifetime(particleEntity);
			break;
		}
	}
}
