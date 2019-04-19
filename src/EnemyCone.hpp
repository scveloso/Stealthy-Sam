#pragma once

#include "common.hpp"
//#include "EnemyCmp.hpp"

class EnemyCone : public Renderable
{
public:
  bool init();
	void destroy();
	void draw(const mat3& projection);
	// void restart();
  void add_enemy_position(int i, vec2 position);
	// left =1; right =2; up =3; down=4; for direction
	void add_enemy_direction(int i, float direction);
  void clear_enemy_position();
  void update_color(int i, float color);
  int death;
  GLfloat cone_color[10];
private:
  GLfloat enemy_position[10];
	GLfloat enemy_direction[10];
};
