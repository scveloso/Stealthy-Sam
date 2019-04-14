#include "light.hpp"

#include <iostream>

bool Light::init(){
  death = 0;
  tutorial = 0;

  static const GLfloat screen_vertex_buffer_data[] = {
		-1.05f, -1.05f, 0.0f,
		1.05f, -1.05f, 0.0f,
		-1.05f,  1.05f, 0.0f,
		-1.05f,  1.05f, 0.0f,
		1.05f, -1.05f, 0.0f,
		1.05f,  1.05f, 0.0f,
	};

	// Clearing errors
	gl_flush_errors();

	// Vertex Buffer creation
	glGenBuffers(1, &mesh.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screen_vertex_buffer_data), screen_vertex_buffer_data, GL_STATIC_DRAW);

	if (gl_has_errors())
		return false;

	// Loading shaders
	if (!effect.load_from_file(shader_path("light.vs.glsl"), shader_path("light.fs.glsl")))
		return false;

	return true;
}

void Light::destroy() {
	glDeleteBuffers(1, &mesh.vbo);

	glDeleteShader(effect.vertex);
	glDeleteShader(effect.fragment);
	glDeleteShader(effect.program);
}

void Light::add_position(vec2 position){
	circle_light_position = position;
}

void Light::clearTorchPositions() {
	for (int i=0; i < 10; i++){
		torch_light[i]= -1000.f;
	}
}

void Light::addTorchPosition(int i, vec2 position) {
  float x= position.x;
	float y= position.y;
	torch_light[i]=x;
	torch_light[i+1]=y;
}

void Light::restart() {
	death = 0;
	tutorial = 0;
}

void Light::clearSamLight() {
	circle_light_position = { -1000.f, -1000.f };
}

void Light::draw(const mat3& projection) {
	// Enabling alpha channel for textures
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_DEPTH_TEST);

	// Setting shaders
	glUseProgram(effect.program);

	// Set screen_texture sampling to texture unit 0
	// Set clock
	GLuint screen_text_uloc = glGetUniformLocation(effect.program, "screen_texture");
	GLuint time_uloc = glGetUniformLocation(effect.program, "time");
	// GLuint dead_timer_uloc = glGetUniformLocation(effect.program, "dead_timer");
	GLint s_position= glGetUniformLocation(effect.program, "circle_light_position");
	GLint tor_position= glGetUniformLocation(effect.program, "torch_light");

	GLint death_cond= glGetUniformLocation(effect.program, "death_cond");
	GLint tutorial_cond= glGetUniformLocation(effect.program, "tutorial_cond");

	glUniform1i(death_cond, death);
	glUniform1i(tutorial_cond, tutorial);
	glUniform2fv(tor_position, 5, torch_light);


	glUniform2f(s_position, circle_light_position.x, circle_light_position.y);
	glUniform1i(screen_text_uloc, 0);
	glUniform1f(time_uloc, (float)(glfwGetTime() * 10.0f));
	// glUniform1f(dead_timer_uloc, (m_dead_time > 0) ? (float)((glfwGetTime() - m_dead_time) * 10.0f) : -1);

	// Draw the screen texture on the quad geometry
	// Setting vertices
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

	// Bind to attribute 0 (in_position) as in the vertex shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Draw
	glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles
	glDisableVertexAttribArray(0);
}
