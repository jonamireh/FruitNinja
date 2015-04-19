#include "World.h"
#include "ChewyEntity.h"
#include "PhongShader.h"
#include "DebugCamera.h"

using namespace std;
using namespace glm;

World::World()
{
	init();
}

void World::init()
{
	camera = shared_ptr<Camera>(new DebugCamera());
	shared_ptr<GameEntity> chewy(new ChewyEntity(MeshSet("chewy.obj"), Material(vec3(0.12, 0.12, 0.06), // Ambient
		vec3(1.0, 0.8, 0.0), // Diffuse
		vec3(0.4, 0.4, 0.14), // Specular
		200.0f)));
	entities.push_back(chewy);

	shared_ptr<Shader> phongShader(new PhongShader("phongVert.glsl", "phongFrag.glsl"));
	shaders.insert(pair<string, shared_ptr<Shader>>("phongShader", phongShader));
}

void World::draw()
{
	shaders.at("phongShader")->draw(camera->getViewMatrix(), entities.at(0));
}

void World::update_key_callbacks()
{
}

void World::update_mouse_callbacks()
{
}