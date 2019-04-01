//
// Created by blue on 3/31/19.
//

#include "MissileSystem.hpp"

void MissileSystem::init(ObjectManager *om, CollisionCmp *cc, MovementCmp *mc, GameStateCmp *gameStateCmp, DrawCmp* drawCmp, TransformCmp* transformCmp) {
    objectManager = om;
    collisionComponent = cc;
    movementComponent = mc;
    gameState = gameStateCmp;
    drawComponent = drawCmp;
    transformCmp = transformCmp;
}

void MissileSystem::spawnMissile() {
    Entity* sam = objectManager->getEntityByLabel(SAM_GUID);
    // TODO: make go towards sam??
    Entity* boss = objectManager->getEntityByLabel(BOSS_GUID);

    std::ostringstream out;
    out << "Missile_" << num_missiles++;
    Entity* missile = objectManager->makeEntity(out.str());

    collisionComponent->add(missile);
    movementComponent->add(missile, 50.f, 0.f);
    drawComponent->add(missile, textures_path("Dungeon/boss_bolt.png"));
    transformCmp->add(missile, transformCmp->getTransform(boss)->m_position, {3.125f, 3.125f}, 0.0);
}
