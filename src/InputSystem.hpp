#include <Components/GameStateCmp.hpp>
#include "Components/TransformCmp.hpp"
#include "Components/InputCmp.hpp"
#include "Components/CollisionCmp.hpp"
#include "ObjectManager.hpp"
#include "common.hpp"

class InputSystem
{
public:
	InputSystem(ObjectManager om, InputCmp ic, TransformCmp tc, CollisionCmp cc, GameStateCmp* gameStateCmp);
	void on_key(GLFWwindow*, int key, int, int action, int mod);
	bool setup(GLFWwindow* m_window);
	void update(float elapsed_ms);

private:
	InputCmp inputComponent;
	TransformCmp transformComponent;
	CollisionCmp collisionComponent;
	ObjectManager objectManager;
	GameStateCmp* gameState;
};