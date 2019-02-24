#include <iostream>
#include <cstring>
#include "InputSystem.hpp"
#include "Components/Component.hpp"
#include "UpdateAction.hpp"
#include <iostream>
#include <string>

InputSystem::InputSystem(ObjectManager om, InputCmp ic, TransformCmp tc, CollisionCmp cc, GameStateCmp* gameStateCmp)
{
    objectManager = om;
    inputComponent = ic;
    transformComponent = tc;
    collisionComponent = cc;
    gameState = gameStateCmp;
}

bool InputSystem::setup(GLFWwindow* m_window)
{
	return true;
}

int InputSystem::on_key(GLFWwindow *, int key, int _, int action, int mod)
{
    int returnAction = NO_CHANGE;
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
                            transformComponent.setDirection(entity, LEFT);
                        std::cout << transformComponent.getDirection(entity) << std::endl;
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
						if (it.first == SAMS_GUID && collisionComponent.getmap().at(SAMS_GUID)->closet == true) {
							if (transformComponent.getTransform(entity)->visible == false) {
								transformComponent.getTransform(entity)->visible = true;
							}
							else {
								transformComponent.getTransform(entity)->visible = false;
							}
						}
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
					case GLFW_KEY_R:
						returnAction = RESET_GAME;
						break;
                    default:
                        break;
				}
			}
		}
	}


	// Deal with textboxes only if we're in room one:
	if (gameState->current_room == ROOM_ONE_GUID) {
		// If we move using WASD, remove the text box showing movement directions
		if (didMove) {
			Entity* wasd_text_ent = objectManager.getEntityByLabel(USE_WASD_TEXT_LABEL);
			has_move = 1;
			// If we've moved and haven't already made this inactive, trigger the second text box (press E) to appear
			if (wasd_text_ent->active) {
				wasd_text_ent->active = false;
				objectManager.getEntityByLabel(USE_E_INTERACT_LABEL)->active = true;
			}
		}

		// Once they press E, we can remove the E text box
		if (didPressE) {
			press_keyE = 1;
			objectManager.getEntityByLabel(USE_E_INTERACT_LABEL)->active = false;
		}
	}

    return returnAction;
}

void InputSystem::update(float elapsed_ms)
{
	const float SAM_SPEED = 200.f;
	float step = SAM_SPEED * (elapsed_ms / 1000);

	for (auto& it : inputComponent.getmap())
	{
		Entity* entity = objectManager.getEntity(it.first);

		// Don't move sam's position if he's dead
		if (entity->id == SAMS_GUID && !gameState->sam_is_alive) {
		    continue;
		}

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
