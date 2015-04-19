#pragma once

#include <map>
#include "Shader.h"
#include <vector>
#include "GameEntity.h"
#include "PhongShader.h"
#include "Camera.h"


class World
{
    bool keys[1024];

public:
	World();
    void update_key_callbacks();
    void update_mouse_callbacks();
	std::map<std::string, std::shared_ptr<Shader>> shaders;
	std::vector<std::shared_ptr<GameEntity>> entities;
	void draw();
private:
	void init();
	std::shared_ptr<Camera> camera;
};

