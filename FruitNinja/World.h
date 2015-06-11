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
#include "LoadingScreen.h"

#define DEBUG_MODE true

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define GRAVITY 33
#define MAX_TIME_STEP (1.0f / 25.0f)
#define PLAYER_FOV 45.f
#define PLAYER_NEAR 0.1f
#define PLAYER_FAR 1000.f
#define GUARD_NEAR 0.1f

#define MAX_HEALTH 4
#include "fmodex/fmod.hpp"
#include "EndScene.h"

class ProjectileEntity;
class ArcheryCamera;
class GuardEntity;
class GuardPuppeteer;
class DeferredShader;

const string assetPath = "assets/";
const string level_path = "levels/";
extern bool keys[1024];
extern float actual_seconds_passed;
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
extern int arrow_count;
extern int current_courtyard;
extern int health;
extern glm::vec3 directional_light;
extern float wall_height;

enum GameState{
	HIDDEN,
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
    std::map<std::string, Shader*> shaders;
	std::vector<GameEntity*> entities;
	void draw();
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
    static void mouse_callback(GLFWwindow* window, double x_position, double y_position);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    void change_camera();
	void change_camera(Camera* camera);
    std::map<std::string, MeshSet*> meshes;
	void enable_debugging();
	void flip_controls();
	void cancel_cinematic();
	void skip_level();
    static void draw_line(glm::vec3 p1, glm::vec3 p2, glm::vec3 color);
	static void draw_point(glm::vec3 p, glm::vec3 color, float radius);
	static void draw_sphere(glm::vec3 center, float radius, glm::vec3 color, float delta);
	static void draw_box(EntityBox* box, glm::vec3 color);

	void setup_next_courtyard(bool setup_cin_cam = true);
	void push_courtyards(int current_courtyard);
	void lose_condition();
	void delayed_lose_condition();
	void setup_cinematic_camera(string file_path, bool setup_cin_cam);
    void setup_level(string file_path, bool animate_elements = false);
    void load_button(string file_path);
    void setup_moving_platform(string file_path);
	void convert_to_collectible(ProjectileEntity* p);
	void zoom_on_guard(GameEntity* g);
	void set_chewy_light_distance(float dist, float le_hv_length);

	GameState getState();

	LoadingScreen* loading_screen = nullptr;

	static void resize_window(GLFWwindow* window, int w, int h);
	void addExplosion(glm::vec3 pos);
	void enableFireArrows();

	EndScene* endScene = nullptr;
	DebugCamera* debug_camera;
	ArcheryCamera* archery_camera;
	PlayerCamera* player_camera;
	CinematicCamera* cinematic_camera;
	Skybox* _skybox;
private:
    
	
	HUD* hud;
	void init();
    void setup_token(char obj_to_place, glm::vec3 file_index);
    void setup_guard(string file_path);
    void shootArrows();
    ChewyEntity* chewy;

	bool run_cinematic_camera = true;

	GameState state;
	vector<GameEntity*> should_del;

	float starting_platform_height;
	int num_doors = 0;
	GuardPuppeteer* _puppeteer = nullptr;
	void set_puppeteer(int courtyard);
	DeferredShader* defShader;

	bool _fiery_arrows = false;
};

