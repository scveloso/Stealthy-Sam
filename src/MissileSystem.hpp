#include <Components/GameStateCmp.hpp>
#include <Components/CollisionCmp.hpp>
#include <Components/MovementCmp.hpp>
#include <Components/DrawCmp.hpp>
#include <Components/TransformCmp.hpp>
#include "ObjectManager.hpp"

#include <stdio.h>
#include <sstream>

class MissileSystem {
public:
    void init(ObjectManager* om, CollisionCmp* cc, MovementCmp* mc, GameStateCmp* gameStateCmp, DrawCmp* drawCmp, TransformCmp* transformCmp);
    std::pair<std::string, Draw*> spawnMissile();
private:
    CollisionCmp* collisionComponent;
    MovementCmp* movementComponent;
    ObjectManager* objectManager;
    GameStateCmp *gameState;
    DrawCmp *drawComponent;
    TransformCmp *transformCmp;
    int num_missiles;
};
