#pragma once

// stlib
#include <fstream> // stdout, stderr..

// glfw
#define NOMINMAX
#include <gl3w.h>
#include <GLFW/glfw3.h>


#include "btBulletDynamicsCommon.h"

// JSON
#include <../ext/json/json.hpp>

// Simple utility macros to avoid mistyping directory name, name has to be a string literal
// audio_path("audio.ogg") -> data/audio/audio.ogg
// Get defintion of PROJECT_SOURCE_DIR from:
#include "project_path.hpp"

#define shader_path(name) PROJECT_SOURCE_DIR "./shaders/" name

#define data_path PROJECT_SOURCE_DIR "./data"
#define textures_path(name)  data_path "/textures/" name
#define audio_path(name) data_path  "/audio/" name
#define mesh_path(name) data_path  "/meshes/" name
#define map_path(name) data_path "/maps/" name
#define saves_path(name) data_path "/saves/" name
#define sam_default_path(name) data_path "/textures/sam/" name

// Not much math is needed and there are already way too many libraries linked (:
// If you want to do some overloads..
struct vec2 { float x, y; };
struct vec3 { float x, y, z; };
struct mat3 { vec3 c0, c1, c2; };

// Utility functions
float dot(vec2 l, vec2 r);
float dot(vec3 l, vec3 r);
mat3  mul(const mat3& l, const mat3& r);
vec2  normalize(vec2 v);

// OpenGL utilities
// cleans error buffer
void gl_flush_errors();
bool gl_has_errors();

//Spline stuff
vec2 crSpline(float t, std::vector<vec2>, int i);

// Single Vertex Buffer element for non-textured meshes (colored.vs.glsl)
struct Vertex
{
	vec3 position;
	vec3 color;
};

// Single Vertex Buffer element for textured sprites (textured.vs.glsl)
struct TexturedVertex
{
	vec3 position;
	vec2 texcoord;
};

// Texture wrapper
struct Texture
{
	Texture();
	~Texture();

	GLuint id;
	GLuint depth_render_buffer_id;
	int width;
	int height;

	// Loads texture from file specified by path
	bool load_from_file(const char* path);
	// Screen texture
	bool create_from_screen(GLFWwindow const * const window);
	bool is_valid()const; // True if texture is valid
};

// A Mesh is a collection of a VertexBuffer and an IndexBuffer. A VAO
// represents a Vertex Array Object and is the container for 1 or more Vertex Buffers and
// an Index Buffer
struct Mesh
{
	GLuint vao;
	GLuint vbo;
	GLuint ibo;
};

// Container for Vertex and Fragment shader, which are then put(linked) together in a
// single program that is then bound to the pipeline.
struct Effect
{
	bool load_from_file(const char* vs_path, const char* fs_path);
	void release();

	GLuint vertex;
	GLuint fragment;
	GLuint program;
};

// Helper container for all the information we need when rendering an object together
// with its transform.
struct Renderable
{
	Mesh mesh;
	Effect effect;
	mat3 transform;

	// projection contains the orthographic projection matrix. As every Renderable::draw()
	// renders itself it needs it to correctly bind it to its shader.
	void draw(const mat3& projection);

	// gl Immediate mode equivalent, see the Rendering and Transformations section in the
	// specification pdf
	void transform_begin();
	void transform_scale(vec2 scale);
	void transform_rotate(float radians);
	void transform_translate(vec2 pos);
	void transform_end();
};

const vec2 TEXT_POSITION = { 300, 150 };


const std::string USE_P_RESTART = "Textbox.RtoRestart";
const std::string TUTORIAL_SCREEN = "Tutorial_Screen";
const std::string PAUSE_SCREEN = "Pause_Screen";
const std::string VICTORY_SCREEN = "Victory_Screen";
const std::string MAIN_MENU = "Main_Menu";
const std::string T1_WELCOME = "T1_Welcome";
const std::string BOSS_DOOR_TEXT = "Boss_Door_Text";
const std::string BOSS_TEXT = "Boss_Text";
const std::string INTRO_TEXT = "Intro_Text";

const std::string GAME_SAVED_ALERT = "Game_Saved";
const std::string GAME_LOADED_ALERT = "Game_Loaded";
const std::string GAME_DEATH_ALERT = "Game_Death";

const std::string ROOM_ONE_GUID = "ROOM_ONE";
const std::string ROOM_TWO_GUID = "ROOM_TWO";
const std::string ROOM_THREE_GUID = "ROOM_THREE";
const std::string ROOM_FOUR_GUID = "ROOM_FOUR";
const std::string TUTORIAL_LEVEL_GUID = "TUTORIAL_LEVEL";

const std::string ENEMY_LABEL = "Enemy.Ghost";
const std::string TORCH_LABEL = "Torch";
const std::string CAULDRON_LABEL = "Cauldron";
const std::string SMOKE_LABEL = "Smoke";
const std::string BOSS_GUID = "Enemy.Boss";

const std::string SAM_GUID = "Player";
