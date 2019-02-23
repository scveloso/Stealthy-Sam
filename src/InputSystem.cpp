#include <iostream>
#include <cstring>
#include "InputSystem.hpp"
#include "Components/Component.hpp"
#include <iostream>
#include <string>

InputSystem::InputSystem(ObjectManager om, InputCmp ic, TransformCmp tc, CollisionCmp cc)
{
    objectManager = om;
    inputComponent = ic;
    transformComponent = tc;
    collisionComponent = cc;
}

bool InputSystem::setup(GLFWwindow* m_window)
{
	glfwSetWindowUserPointer(m_window, this);
	auto key_redirect = [](GLFWwindow* wnd, int _0, int _1, int _2, int _3) { ((InputSystem*)glfwGetWindowUserPointer(wnd))->on_key(wnd, _0, _1, _2, _3); };
	glfwSetKeyCallback(m_window, key_redirect);
	return true;
}

void InputSystem::on_key(GLFWwindow*, int key, int, int action, int mod)
{
    bool didMove = false;
    bool didPressE = false;
	for (auto& it : inputComponent.getmap())
	{
	    Entity* entity = objectManager.getEntity(it.first);
	    if (entity->label == USE_WASD_TEXT_LABEL || entity->label == USE_E_INTERACT_LABEL)
        {
	        // skip
        } else {
            if (action == GLFW_PRESS)
            {
                switch (key) {
                    case GLFW_KEY_A:
<<<<<<< HEAD
                        if (!collisionComponent.getmap()[it.first]->left) {
                            transformComponent.setDirection(entity, LEFT);
                        std::cout << transformComponent.getDirection(entity) << std::endl;}
=======
                            transformComponent.setDirection(entity, LEFT);
                        std::cout << transformComponent.getDirection(entity) << std::endl;
>>>>>>> e06218ff58fb4ad84070409b7f668472c84ef270
                        didMove = true;
                        break;
                    case GLFW_KEY_D:
                        transformComponent.setDirection(entity, RIGHT);std::cout << transformComponent.getDirection(entity) << std::endl;
                        didMove = true;
                        break;
                    case GLFW_KEY_S:
                        transformComponent.setDirection(entity, DOWN);std::cout << transformComponent.getDirection(entity) << std::endl;
                        didMove = true;
                        break;
                    case GLFW_KEY_W:
                        transformComponent.setDirection(entity, UP);std::cout << transformComponent.getDirection(entity) << std::endl;
                        didMove = true;
                        break;
                    case GLFW_KEY_E:
                        // TODO: Implement interactables
<<<<<<< HEAD
=======
						if (it.first == SAMS_GUID && collisionComponent.getmap().at(SAMS_GUID)->closet == true) {
							if (transformComponent.getTransform(entity)->visible == false) {
								transformComponent.getTransform(entity)->visible = true;
								printf("visible");
							}
							else {
								transformComponent.getTransform(entity)->visible = false;
							}
						}
>>>>>>> e06218ff58fb4ad84070409b7f668472c84ef270
                        didPressE = true;
                        break;
                    case GLFW_KEY_P:
                        //print out current position
                        printf("X:%f , Y:%f \n", transformComponent.getTransform(entity)->m_position.x,
                               transformComponent.getTransform(entity)->m_position.y);
                        break;
                    case GLFW_KEY_C:
                        //print out collision stuff
                        Transform *tr1 = transformComponent.getTransform(entity);
                        vec2 size1 = {tr1->width * tr1->m_scale.x, tr1->height * tr1->m_scale.y};

                        printf("%f, %f, \n", tr1->width, tr1->height);

                        printf("SIZEX:%f , SIZEY:%f \n",
                               transformComponent.getTransform(entity)->m_position.x + size1.x,
                               transformComponent.getTransform(entity)->m_position.y + size1.y);
                        break;
                }

            }

            if (action == GLFW_RELEASE)
            {
                switch (key)
                {
                    case GLFW_KEY_A:
                        transformComponent.removeDirection(entity, LEFT);
                        break;
                    case GLFW_KEY_D:
                        transformComponent.removeDirection(entity, RIGHT);
                        break;
                    case GLFW_KEY_S:transformComponent.removeDirection(entity, DOWN);
                        break;
                    case GLFW_KEY_W:
                        transformComponent.removeDirection(entity, UP);
                        break;
                    default:
                        break;
                }
            }
        }
<<<<<<< HEAD
	}

	// If we move using WASD, remove the text box showing movement directions
	if (didMove) {
	    Entity* wasd_text_ent = objectManager.getEntityByLabel(USE_WASD_TEXT_LABEL);
      move = 1;
	    // If we've moved and haven't already made this inactive, trigger the second text box (press E) to appear
	    if (wasd_text_ent->active) {
	        wasd_text_ent->active = false;
            objectManager.getEntityByLabel(USE_E_INTERACT_LABEL)->active = true;
	    }
	}

	// Once they press E, we can remove the E text box
    if (didPressE) {
        keyE = 1;
=======
	}

	// If we move using WASD, remove the text box showing movement directions
	if (didMove) {
	    Entity* wasd_text_ent = objectManager.getEntityByLabel(USE_WASD_TEXT_LABEL);

	    // If we've moved and haven't already made this inactive, trigger the second text box (press E) to appear
	    if (wasd_text_ent->active) {
	        wasd_text_ent->active = false;
            objectManager.getEntityByLabel(USE_E_INTERACT_LABEL)->active = true;
	    }
	}

	// Once they press E, we can remove the E text box
    if (didPressE) {
>>>>>>> e06218ff58fb4ad84070409b7f668472c84ef270
        objectManager.getEntityByLabel(USE_E_INTERACT_LABEL)->active = false;
    }
}

void InputSystem::update(float elapsed_ms)
{
	const float SAM_SPEED = 200.f;
	float step = SAM_SPEED * (elapsed_ms / 1000);

	for (auto& it : inputComponent.getmap())
	{
		Entity* entity = objectManager.getEntity(it.first);

		int direction = transformComponent.getTransform(entity)->direction;

		vec2 new_position = transformComponent.getTransform(entity)->m_position;

		if (direction % LEFT == 0)
		{
			new_position.x = new_position.x - step;
		}

		if (direction % RIGHT == 0)
		{
			new_position.x = new_position.x + step;
		}

		if (direction % DOWN == 0)
		{
			new_position.y = new_position.y + step;
		}

		if (direction % UP == 0)
		{
			new_position.y = new_position.y - step;
		}

		if (transformComponent.getTransform(entity)->visible == true) {
			transformComponent.setPosition(entity, new_position);
		}
	}
}
