#pragma once

#include "common.hpp"
#include "sam.hpp"
#include "background.hpp"
#include "wall.hpp"
#include "enemy.hpp"
#include "closet.hpp"
#include "door.hpp"
#include "room.hpp"

// stlib
#include <vector>

class Room
{
  public:
  	virtual bool init(vec2 screen, Sam* sam) = 0;

  	// Steps the game ahead by ms milliseconds
    // Returns an update_action enumeration in update_action.cpp
  	virtual int update(float ms, vec2 screen) = 0;

  	// Renders our scene
  	void draw(const mat3& projection);

    bool spawn_enemy(float posx, float posy, float patrol_x, float patrol_y);

    void on_key(int key, int action, int mod);

    bool is_initialized();

  protected:
    Sam* m_sam;
    bool m_is_initialized = false;
    std::vector<Enemy> m_enemies;
  	std::vector<Wall> m_walls;
  	std::vector<Closet> m_closets;
    std::vector<Door> m_doors;
};
