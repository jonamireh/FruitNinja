#pragma once

#include <map>
#include "Shader.h"
#include <vector>
#include "GameEntity.h"
#include "PhongShader.h"
#include "Camera.h"
#include "Timer.h"

extern bool keys[1024];
extern double seconds_passed;

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
private:
    Timer timer;
    std::shared_ptr<Camera> camera;

	void init();
};

