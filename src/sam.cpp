// Header
#include "sam.hpp"

// internal
#include "wall.hpp"
#include "constants.hpp"

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
		if (!sam_texture.load_from_file(sam_textures_path("Run_01.png")))
		{
			fprintf(stderr, "Failed to load sam texture!");
			return false;
		}
	}

	// The position corresponds to the center of the texture
	//float wr = sam_texture.width * 7.f;
	//float hr = sam_texture.height * 7.f;

	float wr = sam_texture.width * 0.5;
	float hr = sam_texture.height * 0.5;

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
	m_scale.x = 1.5;
	m_scale.y = 1.5;
	m_is_alive = true;
	m_position = { 50.f, 100.f };
	m_rotation = 0.f;
	direction = NO_DIRECTION;

	return true;
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
void Sam::update(float ms, std::vector<Wall> m_walls, vec2 screen)
{
	const float SAM_SPEED = 200.f;
	float step = SAM_SPEED * (ms / 1000);
	if (m_is_alive && !m_is_hidden)
	{
		vec2 new_position = {m_position.x, m_position.y};

		if (direction % LEFT == 0)
		{
			new_position.x = new_position.x - step;
			sam_texture.load_from_file(sam_textures_path("leftRun_02.png"));
			if (is_movement_interrupted(new_position, m_walls, screen))
			{
			    new_position.x = new_position.x + step;
			}
		}

		if (direction % RIGHT == 0)
		{
			new_position.x = new_position.x + step;
			sam_texture.load_from_file(sam_textures_path("Run_02.png"));
			if (is_movement_interrupted(new_position, m_walls, screen))
            {
                new_position.x = new_position.x - step;
            }
		}

		if (direction % DOWN == 0)
		{
			new_position.y = new_position.y + step;
			if (is_movement_interrupted(new_position, m_walls, screen))
            {
                new_position.y = new_position.y - step;
            }
		}

		if (direction % UP == 0)
		{
			new_position.y = new_position.y - step;
			if (is_movement_interrupted(new_position, m_walls, screen))
            {
                new_position.y = new_position.y + step;
            }
		}

		if (direction % LEFT != 0 && direction % RIGHT != 0 && direction % UP != 0 && direction % DOWN != 0)
		{
			sam_texture.load_from_file(sam_textures_path("Run_02.png"));
		}

        m_position = new_position;

	}
}

bool Sam::is_movement_interrupted(vec2 new_position, std::vector<Wall> m_walls, vec2 screen){
    auto wall_it = m_walls.begin();
    while (wall_it != m_walls.end())
    {
        if (collides_with_wall(new_position, *wall_it) ||
						collides_with_screen_edge(new_position, screen))
        {
            return true;
        }
        else
            ++wall_it;
    }
    return false;
}

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

// Return true if new position will collide with the given wall, false otherwise
bool Sam::collides_with_wall(vec2 new_position, const Wall& wall)
{
	float hw = get_half_width();
	float hh = get_half_height();

	// Grab sam's edges
	// (Note: we don't want the edges of the texture but rather the edges of the actual sprite)
	float sam_x1 = (new_position.x - (hw * 0.25));
	float sam_x2 = (new_position.x + (hw * 0.25));
	float sam_y1 = new_position.y - (hh * 0.75);
	float sam_y2 = new_position.y + (hh * 0.5);

	// Grab wall's edges
	float wall_x1 = wall.get_left_edge();
	float wall_x2 = wall.get_right_edge();
	float wall_y1 = wall.get_top_edge();
	float wall_y2 = wall.get_bottom_edge();

	// Collision case 1: top right corner will be inside the wall
	if (sam_x2 >= wall_x1 && sam_x2 <= wall_x2 &&
			sam_y1 >= wall_y1 && sam_y1 <= wall_y2)
	{
		return true;
	}

	// Collision case 2: top left corner will be inside the wall
	if (sam_x1 >= wall_x1 && sam_x1 <= wall_x2 &&
			sam_y1 >= wall_y1 && sam_y1 <= wall_y2)
	{
		return true;
	}

	// Collision case 3: bottom right corner will be inside the wall
	if (sam_x2 >= wall_x1 && sam_x2 <= wall_x2 &&
			sam_y2 >= wall_y1 && sam_y2 <= wall_y2)
	{
		return true;
	}

	// Collision case 4: bottom left corner will be inside the wall
	if (sam_x1 >= wall_x1 && sam_x1 <= wall_x2 &&
			sam_y2 >= wall_y1 && sam_y2 <= wall_y2)
	{
		return true;
	}

	// Collision case 5: prevent fat sam from going through thin wall from the bottom
	if (sam_x1 <= wall_x1 && sam_x2 >= wall_x2 &&
			sam_y1 <= wall_y2 && sam_y2 >= wall_y2)
	{
		return true;
	}

	// Collision case 6: prevent fat sam from going through thin wall from the top
	if (sam_x1 <= wall_x1 && sam_x2 >= wall_x2 &&
			sam_y2 >= wall_y1 && sam_y1 <= wall_y1)
	{
		return true;
	}

	// Collision case 7: prevent tall sam from going through short wall from the left
	if (sam_y1 <= wall_y1 && sam_y2 >= wall_y2 &&
			sam_x2 >= wall_x1 && sam_x1 <= wall_x1)
	{
		return true;
	}

	// Collision case 8: prevent tall sam from going through short wall from the right
	if (sam_y1 <= wall_y1 && sam_y2 >= wall_y2 &&
			sam_x1 <= wall_x2 && sam_x2 >= wall_x2)
	{
		return true;
	}

	return false;
}

// Return true if new position will collide with the screen's edges, false otherwise
bool Sam::collides_with_screen_edge(vec2 new_position, vec2 screen)
{
	float hw = get_half_width();
	float hh = get_half_height();

	// Grab sam's edges
	// (Note: we don't want the edges of the texture but rather the edges of the actual sprite)
	float sam_x1 = (new_position.x - (hw * 0.6));
	float sam_x2 = (new_position.x + (hw * 0.6));
	float sam_y1 = new_position.y - (hh * 1.2);
	float sam_y2 = new_position.y + (hh);

	return (sam_x2 >= screen.x || sam_x1 <= 0 || sam_y1 <= 0 || sam_y2 >= screen.y);
}

float Sam::get_half_width()const
{
	return (m_scale.x) * (sam_texture.width / 2);
}

float Sam::get_half_height()const
{
	return (m_scale.y) * (sam_texture.height / 2);
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

// Called when the sam collides with an enemy
void Sam::kill()
{
	m_is_alive = false;
}


void Sam::interact_in_front(std::vector<Closet> closets) {

	// Check if we're already hidden, if so, unhide:
	if (m_is_hidden) {
		m_position.x = m_previous_location.x;
		m_position.y = m_previous_location.y;
		m_is_hidden = false;
		return;
	}


	// right now assume that we're interacting with the right:

	vec2 position_to_check;

	if (true)
	{ // interact with the right
		position_to_check = { m_position.x + get_half_width(), m_position.y };
	}

	for (auto& closet : closets)
	{
		bool collision = collides_with(position_to_check, m_scale, closet.get_position(), closet.get_bounding_box());
		std::cout << "Interacted with closet: " << collision << std::endl;

		if (collision)
		{
			// store our current position to restore later and put us off the map
			m_previous_location = { m_position.x, m_position.y };
			m_is_hidden = true;
			m_position.x = 10000;
			m_position.y = 10000;
		}
	}
}
