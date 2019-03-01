#include <Components/GameStateCmp.hpp>
#include "DrawSystem.hpp"
#include "Components/Cmp.hpp"
#include "common.hpp"

void DrawSystem::init(ObjectManager om, DrawCmp dc, TransformCmp tc, GameStateCmp* gameStateCmp)
{
	objectManager = om;
	drawComponent = dc;
	transformComponent = tc;
	gameState = gameStateCmp;
}

bool DrawSystem::setup()
{
	for (auto& it : drawComponent.getmap())
	{
		Entity *entity = objectManager.getEntity(it.first);
		Draw *draw = it.second;

		transformComponent.setHeight(entity, draw->texture.height);
		transformComponent.setWidth(entity, draw->texture.width);

		// The position corresponds to the center of the texture
		float wr = draw->texture.width * 0.5f;
		float hr = draw->texture.height * 0.5f;

		TexturedVertex vertices[4];
		vertices[0].position = { -wr, +hr, -0.02f };
		vertices[0].texcoord = { 0.f, 1.f };
		vertices[1].position = { +wr, +hr, -0.02f };
		vertices[1].texcoord = { 1.f, 1.f };
		vertices[2].position = { +wr, -hr, -0.02f };
		vertices[2].texcoord = { 1.f, 0.f };
		vertices[3].position = { -wr, -hr, -0.02f };
		vertices[3].texcoord = { 0.f, 0.f };

		// counterclockwise as it's the default opengl front winding direction
		uint16_t indices[] = { 0, 3, 1, 1, 3, 2 };

		// Clearing errors
		gl_flush_errors();

		// Vertex Buffer creation
		glGenBuffers(1, &draw->mesh.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, draw->mesh.vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(TexturedVertex) * 4, vertices, GL_STATIC_DRAW);

		// Index Buffer creation
		glGenBuffers(1, &draw->mesh.ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, draw->mesh.ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * 6, indices, GL_STATIC_DRAW);

		// Vertex Array (Container for Vertex + Index buffer)
		glGenVertexArrays(1, &draw->mesh.vao);
		if (gl_has_errors())
			return false;

		// Loading shaders
		if (!draw->effect.load_from_file(shader_path("textured.vs.glsl"), shader_path("textured.fs.glsl")))
			return false;
	}

	return true;
}

void DrawSystem::update(const mat3 projection)
{
	for (auto& it : drawComponent.getmap())
	{
		Entity *entity = objectManager.getEntity(it.first);
		Draw *draw = it.second;

		// Don't draw inactive entities
		if (transformComponent.getTransform(objectManager.getEntity(entity->id))->visible == true) {

			if (!entity->active) {
				continue;
			}

			draw->transform_begin();
			draw->transform_translate(transformComponent.getTransform(entity)->m_position);
			draw->transform_rotate(transformComponent.getTransform(entity)->m_rotation);
			draw->transform_scale(transformComponent.getTransform(entity)->m_scale);
			draw->transform_end();

			if (entity->label == "Player") {
				Transform* samTransform = transformComponent.getTransform(entity);
			  gameState->sam_position = samTransform->m_position;

				// If sam is dead, we should also rotate him sideways:
				if (!gameState->sam_is_alive)
				{
					draw->transform_rotate(1.5708f);
				}
			}

			// if (entity->label == "Enemy"){
			// 	en_position= transformComponent.getTransform(entity)->m_position;
			// 	en_direction= transformComponent.getTransform(entity)->direction;
			// }

			// Setting shaders
			glUseProgram(draw->effect.program);

			// Enabling alpha channel for textures
			glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDisable(GL_DEPTH_TEST);

			// Getting uniform locations for glUniform* calls
			GLint transform_uloc = glGetUniformLocation(draw->effect.program, "transform");
			GLint color_uloc = glGetUniformLocation(draw->effect.program, "fcolor");
			GLint projection_uloc = glGetUniformLocation(draw->effect.program, "projection");

			// Setting vertices and indices
			glBindVertexArray(draw->mesh.vao);
			glBindBuffer(GL_ARRAY_BUFFER, draw->mesh.vbo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, draw->mesh.ibo);

			// Input data location as in the vertex buffer
			GLint in_position_loc = glGetAttribLocation(draw->effect.program, "in_position");
			GLint in_texcoord_loc = glGetAttribLocation(draw->effect.program, "in_texcoord");
			glEnableVertexAttribArray(in_position_loc);
			glEnableVertexAttribArray(in_texcoord_loc);
			glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)0);
			glVertexAttribPointer(in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)sizeof(vec3));

			// Enabling and binding texture to slot 0
			glActiveTexture(GL_TEXTURE0);
			//gets first texture need to make it work for changing textures
			glBindTexture(GL_TEXTURE_2D, draw->texture.id);

			// Setting uniform values to the currently bound program
			glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float*)&draw->transform);
			float color[] = { 1.f, 1.f, 1.f };
			glUniform3fv(color_uloc, 1, color);
			glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float*)&projection);

			// Drawing!
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

			//printf("DRAWING\n");
		}
	}
}
