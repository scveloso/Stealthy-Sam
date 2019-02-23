#include "water.hpp"

#include <iostream>

bool Water::init() {
	m_dead_time = -1;

	// Since we are not going to apply transformation to this screen geometry
	// The coordinates are set to fill the standard openGL window [-1, -1 .. 1, 1]
	// Make the size slightly larger then the screen to crop the boundary
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
	if (!effect.load_from_file(shader_path("water.vs.glsl"), shader_path("water.fs.glsl")))
		return false;

	return true;
}

void Water::destroy() {
	glDeleteBuffers(1, &mesh.vbo);

	glDeleteShader(effect.vertex);
	glDeleteShader(effect.fragment);
	glDeleteShader(effect.program);
}

void Water::set_salmon_dead() {
	m_dead_time = glfwGetTime();
}

void Water::add_position(vec2 position){
	sam_position= position;
}

void Water::add_text(vec2 position){
	text_position= position;
}
void Water::add_key(vec2 position){
	key_position= position;
}

void Water::reset_salmon_dead_time() {
	m_dead_time = -1;
}

float Water::get_salmon_dead_time() const {
	return glfwGetTime() - m_dead_time;
}

void Water::draw(const mat3& projection) {
	// Enabling alpha channel for textures
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	// Setting shaders
	glUseProgram(effect.program);


	// Set screen_texture sampling to texture unit 0
	// Set clock
	GLuint screen_text_uloc = glGetUniformLocation(effect.program, "screen_texture");
	GLuint time_uloc = glGetUniformLocation(effect.program, "time");
	GLuint dead_timer_uloc = glGetUniformLocation(effect.program, "dead_timer");
	GLint s_position= glGetUniformLocation(effect.program, "sam_position");
	GLint t_position= glGetUniformLocation(effect.program, "text_position");
	GLint e_position= glGetUniformLocation(effect.program, "e_position");
	GLint text_cond= glGetUniformLocation(effect.program, "text_cond");
	GLint key_cond= glGetUniformLocation(effect.program, "key_cond");

	glUniform1i(text_cond, removeText);
	glUniform1i(key_cond, removeKey);
	glUniform2f(t_position, text_position.x-15.f, text_position.y-812.f );
	glUniform2f(e_position, key_position.x-27.f, key_position.y-812.f );
	glUniform2f(s_position, sam_position.x-25.f, sam_position.y-820.f );
	glUniform1i(screen_text_uloc, 0);
	glUniform1f(time_uloc, (float)(glfwGetTime() * 10.0f));
	glUniform1f(dead_timer_uloc, (m_dead_time > 0) ? (float)((glfwGetTime() - m_dead_time) * 10.0f) : -1);

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
