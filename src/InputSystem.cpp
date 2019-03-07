#include <iostream>
#include <cstring>
#include "InputSystem.hpp"
#include "Components/Cmp.hpp"
#include "UpdateAction.hpp"
#include "EnemyAction.hpp"
#include <iostream>
#include <string>
#include "TileConstants.hpp"

// System to update game based on user input.
//
// Has access to GameStateCmp to know about whether Sam
// completed tutorial actions.
// Has access to InputCmp to be able to access entities requiring user input.
// Has access to CollisionCmp to know if Sam can interact with objects he is colliding with.
// Has access to TransformCmp to know where everything is.
// Has access to MovementCmp to allow Sam to throw held items (move those items).
// Has access to ItemCmp to toggle items as thrown.
void InputSystem::init(ObjectManager om, InputCmp ic, TransformCmp tc, CollisionCmp cc, MovementCmp mc, EnemyCmp ec, ItemCmp itc, GameStateCmp* gameStateCmp)
{
    objectManager = om;
    inputComponent = ic;
    transformComponent = tc;
    collisionComponent = cc;
    movementComponent = mc;
    enemyComponent = ec;
    itemComponent = itc;
    gameState = gameStateCmp;
}

bool InputSystem::setup(GLFWwindow* m_window)
{
	return true;
}

int InputSystem::on_key(GLFWwindow *, int key, int _, int action, int mod)
{
  int returnAction = NO_CHANGE;

  // Update relevant entities based on input key
	for (auto& it : inputComponent.getmap())
	{
    Entity* entity = objectManager.getEntity(it.first);

    // Handle inputs for Sam
    if (entity->label.compare("Player") == 0)
    {
      if (action == GLFW_PRESS)
      {
        float direction = movementComponent.getMovementDirection(entity);
        switch (key) {
          case GLFW_KEY_A:
            if (!movementComponent.isGoingLeft(entity))
            {
              movementComponent.setMovementDirection(entity, LEFT);
              if (gameState->sam_is_alive) {
                transformComponent.faceLeft(entity); // this is to rotate Sam texture
                transformComponent.setFacingDirection(entity, LEFT);
              }
              gameState->has_moved = true;
            }
            break;
          case GLFW_KEY_D:
            if (!movementComponent.isGoingRight(entity))
            {
              movementComponent.setMovementDirection(entity, RIGHT);
              if (gameState->sam_is_alive) {
                transformComponent.faceRight(entity); // this is to rotate Sam texture
                transformComponent.setFacingDirection(entity, RIGHT);
              }
              gameState->has_moved = true;
            }
            break;
          case GLFW_KEY_S:
            movementComponent.setMovementDirection(entity, DOWN);
            if (gameState->sam_is_alive) {
              transformComponent.setFacingDirection(entity, DOWN);
            }
            gameState->has_moved = true;
            break;
          case GLFW_KEY_W:
            movementComponent.setMovementDirection(entity, UP);
            if (gameState->sam_is_alive) {
              transformComponent.setFacingDirection(entity, UP);
            }
            gameState->has_moved = true;
            break;
          case GLFW_KEY_E:
            if (it.first == SAMS_GUID && collisionComponent.getmap().at(SAMS_GUID)->closet == true)
            {
              if (transformComponent.getTransform(entity)->visible == false)
              {
                transformComponent.getTransform(entity)->visible = true;
              }
              else
              {
                transformComponent.getTransform(entity)->visible = false;
              }
            }
            gameState->has_pressed_E = true;
            break;
          case GLFW_KEY_P:
            // Print out current position
            printf("X:%f , Y:%f \n", transformComponent.getTransform(entity)->m_position.x,
            transformComponent.getTransform(entity)->m_position.y);
            break;
          case GLFW_KEY_C:
          {
            // Print out collision stuff
            Transform *tr1 = transformComponent.getTransform(entity);
            vec2 size1 = {tr1->width * tr1->m_scale.x, tr1->height * tr1->m_scale.y};

            printf("%f, %f, \n", tr1->width, tr1->height);
            printf("SIZEX:%f , SIZEY:%f \n",
                               transformComponent.getTransform(entity)->m_position.x + size1.x,
                               transformComponent.getTransform(entity)->m_position.y + size1.y);
            break;
          }
          case GLFW_KEY_T:
          {
            handleThrowable(entity);

            break;
          }
        }
      }
      if (action == GLFW_RELEASE)
      {
        switch (key) {
          case GLFW_KEY_A:
            movementComponent.removeMovementDirection(entity, LEFT);
            break;
          case GLFW_KEY_D:
            movementComponent.removeMovementDirection(entity, RIGHT);
            break;
          case GLFW_KEY_S:
            movementComponent.removeMovementDirection(entity, DOWN);
            break;
          case GLFW_KEY_W:
            movementComponent.removeMovementDirection(entity, UP);
            break;
          default:
            break;
        }
      }
    }
	}

  if (action == GLFW_RELEASE)
  {
    switch (key) {
      case GLFW_KEY_R:
        returnAction = RESET_GAME;
        break;
      default:
        break;
    }
  }

	return returnAction;
}

void InputSystem::handleThrowable(Entity* entity) {
  Entity* heldEntity = gameState->held_entity;

  // If a torch is thrown
  if (heldEntity && heldEntity->label.compare("Torch") == 0)
  {
    // Figure out what direction to throw the torch in,
    // MovementDirection is prioritized, facing direction used at rest
    int throwDirection = NO_DIRECTION;
    int movementDirection = movementComponent.getMovementDirection(entity);
    int facingDirection = transformComponent.getFacingDirection(entity);

    if (movementDirection != NO_DIRECTION) {
      throwDirection = movementDirection;
    } else {
      throwDirection = facingDirection;
    }

    // Throw torch if there is a valid direction to throw it in
    if (throwDirection != NO_DIRECTION) {
      Movement* entityMovement = movementComponent.getMovement(heldEntity);
      movementComponent.resetMovementDirection(heldEntity);

      vec2 torch_position = gameState->sam_position;
      if (throwDirection % LEFT == 0) {
        torch_position = { gameState->sam_position.x - (TILE_WIDTH), gameState->sam_position.y };
        movementComponent.setMovementDirection(heldEntity, LEFT);
      }

      if (throwDirection % RIGHT == 0) {
        torch_position = { gameState->sam_position.x + (TILE_WIDTH), gameState->sam_position.y };
        movementComponent.setMovementDirection(heldEntity, RIGHT);
      }

      if (throwDirection % UP == 0) {
        torch_position = { gameState->sam_position.x, gameState->sam_position.y - (TILE_HEIGHT) };
        movementComponent.setMovementDirection(heldEntity, UP);
      }

      if (throwDirection % DOWN == 0) {
        torch_position = { gameState->sam_position.x, gameState->sam_position.y + (TILE_HEIGHT) };
        movementComponent.setMovementDirection(heldEntity, DOWN);
      }

      // Torch is now active and thrown
      heldEntity->active = true;
      itemComponent.throwItem(heldEntity);

      // Set its position and get it moving
      transformComponent.setPosition(heldEntity, torch_position);
      movementComponent.setCurrSpeed(heldEntity, entityMovement->baseSpeed);

      // Update enemies chasing Sam to chase torch instead
      enemyComponent.updateEnemyAction(CHASE_SAM, CHASE_TORCH);

      // No longer holding torch
      gameState->held_item = -1;
      gameState->held_entity = NULL;
    }
  }
}
