#include <Components/GameStateCmp.hpp>
#include "Components/InputCmp.hpp"
#include "Components/TransformCmp.hpp"
#include "Components/CollisionCmp.hpp"
#include "Components/MovementCmp.hpp"
#include "Components/EnemyCmp.hpp"
#include "ObjectManager.hpp"
#include "SoundManager.hpp"
#include "common.hpp"

class MovementSystem
{
public:
	void init(ObjectManager* om, TransformCmp* tc, CollisionCmp* cc, MovementCmp* mc, GameStateCmp* gameStateCmp);
	void update(float elapsed_ms);
  bool is_movement_interrupted(Entity* entity, Transform* entityTransform);
	void stopEntityMovement(Entity* entity);
	void torch_cauldron_collision(int entityId, Transform* entityTransform);
	void cauldronCheck(Entity *entity, Transform *entityTransform);

private:
	TransformCmp* transformComponent;
	CollisionCmp* collisionComponent;
	MovementCmp* movementComponent;
	ObjectManager* objectManager;
	GameStateCmp *gameState;

    void handleBossDoor(Entity *pEntity, Entity *qEntity);

	void handleTimer(float elapsed_ms) const;
};
