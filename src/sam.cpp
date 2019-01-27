// Header
#include "sam.hpp"

// internal
#include "turtle.hpp"
#include "fish.hpp"
#include "wall.hpp"

// stlib
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

Texture Sam::sam_texture;

bool Sam::init()
{
	// Load shared texture
	if (!sam_texture.is_valid())
	{
		if (!sam_texture.load_from_file(textures_path("detective.png")))
		{
			fprintf(stderr, "Failed to load sam texture!");
			return false;
		}
	}

	// The position corresponds to the center of the texture
	float wr = sam_texture.width * 0.5f;
	float hr = sam_texture.height * 0.5f;

	TexturedVertex vertices[4];
	vertices[0].position = { -wr, +hr, -0.01f };
	vertices[0].texcoord = { 0.f, 1.f };
	vertices[1].position = { +wr, +hr, -0.01f };
	vertices[1].texcoord = { 1.f, 1.f,  };
	vertices[2].position = { +wr, -hr, -0.01f };
	vertices[2].texcoord = { 1.f, 0.f };
	vertices[3].position = { -wr, -hr, -0.01f };
	vertices[3].texcoord = { 0.f, 0.f };

	// counterclockwise as it's the default opengl front winding direction
	uint16_t indices[] = { 0, 3, 1, 1, 3, 2 };

	// Clearing errors
	gl_flush_errors();

	// Vertex Buffer creation
	glGenBuffers(1, &mesh.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TexturedVertex) * 4, vertices, GL_STATIC_DRAW);

	// Index Buffer creation
	glGenBuffers(1, &mesh.ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * 6, indices, GL_STATIC_DRAW);

	// Vertex Array (Container for Vertex + Index buffer)
	glGenVertexArrays(1, &mesh.vao);
	if (gl_has_errors())
		return false;

	// Loading shaders
	if (!effect.load_from_file(shader_path("textured.vs.glsl"), shader_path("textured.fs.glsl")))
		return false;

	// Setting initial values
	m_scale.x = 0.2f;
	m_scale.y = 0.2f;
	m_is_alive = true;
	m_position = { 50.f, 100.f };
	m_rotation = 0.f;
	m_light_up_countdown_ms = -1.f;

	return true;
	//
	// ///////// --- original impl
	// std::vector<Vertex> vertices;
	// std::vector<uint16_t> indices;
	//
	// // Reads the salmon mesh from a file, which contains a list of vertices and indices
	// FILE* mesh_file = fopen(mesh_path("salmon.mesh"), "r");
	// if (mesh_file == nullptr)
	// 	return false;
	//
	// // Reading vertices and colors
	// size_t num_vertices;
	// fscanf(mesh_file, "%zu\n", &num_vertices);
	// for (size_t i = 0; i < num_vertices; ++i)
	// {
	// 	float x, y, z;
	// 	float _u[3]; // unused
	// 	int r, g, b;
	// 	fscanf(mesh_file, "%f %f %f %f %f %f %d %d %d\n", &x, &y, &z, _u, _u+1, _u+2, &r, &g, &b);
	// 	Vertex vertex;
	// 	vertex.position = { x, y, -z };
	// 	vertex.color = { (float)r / 255, (float)g / 255, (float)b / 255 };
	// 	vertices.push_back(vertex);
	// }
	//
	// // Reading associated indices
	// size_t num_indices;
	// fscanf(mesh_file, "%zu\n", &num_indices);
	// for (size_t i = 0; i < num_indices; ++i)
	// {
	// 	int idx[3];
	// 	fscanf(mesh_file, "%d %d %d\n", idx, idx + 1, idx + 2);
	// 	indices.push_back((uint16_t)idx[0]);
	// 	indices.push_back((uint16_t)idx[1]);
	// 	indices.push_back((uint16_t)idx[2]);
	// }
	//
	// // Done reading
	// fclose(mesh_file);
	//
	// // Clearing errors
	// gl_flush_errors();
	//
	// // Vertex Buffer creation
	// glGenBuffers(1, &mesh.vbo);
	// glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	//
	// // Index Buffer creation
	// glGenBuffers(1, &mesh.ibo);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * indices.size(), indices.data(), GL_STATIC_DRAW);
	//
	// // Vertex Array (Container for Vertex + Index buffer)
	// glGenVertexArrays(1, &mesh.vao);
	// if (gl_has_errors())
	// 	return false;
	//
	// // Loading shaders
	// if (!effect.load_from_file(shader_path("colored.vs.glsl"), shader_path("colored.fs.glsl")))
	// 	return false;
	//
	// // Setting initial values
	// m_scale.x = -35.f;
	// m_scale.y = 35.f;
	// m_is_alive = true;
	// m_num_indices = indices.size();
	// m_position = { 50.f, 100.f };
	// m_rotation = 0.f;
	// m_light_up_countdown_ms = -1.f;
	//
	// return true;
}

