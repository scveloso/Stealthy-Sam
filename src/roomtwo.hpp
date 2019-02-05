#pragma once

#include "room.hpp"
#include "common.hpp"

class RoomTwo : public Room
{
  public:
    bool init(vec2 screen, Sam* sam);

  	// Steps the game ahead by ms milliseconds
    // Returns an update_action enumeration in update_action.cpp
  	int update(float ms, vec2 screen);
};
