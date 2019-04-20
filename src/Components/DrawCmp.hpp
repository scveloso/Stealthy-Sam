#pragma once
#include "Cmp.hpp"

class DrawCmp : public Cmp
{
public:
	void add(Entity *entity, const char* file);
	void remove(Entity *entity);

	void addGhost(Entity *entity, const char* default_file, const char* chase_file);

	void addFull(Entity *entity, const char* file1, const char* file2, const char* file3
	    , const char* file4, const char* file5, const char* file6, const char* file7
	    , const char* file8, const char* file9);
	std::vector<std::pair<Entity*, Draw *>> getmap();
    Draw* getDrawByLabel(std::string label);

	~DrawCmp();

private:
	std::vector<std::pair<Entity*, Draw *>> draw_vec;
};
