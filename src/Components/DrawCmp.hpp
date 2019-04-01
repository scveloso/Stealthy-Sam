#pragma once
#include "Cmp.hpp"

class DrawCmp : public Cmp
{
public:
	void add(Entity *entity, const char* file);
	void addFull(Entity *entity, const char* file1, const char* file2, const char* file3
	    , const char* file4, const char* file5, const char* file6, const char* file7
	    , const char* file8, const char* file9);
	std::vector<std::pair<Entity*, Draw *>> getmap();

	~DrawCmp();

private:
	std::vector<std::pair<Entity*, Draw *>> draw_vec;
};
