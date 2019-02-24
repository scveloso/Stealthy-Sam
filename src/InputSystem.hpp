#include "Components/TransformCmp.hpp"
#include "Components/InputCmp.hpp"
#include "Components/CollisionCmp.hpp"
#include "ObjectManager.hpp"
#include "common.hpp"

class InputSystem
{
public:
	InputSystem(ObjectManager om, InputCmp ic, TransformCmp tc, CollisionCmp cc);
	void on_key(GLFWwindow*, int key, int, int action, int mod);
	bool setup(GLFWwindow* m_window);
<<<<<<< HEAD
	void update(float elapsed_ms);
	int has_move;
	int press_keyE;
=======
>>>>>>> Implemented MovementSystem, moves Sam and checks for wall collisions.

private:
	InputCmp inputComponent;
	TransformCmp transformComponent;
	CollisionCmp collisionComponent;
	ObjectManager objectManager;
};
