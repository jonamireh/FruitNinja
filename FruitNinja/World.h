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
#include "Skybox.h"
#include "DebugShader.h"
#include "ChewyEntity.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720	

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

class World
{
public:
	World();
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
    static void draw_line(glm::vec3 p1, glm::vec3 p2, glm::vec3 color);
	static void draw_point(glm::vec3 p, glm::vec3 color, float radius);
	static void draw_sphere(glm::vec3 center, float radius, glm::vec3 color, float delta);
private:
    std::shared_ptr<Camera> debug_camera;
    std::shared_ptr<Camera> archery_camera;
    std::shared_ptr<Camera> player_camera;
	std::shared_ptr<Skybox> _skybox;
	void init();
	void shootArrows();
    std::shared_ptr<ChewyEntity> chewy;
};

