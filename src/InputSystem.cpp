#include <iostream>
#include <cstring>
#include "InputSystem.hpp"
#include "Components/Cmp.hpp"
#include "UpdateAction.hpp"
#include "Strategies/strategies_common.hpp"
#include "TileConstants.hpp"
#include "CollisionSystem.hpp"
#include <iostream>
#include <string>
#include <math.h>

// System to update game based on user input.
//
// Has access to GameStateCmp to know about whether Sam
// completed tutorial actions.
// Has access to InputCmp to be able to access entities requiring user input.
// Has access to CollisionCmp to know if Sam can interact with objects he is colliding with.
// Has access to TransformCmp to know where everything is.
// Has access to MovementCmp to allow Sam to throw held items (move those items).
// Has access to ItemCmp to toggle items as thrown.
void InputSystem::init(ObjectManager* om, InputCmp ic, TransformCmp* tc, CollisionCmp* cc, MovementCmp* mc, EnemyCmp ec, ItemCmp itc, GameStateCmp* gameStateCmp)
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

// TODO: Refactor this to only trigger inputs when menus/ui are closed/inactive
int InputSystem::on_key(GLFWwindow *, int key, int _, int action, int mod)
{
  int returnAction = NO_CHANGE;

  // Inputs available in tutorial
  if (objectManager->getEntityByLabel(TUTORIAL_SCREEN)->active)
  {
    if (action == GLFW_PRESS && key == GLFW_KEY_H) {
      toggleTutorial();
    }
  }
  // Inputs available in main menu
  else if (gameState->in_main_menu)
  {
    switch (key) {
      case GLFW_KEY_N: {
        returnAction = START_NEW_GAME;
        break;
      }
      case GLFW_KEY_L: {
        bool loadResult = loadGame();
        if (loadResult) {
          returnAction = LOAD_GAME;
        }
        break;
      }
      default:
        break;
    }
  }
  // Inputs available when game is paused
  else if (gameState->is_game_paused) {
    if (action == GLFW_PRESS) {
      switch (key) {
        case GLFW_KEY_H:
        {
          toggleTutorial();
          break;
        }
        case GLFW_KEY_SPACE:
        {
          togglePause();
          break;
        }
        case GLFW_KEY_B:
          saveGame();
          break;
        case GLFW_KEY_L: {
          bool loadResult = loadGame();
          if (loadResult) {
            returnAction = LOAD_GAME;
          }
          break;
        }
        case GLFW_KEY_N: {
          returnAction = START_NEW_GAME;
          break;
        }
        case GLFW_KEY_M: {
          goToMainMenu();
          break;
        }
        default:
          break;
      }
    }
  }
  // Inputs available in game
  else
  {
    // Update relevant entities based on input key
  	for (auto& it : inputComponent.getmap())
  	{
      Entity* entity = objectManager->getEntity(it.first);

      // Handle inputs for Sam
      if (entity->label.compare("Player") == 0)
      {
        if (action == GLFW_PRESS)
        {
          float direction = movementComponent->getMovementDirection(entity);
          switch (key) {
            case GLFW_KEY_A:
              if (!movementComponent->isGoingLeft(entity))
              {
                movementComponent->setMovementDirection(entity, LEFT);
                if (gameState->sam_is_alive) {
                  transformComponent->faceLeft(entity); // this is to rotate Sam texture
                  transformComponent->setFacingDirection(entity, LEFT);
                }
                gameState->has_moved = true;
              }
              break;
            case GLFW_KEY_D:
              if (!movementComponent->isGoingRight(entity))
              {
                movementComponent->setMovementDirection(entity, RIGHT);
                if (gameState->sam_is_alive) {
                  transformComponent->faceRight(entity); // this is to rotate Sam texture
                  transformComponent->setFacingDirection(entity, RIGHT);
                }
                gameState->has_moved = true;
              }
              break;
            case GLFW_KEY_S:
              movementComponent->setMovementDirection(entity, DOWN);
              if (gameState->sam_is_alive) {
                transformComponent->setFacingDirection(entity, DOWN);
              }
              gameState->has_moved = true;
              break;
            case GLFW_KEY_W:
              movementComponent->setMovementDirection(entity, UP);
              if (gameState->sam_is_alive) {
                transformComponent->setFacingDirection(entity, UP);
              }
              gameState->has_moved = true;
              break;
            case GLFW_KEY_E:
              if (it.first == SAMS_GUID && collisionComponent->getmap().at(SAMS_GUID)->closet == true)
              {
                if (transformComponent->getTransform(entity)->visible == false)
                {
                  transformComponent->getTransform(entity)->visible = true;
                  gameState->hidden = false;
                  SoundManager::getInstance().playCloseCloset();
                }
                else
                {
                  transformComponent->getTransform(entity)->visible = false;
                  gameState->hidden = true;
                  SoundManager::getInstance().playOpenCloset();
                }
              }
              break;
            case GLFW_KEY_C:
            {
              // Print out collision stuff
              Transform *tr1 = transformComponent->getTransform(entity);
              vec2 size1 = {tr1->width * tr1->m_scale.x, tr1->height * tr1->m_scale.y};

              printf("%f, %f, \n", tr1->width, tr1->height);
              printf("SIZEX:%f , SIZEY:%f \n",
                                 transformComponent->getTransform(entity)->m_position.x + size1.x,
                                 transformComponent->getTransform(entity)->m_position.y + size1.y);
              break;
            }
            case GLFW_KEY_T:
            {
              handleThrowable(entity);
              break;
            }
            case GLFW_KEY_SPACE:
            {
              transformComponent->resetFacingDirection(entity);
              movementComponent->resetMovementDirection(entity);
              togglePause();
              break;
            }
          }
        }
        if (action == GLFW_RELEASE)
        {
          switch (key) {
            case GLFW_KEY_A:
              movementComponent->removeMovementDirection(entity, LEFT);
              transformComponent->removeFacingDirection(entity, LEFT);
              break;
            case GLFW_KEY_D:
              movementComponent->removeMovementDirection(entity, RIGHT);
              transformComponent->removeFacingDirection(entity, RIGHT);
              break;
            case GLFW_KEY_S:
              movementComponent->removeMovementDirection(entity, DOWN);
              transformComponent->removeFacingDirection(entity, DOWN);
              break;
            case GLFW_KEY_W:
              movementComponent->removeMovementDirection(entity, UP);
              transformComponent->removeFacingDirection(entity, UP);
              break;
            default:
              break;
          }
        }
      }
  	}
  }

	return returnAction;
}

