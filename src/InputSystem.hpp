#include <Components/GameStateCmp.hpp>
#include "Components/TransformCmp.hpp"
#include "Components/InputCmp.hpp"
#include "Components/ItemCmp.hpp"
#include "Components/CollisionCmp.hpp"
#include "Components/MovementCmp.hpp"
#include "Components/EnemyCmp.hpp"
#include "ObjectManager.hpp"
#include "common.hpp"

class InputSystem
{
public:
	void init(ObjectManager om, InputCmp ic, TransformCmp tc, CollisionCmp cc, MovementCmp mc, EnemyCmp ec, ItemCmp itc, GameStateCmp* gameStateCmp);
	int on_key(GLFWwindow *, int key, int _, int action, int mod);
	bool setup(GLFWwindow* m_window);
	void handleThrowable(Entity* entity);

private:
	InputCmp inputComponent;
	TransformCmp transformComponent;
	CollisionCmp collisionComponent;
	MovementCmp movementComponent;
	EnemyCmp enemyComponent;
	ItemCmp itemComponent;
	ObjectManager objectManager;
	GameStateCmp* gameState;
};
