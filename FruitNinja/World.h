#pragma once

#include <map>
#include "Shader.h"
#include <vector>
#include "GameEntity.h"
#include "PhongShader.h"


class World
{
    bool keys[1024];

public:
	World();
    void update_key_callbacks();
    void update_mouse_callbacks();
	std::map<std::string, std::auto_ptr<Shader>> shaders;
	std::vector<GameEntity> entities;
	void draw();
private:
	void init();
};

