#include "Components/InputCmp.hpp"
#include "Components/TransformCmp.hpp"
#include "Components/CollisionCmp.hpp"
#include "ObjectManager.hpp"
#include "common.hpp"

class MovementSystem
{
public:
	MovementSystem(ObjectManager om, InputCmp ic, TransformCmp tc, CollisionCmp cc);
	void update(float elapsed_ms);
  bool is_movement_interrupted(int entityId, Transform* entityTransform);

private:
  InputCmp inputComponent;
	TransformCmp transformComponent;
  CollisionCmp collisionComponent;
	ObjectManager objectManager;
};
