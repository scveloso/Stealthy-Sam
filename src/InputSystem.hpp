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
	int on_key(GLFWwindow *, int key, int _, int action, int mod);
	bool setup(GLFWwindow* m_window);
	void update(float elapsed_ms);
	int has_move;
	int press_keyE;

private:
	InputCmp inputComponent;
	TransformCmp transformComponent;
	CollisionCmp collisionComponent;
	ObjectManager objectManager;
	GameStateCmp* gameState;
};
