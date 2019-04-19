//
// Created by blue on 3/31/19.
//

#include <iostream>
#include "MissileSystem.hpp"

void MissileSystem::init(ObjectManager *om, CollisionCmp *cc, MovementCmp *mc, GameStateCmp *gameStateCmp, DrawCmp* drawCmp, TransformCmp* tc, btDiscreteDynamicsWorld* dw) {
    objectManager = om;
    collisionComponent = cc;
    movementComponent = mc;
    gameState = gameStateCmp;
    drawComponent = drawCmp;
    transformCmp = tc;
    dynamicWorld = dw;
}

std::pair<std::string, Draw*> MissileSystem::spawnMissile() {
    Entity* boss = objectManager->getEntityByLabel(BOSS_GUID);

    std::ostringstream out;
    out << MISSILE_LABEL_PREFIX << num_missiles++;
    Entity* missile = objectManager->makeEntity(out.str());

    collisionComponent->add(missile);
    movementComponent->add(missile, 50.f, 0.f);
    missile->ui = true;
    transformCmp->add(missile, transformCmp->getTransform(boss)->m_position, {3.125f, 3.125f}, 0.0);
    drawComponent->add(missile, textures_path("Dungeon/boss_bolt.png"));


    // Launch in direction of sam (copied from Sam G's code)
    vec2 sampos = gameState->sam_position;
    vec2 bossPosition = transformCmp->getTransform(boss)->m_position;
    double xval = sampos.x - bossPosition.x;
    double yval = sampos.y - bossPosition.y;
    double normval = sqrt( pow(xval,2) + pow(yval,2) );
    vec2 throwDir = { static_cast<float>(xval / normval) * 100 , static_cast<float>(yval / normval) * 100 };

    movementComponent->setVecDirection(missile, throwDir);

    Collision* collision = collisionComponent->getCollision(missile);
    collision->shape = new btBoxShape(btVector3(10, 10, 0));
    createMissile(missile, collision, bossPosition);

    collision->body->setLinearVelocity(btVector3(throwDir.x, throwDir.y, 0));

    Transform* transform = transformCmp->getTransform(missile);
    transform->body = collision->body;

    dynamicWorld->addRigidBody(collision->body);

    return std::make_pair(out.str(), drawComponent->getDrawByLabel(out.str()));
}

void createMissile(Entity* self, Collision* collision, vec2 pos)
{
    // Set default rotation and pos from param
    btQuaternion rotation;
    rotation.setEulerZYX(0,0,0);
    btVector3 position = btVector3(pos.x, pos.y, 0);
    btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(rotation, position));

    // Default mass of 1, dunno if we need inertia
    btScalar bodyMass = 1.0;
    btVector3 bodyInertia;
    collision->shape->calculateLocalInertia(bodyMass, bodyInertia);

    btRigidBody::btRigidBodyConstructionInfo bodyCI = btRigidBody::btRigidBodyConstructionInfo(bodyMass, motionState, collision->shape, bodyInertia);

    // Restitution is how much energy is lost on bouncing
    bodyCI.m_restitution = 0.9f;
    bodyCI.m_friction = 0.5f;

    collision->body = new btRigidBody(bodyCI);

    collision->body->setUserPointer(self);

    collision->body->setLinearFactor(btVector3(1,1,0));
}