void InputSystem::goToMainMenu() {
  Entity* pause_screen = objectManager->getEntityByLabel(PAUSE_SCREEN);
  pause_screen->active = !pause_screen->active;

  Entity* main_menu = objectManager->getEntityByLabel(MAIN_MENU);
  main_menu->active = true;

  gameState->in_main_menu = true;
  gameState->is_game_paused = false;
}

void InputSystem::togglePause() {
  Entity* tutorial_screen = objectManager->getEntityByLabel(TUTORIAL_SCREEN);
  Entity* main_menu = objectManager->getEntityByLabel(MAIN_MENU);

  // Can only pause/unpause if showing no other screens
  if (!tutorial_screen->active && !main_menu->active) {
    Entity* pause_screen = objectManager->getEntityByLabel(PAUSE_SCREEN);
    pause_screen->active = !pause_screen->active;

    gameState->is_game_paused = !gameState->is_game_paused;

    if (gameState->is_game_paused) {
      SoundManager().getInstance().pauseMusic();
    } else {
      SoundManager().getInstance().resumeMusic();
    }
  }
}

void InputSystem::toggleTutorial() {
  // Can only access tutorial screen when game is paused
  if (gameState->is_game_paused) {
    Entity* tutorial_screen = objectManager->getEntityByLabel(TUTORIAL_SCREEN);
    tutorial_screen->active = !tutorial_screen->active;
  }
}

void InputSystem::startGame() {
  if (gameState->in_main_menu) {
    gameState->in_main_menu = false;

    Entity* main_menu = objectManager->getEntityByLabel(MAIN_MENU);
    main_menu->active = false;

    SoundManager::getInstance().playBackgroundMusic();
  }
}

