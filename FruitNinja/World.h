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

class ArcheryCamera;
extern bool keys[1024];
extern float seconds_passed;
extern Timer timer;
extern float x_offset;
extern float y_offset;
extern float screen_width;
extern float screen_height;

class World
{
public:
	World();
    void update_key_callbacks();
    void update_mouse_callbacks();
	void update();
	std::map<std::string, std::shared_ptr<Shader>> shaders;
	std::vector<std::shared_ptr<GameEntity>> entities;
	void draw();
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
    static void mouse_callback(GLFWwindow* window, double x_position, double y_position);
    void change_camera();
private:
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Camera> debug_camera;
    std::shared_ptr<Camera> archery_camera;
    std::shared_ptr<Camera> player_camera;

	void init();
};

