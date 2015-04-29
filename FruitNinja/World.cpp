#include "World.h"
#include "ChewyEntity.h"
#include "PhongShader.h"
#include "DebugCamera.h"
#include "Global.h"
#include "ArcheryCamera.h"
#include "GuardEntity.h"
#include "ProjectileEntity.h"
#include "ObstacleEntity.h"
#include "OctTree.h"

using namespace std;
using namespace glm;

bool keys[1024];
float seconds_passed = 0;
float x_offset;
float y_offset;
float screen_width = 1280;
float screen_height = 720;

static std::shared_ptr<Camera> camera;


World::World()
{
	init();
    x_offset = 0;
    y_offset = 0;
}

void World::init()
{
	debug_camera = shared_ptr<Camera>(new DebugCamera());
    player_camera = shared_ptr<Camera>(new PlayerCamera());
    archery_camera = shared_ptr<Camera>(new ArcheryCamera());

    meshes.insert(pair<string, shared_ptr<MeshSet>>("chewy", shared_ptr<MeshSet>(new MeshSet("../Assets/Ninja/ninja_final2.dae"))));
	shared_ptr<GameEntity> chewy(new ChewyEntity(vec3(0.0, 0.0, 0.0), meshes.at("chewy"), player_camera));
    meshes.insert(pair<string, shared_ptr<MeshSet>>("guard", shared_ptr<MeshSet>(new MeshSet("../Assets/Samurai/samurai.dae"))));
	shared_ptr<GameEntity> guard(new GuardEntity(vec3(40.0, 0.0, -2.0), meshes.at("guard")));
    meshes.insert(pair<string, shared_ptr<MeshSet>>("arrow", shared_ptr<MeshSet>(new MeshSet("../Assets/Arrow/arrow.dae"))));
	shared_ptr<GameEntity> arrow(new ProjectileEntity(vec3(40.0f, 15.0f, -2.0f), meshes.at("arrow"), chewy, archery_camera));
    //meshes.insert(pair<string, shared_ptr<MeshSet>>("tower", shared_ptr<MeshSet>(new MeshSet("../Assets/Tower/tower.dae"))));

    //shared_ptr <GameEntity> tower(new ObstacleEntity(vec3(0.0, 0.0, 0.0), meshes.at("tower")));
    //tower->scale = 30.0f;
    meshes.insert(pair<string, shared_ptr<MeshSet>>("lantern", shared_ptr<MeshSet>(new MeshSet("../Assets/Lantern/lantern.dae"))));

    shared_ptr <GameEntity> lantern(new ObstacleEntity(vec3(30.0, 16.0, 31.5), meshes.at("lantern")));
    lantern->rotations.y = M_PI_2;
    meshes.insert(pair<string, shared_ptr<MeshSet>>("lanternPole", shared_ptr<MeshSet>(new MeshSet("../Assets/Lantern Pole/lanternPole.dae"))));
    shared_ptr <GameEntity> lantern_pole(new ObstacleEntity(vec3(30.0, 0.0, 30.0), meshes.at("lanternPole")));
    meshes.insert(pair<string, shared_ptr<MeshSet>>("closedBarrel", shared_ptr<MeshSet>(new MeshSet("../Assets/Barrel/ClosedBarrel.dae"))));
    shared_ptr <GameEntity> cBarrel(new ObstacleEntity(vec3(48.0, 0.0, 30.0), meshes.at("closedBarrel")));
    cBarrel->scale = 3.0f;
    meshes.insert(pair<string, shared_ptr<MeshSet>>("openBarrel", shared_ptr<MeshSet>(new MeshSet("../Assets/Barrel/OpenBarrel.dae"))));
    shared_ptr <GameEntity> oBarrel(new ObstacleEntity(vec3(48.0, 50.0, 40.0), meshes.at("openBarrel")));
    oBarrel->scale = 3.0f;
    meshes.insert(pair<string, shared_ptr<MeshSet>>("box", shared_ptr<MeshSet>(new MeshSet("../Assets/Box/Box.dae"))));
    shared_ptr <GameEntity> box1(new ObstacleEntity(vec3(50.0, 0.0, 20.0), meshes.at("box")));
    box1->scale = 3.0f;
    shared_ptr <GameEntity> box2(new ObstacleEntity(vec3(50.0, 7.0, 20.0), meshes.at("box")));
    box2->scale = 3.0f;
    shared_ptr <GameEntity> box3(new ObstacleEntity(vec3(50.0, 14.0, 20.0), meshes.at("box")));
    box3->scale = 3.0f;

    camera = player_camera;
    player_camera->in_use = true;
	entities.push_back(chewy);
	entities.push_back(guard);
	entities.push_back(arrow);
    //entities.push_back(tower);
    entities.push_back(lantern);
    entities.push_back(lantern_pole);
    entities.push_back(oBarrel);
    entities.push_back(cBarrel);
    entities.push_back(box1);
    entities.push_back(box2);
    entities.push_back(box3);




	shared_ptr<Shader> phongShader(new PhongShader("phongVert.glsl", "phongFrag.glsl"));
	shaders.insert(pair<string, shared_ptr<Shader>>("phongShader", phongShader));
}

void World::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaders.at("phongShader")->getProgramID());
	for (int i = 0; i < entities.size(); i++)
		shaders.at("phongShader")->draw(camera->getViewMatrix(), entities.at(i));
	//bunny_mesh->draw(mat4(1.0f), vec3(0, 0, 0));
    glUseProgram(0);
}

void World::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (action == GLFW_PRESS)
        keys[key] = true;
    else if (action == GLFW_RELEASE)
        keys[key] = false;
}

void World::mouse_callback(GLFWwindow* window, double x_position, double y_position)
{
    x_offset = x_position;
    y_offset = -1.f * y_position;

    glfwSetCursorPos(window, 0, 0);
}

void World::change_camera()
{
    if (keys[GLFW_KEY_1])
    {
        camera = debug_camera;
        debug_camera->in_use = true;
        player_camera->in_use = false;
        archery_camera->in_use = false;

    }
    if (keys[GLFW_KEY_2])
    {
        camera = player_camera;
        debug_camera->in_use = false;
        player_camera->in_use = true;
        archery_camera->in_use = false;
    }
    if (keys[GLFW_KEY_3])
    {
        camera = archery_camera;
        debug_camera->in_use = false;
        player_camera->in_use = false;
        archery_camera->in_use = true;
    }
}

void World::update_key_callbacks()
{
    camera->movement(entities.at(0));
    change_camera();

    x_offset = 0;
    y_offset = 0;
}

void World::update()
{
    OctTree* world_oct_tree = new OctTree(Voxel(vec3(-1000.f, -1000.f, -1000.f), vec3(1000.f, 1000.f, 1000.f)), entities, nullptr);

    // get the collison pairs and handle them as you desire

	static float start_time = 0.0;
	float end_time = glfwGetTime();
	for (int i = 0; i < entities.size(); i++) {
		entities[i]->update();
	}
	seconds_passed = end_time - start_time;
	start_time = glfwGetTime();

    update_key_callbacks();
}

void World::scroll_callback(GLFWwindow* window, double x_pos, double y_pos)
{
    shared_ptr<PlayerCamera> radius_changer = dynamic_pointer_cast<PlayerCamera>(camera);
    if (radius_changer)
        radius_changer->update_radius(y_pos);
}