// Releases all graphics resources
void Sam::destroy()
{
	glDeleteBuffers(1, &mesh.vbo);
	glDeleteBuffers(1, &mesh.ibo);
	glDeleteBuffers(1, &mesh.vao);

	glDeleteShader(effect.vertex);
	glDeleteShader(effect.fragment);
	glDeleteShader(effect.program);
}

// Called on each frame by World::update()
void Sam::update(float ms, std::vector<Wall> m_walls)
{
	const float SALMON_SPEED = 200.f;
	float step = SALMON_SPEED * (ms / 1000);
	if (m_is_alive)
	{
		vec2 new_position = {m_position.x, m_position.y};

		if (should_move_left)
		{
			new_position.x = new_position.x - step;
		}

		if (should_move_right)
		{
			new_position.x = new_position.x + step;
		}

		if (should_move_up)
		{
			new_position.y = new_position.y + step;
		}

		if (should_move_down)
		{
			new_position.y = new_position.y - step;
		}

		// Check if moving will hit a wall
		auto wall_it = m_walls.begin();
		bool hit_wall = false;
		while (wall_it != m_walls.end())
		{
			if (new_position_collides_with(new_position, *wall_it))
			{
				hit_wall = true;
				wall_it = m_walls.end();
			}
			else
				++wall_it;
		}

		// If won't hit a wall, move
		if (!hit_wall)
		{
			m_position = new_position;
		}
	}
	else
	{
		// If dead we make it face upwards and sink deep down
		set_rotation(3.1415f);
		move({ 0.f, step });
	}

	if (m_light_up_countdown_ms > 0.f)
	{
		m_light_up_countdown_ms -= ms;
	}
	else
	{
		should_be_lit_up = false;
	}
}

// void Salmon::draw(const mat3& projection)
// {
// 	transform_begin();
//
// 	// just need to move the salmon whatever direction, so up is:
// 	// this is just the original position, statically.
// 	// so instead, we need to change it to be the x position.
// 	transform_translate({ m_position.x, m_position.y });
//
// 	// rotation is just whatever radians the salmon is currently updated with
// 	transform_rotate(m_rotation);
//
// 	// scale the texture to the correct size (otherwise tiny)
// 	transform_scale(m_scale);
//
//
// 	transform_end();
//
// 	// Setting shaders
// 	glUseProgram(effect.program);
//
// 	// Enabling alpha channel for textures
// 	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
// 	glEnable(GL_DEPTH_TEST);
//
// 	// Getting uniform locations
// 	GLint transform_uloc = glGetUniformLocation(effect.program, "transform");
// 	GLint color_uloc = glGetUniformLocation(effect.program, "fcolor");
// 	GLint projection_uloc = glGetUniformLocation(effect.program, "projection");
// 	GLint light_up_uloc = glGetUniformLocation(effect.program, "light_up");
//
// 	// Add another uniform location for if it's dead
// 	GLint is_dead_uloc = glGetUniformLocation(effect.program, "is_dead");
//
// 	// Setting vertices and indices
// 	glBindVertexArray(mesh.vao);
// 	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
// 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
//
// 	// Input data location as in the vertex buffer
// 	GLint in_position_loc = glGetAttribLocation(effect.program, "in_position");
// 	GLint in_color_loc = glGetAttribLocation(effect.program, "in_color");
// 	glEnableVertexAttribArray(in_position_loc);
// 	glEnableVertexAttribArray(in_color_loc);
// 	glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
// 	glVertexAttribPointer(in_color_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(vec3));
//
// 	// Setting uniform values to the currently bound program
// 	glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float*)&transform);
//
// 	// !!! Salmon Color
// 	float color[] = { 1.f, 1.f, 1.f };
// 	glUniform3fv(color_uloc, 1, color);
// 	glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float*)&projection);
//
// 	int is_dead = !m_is_alive;
// 	glUniform1iv(is_dead_uloc, 1, &is_dead);
//
// 	int light_up = should_be_lit_up;
// 	glUniform1iv(light_up_uloc, 1, &light_up);
//
//
// 	// Drawing!
// 	glDrawElements(GL_TRIANGLES,(GLsizei)m_num_indices, GL_UNSIGNED_SHORT, nullptr);
// }

