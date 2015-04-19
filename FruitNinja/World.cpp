#include "World.h"
#include "ChewyEntity.h"
#include "PhongShader.h"

using namespace std;
using namespace glm;

World::World()
{
	init();
}

void World::init()
{
	ChewyEntity chewy = ChewyEntity(MeshSet("chewy.obj"), Material(vec3(0.12, 0.12, 0.06), // Ambient
		vec3(1.0, 0.8, 0.0), // Diffuse
		vec3(0.4, 0.4, 0.14), // Specular
		200.0f));
	entities.push_back(chewy);

	auto_ptr<Shader> phongShader = auto_ptr<Shader>(new PhongShader("phongVert.glsl", "phongFrag.glsl"));
	shaders.insert(pair<string, auto_ptr<PhongShader>>("phongShader", phongShader));
}

void World::draw()
{
	shaders.at("phongShader")->draw(auto_ptr<GameEntity>(&entities.at(0)));
}

void World::update_key_callbacks()
{
}

void World::update_mouse_callbacks()
{
}