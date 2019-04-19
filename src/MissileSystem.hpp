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
    void init(ObjectManager* om, CollisionCmp* cc, MovementCmp* mc, GameStateCmp* gameStateCmp, DrawCmp* drawCmp, TransformCmp* transformCmp, btDiscreteDynamicsWorld* dw);
    std::pair<std::string, Draw*> spawnMissile();
private:
    CollisionCmp* collisionComponent;
    MovementCmp* movementComponent;
    ObjectManager* objectManager;
    GameStateCmp *gameState;
    DrawCmp *drawComponent;
    TransformCmp *transformCmp;
    int num_missiles;
    btDiscreteDynamicsWorld* dynamicWorld;
};
const std::string MISSILE_LABEL_PREFIX = "Missile_";
void createMissile(Entity* self, Collision* collision, vec2 pos);
