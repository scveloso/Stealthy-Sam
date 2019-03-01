#include <Components/GameStateCmp.hpp>
#include "Components/InputCmp.hpp"
#include "Components/TransformCmp.hpp"
#include "Components/CollisionCmp.hpp"
#include "Components/MovementCmp.hpp"
#include "ObjectManager.hpp"
#include "common.hpp"

class MovementSystem
{
public:
	void init(ObjectManager om, InputCmp ic, TransformCmp tc, CollisionCmp cc, MovementCmp mc, GameStateCmp* gameStateCmp);
	void update(float elapsed_ms);
  bool is_movement_interrupted(int entityId, Transform* entityTransform);
	void stopEntityMovement(Entity* entity);

private:
	InputCmp inputComponent;
	TransformCmp transformComponent;
	CollisionCmp collisionComponent;
	MovementCmp movementComponent;
	ObjectManager objectManager;
	GameStateCmp *gameState;
};