void InputSystem::saveGame() {
  if (gameState->is_game_paused) {
    gameState->saveGame();
  }
}

bool InputSystem::loadGame() {
  if (gameState->is_game_paused || gameState->in_main_menu) {
    return gameState->loadGame();
  } else {
    return false;
  }
}

int InputSystem::on_click(GLFWwindow *, int button, int action, int mods) {
    int returnAction = NO_CHANGE;

    if (!gameState->is_game_paused && !gameState->in_main_menu && gameState->sam_is_alive) {
      if (button == 0 && action == GLFW_PRESS) {
          Entity* heldEntity = gameState->held_entity;

          // If a torch is thrown
          if (heldEntity && heldEntity->label.compare("Torch") == 0) {
              // Throw direction is in direction of mouse, normalized
              vec2 sampos = gameState->sam_position;
              vec2 curpos = gameState->cursor_pos;
              double xval = curpos.x - sampos.x;
              double yval = curpos.y - sampos.y;
              double normval = sqrt( pow(xval,2) + pow(yval,2) );
              vec2 throwDir = { static_cast<float>(xval / normval) , static_cast<float>(yval / normval) };


              Transform* entityTransform = transformComponent->getTransform(heldEntity);
              vec2 torch_position = gameState->sam_position;
              torch_position = tryThrowVecDirection(heldEntity, entityTransform, torch_position, throwDir);

              // If torch movement won't be interrupted by a wall/closet
              if (torch_position.x != gameState->sam_position.x ||
                    torch_position.y != gameState->sam_position.y) {
                  // Torch is now active and thrown
                  heldEntity->active = true;
                  itemComponent.throwItem(heldEntity);

                  // Set its position and get it moving
                  transformComponent->setPosition(heldEntity, torch_position);
                  Movement* entityMovement = movementComponent->getMovement(heldEntity);
                  movementComponent->setCurrSpeed(heldEntity, entityMovement->baseSpeed);

                  // Update enemies chasing Sam to chase torch instead
                  enemyComponent.updateEnemyAction(CHASE_SAM, CHASE_TORCH);

                  // No longer holding torch
                  gameState->held_item = -1;
                  gameState->held_entity = NULL;

                  // Throw sound
                  SoundManager::getInstance().playItemThrow();
              }
          }
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
    int movementDirection = movementComponent->getMovementDirection(entity);
    int facingDirection = transformComponent->getPreviousFacingDirection(entity);
    if (movementDirection != NO_DIRECTION) {
      throwDirection = movementDirection;
    } else {
      throwDirection = facingDirection;
    }

    // Throw torch if there is a valid direction to throw it in
    if (throwDirection != NO_DIRECTION) {
      Transform* entityTransform = transformComponent->getTransform(heldEntity);
      movementComponent->resetMovementDirection(heldEntity);

      vec2 torch_position = gameState->sam_position;
      if (throwDirection % LEFT == 0) {
        torch_position = tryThrowHorizontal(heldEntity, entityTransform, torch_position, LEFT, (-1 * TILE_WIDTH));
      }

      if (throwDirection % RIGHT == 0) {
        torch_position = tryThrowHorizontal(heldEntity, entityTransform, torch_position, RIGHT, TILE_WIDTH);
      }

      if (throwDirection % UP == 0) {
        torch_position = tryThrowVertical(heldEntity, entityTransform, torch_position, UP, (-1 * TILE_HEIGHT));
      }

      if (throwDirection % DOWN == 0) {
        torch_position = tryThrowVertical(heldEntity, entityTransform, torch_position, DOWN, TILE_HEIGHT);
      }

      // If torch movement won't be interrupted by a wall/closet
      if (torch_position.x != gameState->sam_position.x ||
          torch_position.y != gameState->sam_position.y) {
        // Torch is now active and thrown
        heldEntity->active = true;
        itemComponent.throwItem(heldEntity);

        SoundManager::getInstance().playItemThrow();

        // Set its position and get it moving
        transformComponent->setPosition(heldEntity, torch_position);
        Movement* entityMovement = movementComponent->getMovement(heldEntity);
        movementComponent->setCurrSpeed(heldEntity, entityMovement->baseSpeed);

        // Update enemies chasing Sam to chase torch instead
        enemyComponent.updateEnemyAction(CHASE_SAM, CHASE_TORCH);

        // No longer holding torch
        gameState->held_item = -1;
        gameState->held_entity = NULL;
      }
    }
  }
}

vec2 InputSystem::tryThrowVecDirection(Entity* heldEntity, Transform* entityTransform, vec2 torch_position, vec2 throwDir) {
  torch_position = { torch_position.x + (TILE_WIDTH * throwDir.x), torch_position.y + (TILE_HEIGHT * throwDir.y)};
  entityTransform->m_position = torch_position;
  bool movementInterrupted = is_movement_interrupted(heldEntity->id, entityTransform);

  torch_cauldron_collision(heldEntity->id, entityTransform);

  if (movementInterrupted) {
    torch_position = gameState->sam_position;
    entityTransform->m_position = torch_position;
  } else {
    movementComponent->setVecDirection(heldEntity, throwDir);
  }

  return torch_position;
}

vec2 InputSystem::tryThrowHorizontal(Entity* heldEntity, Transform* entityTransform, vec2 torch_position, int direction, int offset) {
  torch_position = { torch_position.x + offset, torch_position.y };
  entityTransform->m_position = torch_position;
  bool movementInterrupted = is_movement_interrupted(heldEntity->id, entityTransform);

  torch_cauldron_collision(heldEntity->id, entityTransform);

  if (movementInterrupted) {
    torch_position = { torch_position.x - offset, torch_position.y };
    entityTransform->m_position = torch_position;
  } else {
    movementComponent->setMovementDirection(heldEntity, direction);
  }

  return torch_position;
}

vec2 InputSystem::tryThrowVertical(Entity* heldEntity, Transform* entityTransform, vec2 torch_position, int direction, int offset) {
  torch_position = { torch_position.x, torch_position.y + offset };
  entityTransform->m_position = torch_position;
  bool movementInterrupted = is_movement_interrupted(heldEntity->id, entityTransform);

  torch_cauldron_collision(heldEntity->id, entityTransform);

  if (movementInterrupted) {
    torch_position = { torch_position.x, torch_position.y - offset};
    entityTransform->m_position = torch_position;
  } else {
    movementComponent->setMovementDirection(heldEntity, direction);
  }

  return torch_position;
}

// Checks if movement to new position will be interrupted by a Wall entity
bool InputSystem::is_movement_interrupted(int entityId, Transform* entityTransform)
{
    for (auto& it2 : collisionComponent->getmap())
    {
        int otherEntityId = it2.first;
        if (otherEntityId != entityId)
        {
            Entity* otherEntity = objectManager->getEntity(otherEntityId);

            if ((otherEntity->label.compare("Wall") == 0) || (otherEntity->label.compare("Closet") == 0) ||
            (otherEntity->label.compare("Cauldron") == 0))
            {
                Transform *otherEntityTransform = transformComponent->getTransform(otherEntity);

                if (CollisionSystem::AABB(entityTransform, otherEntityTransform))
                {
                    return true;
                }
            }
        }
    }

    return false;
}
void InputSystem::torch_cauldron_collision(int entityId, Transform* entityTransform)
{
  for (auto& it2 : collisionComponent->getmap())
  {
    int otherEntityId = it2.first;
    if (otherEntityId != entityId)
    {
      Entity* otherEntity = objectManager->getEntity(otherEntityId);

      if (otherEntity->label.compare("Cauldron") == 0)
      {
        Transform *otherEntityTransform = transformComponent->getTransform(otherEntity);

        if (CollisionSystem::AABB(entityTransform, otherEntityTransform))
        {
          otherEntity->active = true;
          gameState->num_lit_cauldrons++;
          SoundManager::getInstance().playCauldronLightUp();
        }
      }
    }
  }
}
