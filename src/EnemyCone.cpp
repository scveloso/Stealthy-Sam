#include "EnemyCone.hpp"

#include <iostream>
bool EnemyCone::init() {
	death = 0;
	//cone_color= 0;

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
	if (!effect.load_from_file(shader_path("cone.vs.glsl"), shader_path("cone.fs.glsl")))
		return false;

	return true;
}

void EnemyCone::destroy() {
	glDeleteBuffers(1, &mesh.vbo);

	glDeleteShader(effect.vertex);
	glDeleteShader(effect.fragment);
	glDeleteShader(effect.program);
}

void EnemyCone::add_enemy_position(int i, vec2 position){
  float x= position.x;
	float y= position.y;
	enemy_position[i]=x;
	enemy_position[i+1]=y;
}

void EnemyCone::add_enemy_direction(int i, float direction){
	enemy_direction[i] = direction;
	enemy_direction[i+1]=0;
}

void EnemyCone::clear_enemy_position(){
	for (int i=0; i < 10; i++){
		enemy_position[i]=-1000.f;
		enemy_direction[i]=0;
		cone_color[i]= 0;
		death=0;
		// torch_light[i]= -1000.f;
	}
}

void EnemyCone::update_color(int i, float color){
   cone_color[i]= color;
	 cone_color[i+1]= 0;

}

void EnemyCone::draw(const mat3& projection) {
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
  GLint en_position= glGetUniformLocation(effect.program, "enemy_position");
	GLint en_direction= glGetUniformLocation(effect.program, "enemy_direction");
	GLint death_cond= glGetUniformLocation(effect.program, "death_cond");
	GLint color_cond= glGetUniformLocation(effect.program, "color_cond");

	glUniform1i(death_cond, death);
	//glUniform1i(color_cond, cone_color);

	glUniform2fv(en_position, 5, enemy_position);
	glUniform2fv(en_direction, 5, enemy_direction);
	glUniform2fv(color_cond, 5, cone_color);

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
