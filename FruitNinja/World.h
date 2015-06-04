#pragma once

#include <map>
#include "Shader.h"
#include <vector>
#include "GameEntity.h"
#include "PhongShader.h"
#include "Camera.h"
#include "Timer.h"
#include "DebugCamera.h"
#include "PlayerCamera.h"
#include "CinematicCamera.h"
#include "Skybox.h"
#include "DebugShader.h"
#include "ChewyEntity.h"
#include "HUD.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define GRAVITY 33
#define PLAYER_FOV 45.f
#define GUARD_FOV 45.f
#define PLAYER_NEAR 0.1f
#define PLAYER_FAR 1000.f
#define GUARD_NEAR 0.1f
#define GUARD_FAR 30.f
//#define PLAYER_DIST (PLAYER_FAR - PLAYER_NEAR)
//#define GUARD_DIST (GUARD_FAR - GUARD_NEAR)

class ArcheryCamera;
const string assetPath = "assets/";
extern bool keys[1024];
extern float seconds_passed;
extern float x_offset;
extern float y_offset;
extern float screen_width;
extern float screen_height;
extern bool time_stopped;
extern glm::mat4 projection;
extern glm::mat4 guard_projection;
extern float game_speed;
extern float bow_strength;

enum GameState{
	LEVEL1,
	SPOTTED,
	GAMEOVER
};

class World
{
public:
	World();
	~World();
	static void stop_time();
	void update_key_callbacks();
	void update();
    static void scroll_callback(GLFWwindow* window, double x_pos, double y_pos);
    std::map<std::string, std::shared_ptr<Shader>> shaders;
	std::vector<std::shared_ptr<GameEntity>> entities;
	void draw();
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
    static void mouse_callback(GLFWwindow* window, double x_position, double y_position);
    void change_camera();
    std::map<std::string, std::shared_ptr<MeshSet>> meshes;
	void enable_debugging();
	void cancel_cinematic();
    static void draw_line(glm::vec3 p1, glm::vec3 p2, glm::vec3 color);
	static void draw_point(glm::vec3 p, glm::vec3 color, float radius);
	static void draw_sphere(glm::vec3 center, float radius, glm::vec3 color, float delta);
	static void draw_box(std::shared_ptr<EntityBox> box, glm::vec3 color);

    void setup_next_courtyard();
    void setup_cinematic_camera(string file_path);
private:
    std::shared_ptr<DebugCamera> debug_camera;
    std::shared_ptr<ArcheryCamera> archery_camera;
    std::shared_ptr<PlayerCamera> player_camera;
	std::shared_ptr<CinematicCamera> cinematic_camera;
	std::shared_ptr<Skybox> _skybox;
	HUD hud;
	void init();
    int current_courtyard = 1;
    void setup_level(string file_path);
    void setup_token(char obj_to_place, glm::vec3 file_index);
    void setup_guard(string file_path);
    void shootArrows();
    std::shared_ptr<ChewyEntity> chewy;

	bool cinematic_runthrough = true;

	GameState state;
};

