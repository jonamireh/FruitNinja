#include "World.h"
#include "ChewyEntity.h"
#include "PhongShader.h"
#include "DebugCamera.h"
#include "Global.h"
#include "ArcheryCamera.h"
#include "GuardEntity.h"
#include "ProjectileEntity.h"

using namespace std;
using namespace glm;

bool keys[1024];
float seconds_passed = 0;
Timer timer;
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

	shared_ptr<GameEntity> chewy(new ChewyEntity(vec3(0.0, 0.0, 0.0), shared_ptr<MeshSet>(new MeshSet("../Assets/Ninja/ninja_final2.dae")), player_camera));

	shared_ptr<GameEntity> guard1(new GuardEntity(vec3(5.0, 0.0, 0.0), shared_ptr<MeshSet>(new MeshSet("bunny.obj"))));


	shared_ptr<GameEntity> arrow(new ProjectileEntity(vec3(5.0f, 0.0f, 0.0f), shared_ptr<MeshSet>(new MeshSet("bunny.obj")), chewy, archery_camera));

    camera = debug_camera;
    debug_camera->in_use = true;
	entities.push_back(chewy);
	entities.push_back(guard1);
	entities.push_back(arrow);

	shared_ptr<Shader> phongShader(new PhongShader("phongVert.glsl", "phongFrag.glsl"));
	shaders.insert(pair<string, shared_ptr<Shader>>("phongShader", phongShader));
}

void World::draw()
{
    timer.start_timing("startDraw");
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
    camera->movement(timer.end_timing(), entities.at(0));
    change_camera();

    x_offset = 0;
    y_offset = 0;
}

void World::update()
{
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