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
  bool is_movement_interrupted(vec2 new_position);
  bool AABB(Transform *tr1, Transform *tr2, vec2 new_position);

private:
  InputCmp inputComponent;
	TransformCmp transformComponent;
  CollisionCmp collisionComponent;
	ObjectManager objectManager;
};