void Sam::draw(const mat3& projection)
{
	// Transformation code, see Rendering and Transformation in the template specification for more info
	// Incrementally updates transformation matrix, thus ORDER IS IMPORTANT
	transform_begin();
	transform_translate(m_position);
	transform_rotate(m_rotation);
	transform_scale(m_scale);
	transform_end();

	// Setting shaders
	glUseProgram(effect.program);

	// Enabling alpha channel for textures
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	// Getting uniform locations for glUniform* calls
	GLint transform_uloc = glGetUniformLocation(effect.program, "transform");
	GLint color_uloc = glGetUniformLocation(effect.program, "fcolor");
	GLint projection_uloc = glGetUniformLocation(effect.program, "projection");

	// Setting vertices and indices
	glBindVertexArray(mesh.vao);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);

	// Input data location as in the vertex buffer
	GLint in_position_loc = glGetAttribLocation(effect.program, "in_position");
	GLint in_texcoord_loc = glGetAttribLocation(effect.program, "in_texcoord");
	glEnableVertexAttribArray(in_position_loc);
	glEnableVertexAttribArray(in_texcoord_loc);
	glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)0);
	glVertexAttribPointer(in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)sizeof(vec3));

	// Enabling and binding texture to slot 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sam_texture.id);

	// Setting uniform values to the currently bound program
	glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float*)&transform);
	float color[] = { 1.f, 1.f, 1.f };
	glUniform3fv(color_uloc, 1, color);
	glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float*)&projection);

	// Drawing!
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}

// Simple bounding box collision check,
bool Sam::collides_with(const Turtle& turtle)
{
	float dx = m_position.x - turtle.get_position().x;
	float dy = m_position.y - turtle.get_position().y;
	float d_sq = dx * dx + dy * dy;
	float other_r = std::max(turtle.get_bounding_box().x, turtle.get_bounding_box().y);
	float my_r = std::max(m_scale.x, m_scale.y);
	float r = std::max(other_r, my_r);
	r *= 0.6f;
	if (d_sq < r * r)
		return true;
	return false;
}

bool Sam::collides_with(const Fish& fish)
{
	float dx = m_position.x - fish.get_position().x;
	float dy = m_position.y - fish.get_position().y;
	float d_sq = dx * dx + dy * dy;
	float other_r = std::max(fish.get_bounding_box().x, fish.get_bounding_box().y);
	float my_r = std::max(m_scale.x, m_scale.y);
	float r = std::max(other_r, my_r);
	r *= 0.6f;
	if (d_sq < r * r)
		return true;
	return false;
}

bool Sam::new_position_collides_with(vec2 new_position, const Wall& wall)
{
	float dx = new_position.x - wall.get_position().x;
	float dy = new_position.y - wall.get_position().y;
	float d_sq = dx * dx + dy * dy;
	float other_r = std::max(wall.get_bounding_box().x, wall.get_bounding_box().y);
	float my_r = std::max(m_scale.x, m_scale.y);
	float r = std::max(other_r, my_r);
	r *= 0.6f;
	if (d_sq < r * r)
		return true;
	return false;
}

vec2 Sam::get_position()const
{
	return m_position;
}

void Sam::move(vec2 off)
{
	m_position.x += off.x; m_position.y += off.y;
}

void Sam::set_rotation(float radians)
{
	m_rotation = radians;
}

bool Sam::is_alive()const
{
	return m_is_alive;
}

// Called when the salmon collides with a turtle
void Sam::kill()
{
	m_is_alive = false;
}

// Called when the salmon collides with a fish
void Sam::light_up()
{
	should_be_lit_up = true;
	m_light_up_countdown_ms = 1500.f;
}

void Sam::should_move(int direction, bool should)
{
	switch (direction)
	{
	case 1: // left
		should_move_left = should;
		break;
	case 2:
		should_move_right = should;
		break;
	case 3:
		should_move_up = should;
		break;
	case 4:
		should_move_down = should;
		break;
	default:
		std::cout << "failed to move" << std::endl;
	}
}
