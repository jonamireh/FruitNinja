#include "EndScene.h"
#include "World.h"
#include "main.h"
#include "StaticCamera.h"
#include "ObstacleEntity.h"
#include "LightEntity.h"


EndScene::EndScene()
{
	world->entities.clear();

	world->entities.push_back(new ChewyEntity(glm::vec3(0), world->meshes.at("chewy"), world->player_camera, world->player_camera));
	world->entities.back()->list = UNSET_OCTTREE(world->entities.back()->list);
	world->entities.push_back(new ObstacleEntity(glm::vec3(0, 0, -30), world->meshes.at("door_closed")));
	world->entities.back()->setScale(3.f);
	world->entities.push_back(new LightEntity(glm::vec3(8, 7, -30), world->meshes.at("lantern_hook"), 300.f, world->meshes.at("unit_sphere"), glm::vec3(1.0, 0.5, 0.0)));
	world->entities.back()->setRotations(glm::vec3(0, M_PI / 2, 0));
	world->entities.push_back(new LightEntity(glm::vec3(-8, 7, -30), world->meshes.at("lantern_hook"), 300.f, world->meshes.at("unit_sphere"), glm::vec3(1.0, 0.5, 0.0)));
	world->entities.back()->setRotations(glm::vec3(0, M_PI / 2, 0));
	world->entities.back()->list = UNSET_OCTTREE(world->entities.back()->list);
	world->entities.push_back(new ObstacleEntity(glm::vec3(0, 0, 30), world->meshes.at("interior_wall_1x1")));
	world->entities.back()->setScale(3.f);
	world->entities.back()->list = UNSET_OCTTREE(world->entities.back()->list);
	world->entities.push_back(new ObstacleEntity(glm::vec3(0, 6, 30), world->meshes.at("pineapple")));
	world->entities.back()->list = UNSET_OCTTREE(world->entities.back()->list);
	world->entities.push_back(new LightEntity(glm::vec3(8, 7, 40), world->meshes.at("lantern_hook"), 300.f, world->meshes.at("unit_sphere"), glm::vec3(1.0, 0.5, 0.0)));
	world->entities.back()->setRotations(glm::vec3(0, 3 * M_PI / 2, 0));
	world->entities.push_back(new LightEntity(glm::vec3(-8, 7, 40), world->meshes.at("lantern_hook"), 300.f, world->meshes.at("unit_sphere"), glm::vec3(1.0, 0.5, 0.0)));
	world->entities.back()->setRotations(glm::vec3(0, 3 * M_PI / 2, 0));
	world->entities.back()->list = UNSET_OCTTREE(world->entities.back()->list);
	world->player_camera->in_use = false;
	Camera** temp = world->_skybox->_camera;
	world->_skybox = nullptr;
	glClearColor(.01, .01, .01, 0);
}


EndScene::~EndScene()
{
}

void EndScene::update()
{
	static float time = 0.f;
	time += seconds_passed;
	if (time < 9.2)
	{
		world->change_camera(new StaticCamera(glm::vec3(0, 4, 18), glm::vec3(0, 5, -15)));
	}
	else if (time < 10.92f)
	{
		world->change_camera(new StaticCamera(glm::vec3(3, 10, 45), glm::vec3(0, 5, -15)));
	}
	else
	{
		world->entities.at(0)->setPosition(glm::vec3(0, 7.5, 25));
		world->entities.at(5)->setPosition(glm::vec3(0, 17, 30));
		world->change_camera(new StaticCamera(glm::vec3(0, 28, 45), glm::vec3(0, 10, 30)));
	}
	world->entities.at(0)->update();
}

void EndScene::draw()
{
	static float time = 0.f;
	time += seconds_passed;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (time > 10.92f)
	{
		TextShader shdr = TextShader(assetPath + "bonzai.ttf", 108);
		shdr.draw_text_centered("Fruit-tastic!", glm::vec2(screen_width / 2, 50), glm::vec3(212 / 255.f, 175 / 255.f, 55 / 255.f));
	}
	glDisable(GL_BLEND);
}