#include "World.h"
#include "World.h"
#include "PhongShader.h"
#include "TextureDebugShader.h"
#include "DebugCamera.h"
#include "Global.h"
#include "ArcheryCamera.h"
#include "GuardEntity.h"
#include "ProjectileEntity.h"
#include "ObstacleEntity.h"
#include "OctTree.h"
#include "DeferredShader.h"
#include "FrustrumCulling.h"
#include "CollisionHandler.h"
#include "DebugShader.h"
#include <glm/gtx/rotate_vector.hpp>
#include "Skybox.h"
#include "SimpleTextureShader.h"
#include "TestSphere.h"
#include <functional>
#include <queue>
#include "LightEntity.h"
#include "ChewyEntity.h"
#include "FlameParticleShader.h"

using namespace std;
using namespace glm;

bool keys[1024];
float seconds_passed = 0;
float x_offset;
float y_offset;

bool debug_enabled = false;
float screen_width = SCREEN_WIDTH;
float screen_height = SCREEN_HEIGHT;

static std::shared_ptr<Camera> camera;
static shared_ptr<DebugShader> debugShader;
bool time_stopped = false;
static vector<std::function<void()>> debugShaderQueue;

World::World()
{
    debugShader = shared_ptr<DebugShader>(new DebugShader("debugVert.glsl", "debugFrag.glsl"));
	init();
    x_offset = 0;
    y_offset = 0;
}

void World::init()
{
	debug_camera = shared_ptr<Camera>(new DebugCamera());
    player_camera = shared_ptr<Camera>(new PlayerCamera());
    archery_camera = shared_ptr<Camera>(new ArcheryCamera());
    meshes.insert(pair<string, shared_ptr<MeshSet>>("tower", make_shared<MeshSet>(assetPath + "tower.dae")));
    shared_ptr <GameEntity> tower(new ObstacleEntity(vec3(0.0, 0.0, 0.0), meshes.at("tower")));
    tower->scale = 30.0f;
    meshes.insert(pair<string, shared_ptr<MeshSet>>("chewy", shared_ptr<MeshSet>(new MeshSet(assetPath + "ninja_final2.dae"))));
    chewy = std::make_shared<ChewyEntity>(vec3(0.0, 0.0, 0.0), meshes.at("chewy"), player_camera);
    // chewy bounding box mesh
    meshes.insert(pair<string, shared_ptr<MeshSet>>("chewy_bb", shared_ptr<MeshSet>(new MeshSet(assetPath + "ninja_boundingbox.dae"))));
    chewy->largestBB = (new ChewyEntity(vec3(0.0, 0.0, 0.0), meshes.at("chewy_bb"), player_camera))->getOuterBoundingBox();
    chewy->sebInit();
    chewy->collision_response = true;

	meshes.insert(pair<string, shared_ptr<MeshSet>>("guard", shared_ptr<MeshSet>(new MeshSet(assetPath + "samurai.dae"))));
	shared_ptr<GameEntity> guard(new GuardEntity(vec3(100.0, 0.0, 0.0), meshes.at("guard"), { vec3(100.0, 0.0, 0.0), vec3(80.0, 0.0, -6.0), 
		vec3(60.0, 0.0, 0.0), vec3(40.0, 0.0, -6.0), vec3(20.0, 0.0, 0.0)}, 10.f));
	shared_ptr<GameEntity> guard1(new GuardEntity(vec3(-50.0, 0.0, 60.0), meshes.at("guard"), { vec3(-50.0, 0.0, 60.0), vec3(-25.0, 0.0, 45.0),
		vec3(3.0, 0.0, 45.0), vec3(50.0, 0.0, 60.0), vec3(90.0, 0.0, 20.0) }, 15.f));
	shared_ptr<GameEntity> guard2(new GuardEntity(vec3(-103.0, 0.0, -35.0), meshes.at("guard"), { vec3(-103.0, 0.0, -35.0), vec3(-60.0, 0.0, -25.0),
		vec3(-12.0, 0.0, -25.0), vec3(35.0, 0.0, -25.0), vec3(45.0, 0.0, -35.0), vec3(25.0, 0, -35.0), vec3(-2.0,0 , -50.0) }, 30.f));

    guard->collision_response = true;
    guard1->collision_response = true;
    guard2->collision_response = true;

	meshes.insert(pair<string, shared_ptr<MeshSet>>("arrow", make_shared<MeshSet>(assetPath + "arrow.dae")));
	//shared_ptr<GameEntity> arrow(new ProjectileEntity(vec3(40.0f, 15.0f, -2.0f), meshes.at("arrow"), chewy, archery_camera));

	meshes.insert(pair<string, shared_ptr<MeshSet>>("unit_sphere", make_shared<MeshSet>(assetPath + "UnitSphere.obj")));
    
	meshes.insert(pair<string, shared_ptr<MeshSet>>("lantern", shared_ptr<MeshSet>(new MeshSet(assetPath + "lantern.dae"))));


	shared_ptr <GameEntity> lantern(new LightEntity(vec3(30.0, 16.0, 31.5), meshes.at("lantern"), 500.0f, meshes.at("unit_sphere")));
    lantern->rotations.y = M_PI_2;
	shared_ptr <GameEntity> lantern2(new LightEntity(vec3(0.0, 16.0, 31.5), meshes.at("lantern"), 500.0f, meshes.at("unit_sphere")));
	lantern2->rotations.y = M_PI_2;
	shared_ptr <GameEntity> lantern3(new LightEntity(vec3(30.0, 16.0, 0.0), meshes.at("lantern"), 500.0f, meshes.at("unit_sphere")));
	lantern3->rotations.y = M_PI_2;
	shared_ptr <GameEntity> lantern4(new LightEntity(vec3(-30.0, 2.0, 20), meshes.at("lantern"), 500.0f, meshes.at("unit_sphere")));
	lantern4->rotations.y = M_PI_2;
	shared_ptr <GameEntity> lantern5(new LightEntity(vec3(-50.0, 10.0, 0.0), meshes.at("lantern"), 500.0f, meshes.at("unit_sphere")));
	lantern5->rotations.y = M_PI_2;



	meshes.insert(pair<string, shared_ptr<MeshSet>>("lanternPole", shared_ptr<MeshSet>(new MeshSet(assetPath + "lanternPole.dae"))));
    shared_ptr <GameEntity> lantern_pole(new ObstacleEntity(vec3(30.0, 0.0, 30.0), meshes.at("lanternPole")));

	meshes.insert(pair<string, shared_ptr<MeshSet>>("closedBarrel", shared_ptr<MeshSet>(new MeshSet(assetPath + "closedBarrel.dae"))));
    shared_ptr <GameEntity> cBarrel(new ObstacleEntity(vec3(48.0, 0.0, 30.0), meshes.at("closedBarrel")));
    cBarrel->scale = 3.0f;
	shared_ptr <GameEntity> cBarrel2(new ObstacleEntity(vec3(-50.0, 0.0, -10.0), meshes.at("closedBarrel")));
	cBarrel2->scale = 3.0f;
	shared_ptr <GameEntity> cBarrel3(new ObstacleEntity(vec3(-58.0, 0.0, 10.0), meshes.at("closedBarrel")));
	cBarrel3->scale = 3.0f;


	meshes.insert(pair<string, shared_ptr<MeshSet>>("openBarrel", shared_ptr<MeshSet>(new MeshSet(assetPath + "openBarrel.dae"))));
    shared_ptr <GameEntity> oBarrel(new ObstacleEntity(vec3(30.0, 0.0, 40.0), meshes.at("openBarrel")));
    oBarrel->scale = 3.0f;
	meshes.insert(pair<string, shared_ptr<MeshSet>>("box", shared_ptr<MeshSet>(new MeshSet(assetPath + "Box.dae"))));
    shared_ptr <GameEntity> box1(new ObstacleEntity(vec3(50.0, 0.0, 20.0), meshes.at("box")));
    box1->scale = 3.0f;
    shared_ptr <GameEntity> box2(new ObstacleEntity(vec3(50.0, 7.0, 20.0), meshes.at("box")));
    box2->scale = 3.0f;
    shared_ptr <GameEntity> box3(new ObstacleEntity(vec3(50.0, 14.0, 20.0), meshes.at("box")));
    box3->scale = 3.0f;
	meshes.insert(pair<string, shared_ptr<MeshSet>>("skybox", shared_ptr<MeshSet>(new MeshSet(assetPath + "skybox.dae", GL_LINEAR, GL_CLAMP_TO_EDGE))));
	_skybox = std::make_shared<Skybox>(Skybox(&camera, meshes.at("skybox")));
	_skybox->scale = 750.f;

	meshes.insert(pair<string, shared_ptr<MeshSet>>("testsphere", shared_ptr<MeshSet>(new MeshSet(assetPath + "testsphere.dae"))));
	shared_ptr <TestSphere> testSphere(new TestSphere(meshes.at("testsphere")));

    meshes.insert(pair<string, shared_ptr<MeshSet>>("wall_back", make_shared<MeshSet>(assetPath + "wall_back.dae")));
    shared_ptr <GameEntity> wb(new ObstacleEntity(vec3(0.0, 0.0, 0.0), meshes.at("wall_back")));
    wb->scale = 30.f;
    meshes.insert(pair<string, shared_ptr<MeshSet>>("wall_front", make_shared<MeshSet>(assetPath + "wall_front.dae")));
    shared_ptr <GameEntity> wf(new ObstacleEntity(vec3(0.0, 0.0, 0.0), meshes.at("wall_front")));
    wf->scale = 30.f;
    meshes.insert(pair<string, shared_ptr<MeshSet>>("wall_left", make_shared<MeshSet>(assetPath + "wall_left.dae")));
    shared_ptr <GameEntity> wl(new ObstacleEntity(vec3(0.0, 0.0, 0.0), meshes.at("wall_left")));
    wl->scale = 30.f;
    meshes.insert(pair<string, shared_ptr<MeshSet>>("wall_right", make_shared<MeshSet>(assetPath + "wall_right.dae")));
    shared_ptr <GameEntity> wr(new ObstacleEntity(vec3(0.0, 0.0, 0.0), meshes.at("wall_right")));
    wr->scale = 30.f;
    meshes.insert(pair<string, shared_ptr<MeshSet>>("ground", make_shared<MeshSet>(assetPath + "ground.dae")));
    shared_ptr <GameEntity> g(new ObstacleEntity(vec3(0.0, 0.0, 0.0), meshes.at("ground")));
    g->scale = 30.f;

    

    camera = player_camera;
    player_camera->in_use = true;
    entities.push_back(tower);
	entities.push_back(chewy);
	entities.push_back(guard);
	entities.push_back(guard1);
	entities.push_back(guard2);

    entities.push_back(lantern);
	entities.push_back(lantern2);
	entities.push_back(lantern3);
	entities.push_back(lantern4);
	entities.push_back(lantern5);

    entities.push_back(lantern_pole);
    entities.push_back(oBarrel);
    entities.push_back(cBarrel);
	entities.push_back(cBarrel2);
	entities.push_back(cBarrel3);

    entities.push_back(box1);
    entities.push_back(box2);
    entities.push_back(box3);
    entities.push_back(testSphere);
    entities.push_back(wb);
    entities.push_back(wf);
    entities.push_back(wr);
    entities.push_back(wl);
    entities.push_back(g);


shared_ptr<Shader> phongShader(new PhongShader("phongVert.glsl", "phongFrag.glsl"));
shaders.insert(pair<string, shared_ptr<Shader>>("phongShader", phongShader));
flame = std::make_shared<Flame>(vec3(-30.0, 3.0, 20), 0.85f, 0.3f);
shared_ptr<Shader> defShader(new DeferredShader("DeferredVertShader.glsl", "DeferredFragShader.glsl", _skybox, flame));
shaders.insert(pair<string, shared_ptr<Shader>>("defShader", defShader));

shaders.insert(pair<string, shared_ptr<Shader>>("debugShader", debugShader));

shared_ptr<Shader> simpleShader(new SimpleTextureShader("simpleVert.glsl", "simpleFrag.glsl"));
shaders.insert(pair<string, shared_ptr<Shader>>("simpleShader", simpleShader));
shared_ptr<Shader> flameShader(new FlameParticleShader("flameVert.glsl", "flameFrag.glsl"));
shaders.insert(pair<string, shared_ptr<Shader>>("flameShader", flameShader));

//shared_ptr<Shader> textDebugShader(new TextureDebugShader());
//shaders.insert(pair<string, shared_ptr<Shader>>("textureDebugShader", textDebugShader));
}

void World::shootArrows()
{

	static bool held = false;
	if (keys[GLFW_KEY_E] && archery_camera->in_use && !held)
	{
		held = true;
	}
	if (held && !keys[GLFW_KEY_E])
	{
		entities.push_back(make_shared<ProjectileEntity>(meshes["arrow"], archery_camera));
		held = false;
	}
}

void World::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(0);
	static bool usePhong = false;

	if (keys[GLFW_KEY_6])
	{
		usePhong = true;
	}
	if (keys[GLFW_KEY_7])
	{
		usePhong = false;
	}


	if (usePhong) {
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glDepthMask(GL_TRUE);


		glUseProgram(shaders.at("simpleShader")->getProgramID());
		glViewport(0, 0, screen_width, screen_height);
		shaders.at("simpleShader")->draw(camera->getViewMatrix(), _skybox);
	}

	shared_ptr<DebugCamera> c_test = dynamic_pointer_cast<DebugCamera>(camera);
	vector<shared_ptr<GameEntity>> in_view;
    if (c_test != nullptr)
    {
        in_view = get_objects_in_view(entities, camera->getViewMatrix());
	}
	else
	{
		in_view = get_objects_in_view(entities, camera->getViewMatrix());

	}
	glUseProgram(0);


	if (usePhong) {
		glUseProgram(shaders.at("phongShader")->getProgramID());
		glViewport(0, 0, screen_width, screen_height);
		for (int i = 0; i < in_view.size(); i++)
			shaders.at("phongShader")->draw(camera->getViewMatrix(), in_view.at(i));
	}
	//otherwise deferred rendering
	else {
		//even if lantern culled still need light from it
		vector<Light*> lights;
		for (int i = 0; i < entities.size(); i++) {
			if (typeid(*entities[i]) == typeid(LightEntity) && entities[i]->should_draw) {
				shared_ptr<LightEntity> le = dynamic_pointer_cast<LightEntity>(entities[i]);
				lights.push_back(&le->light);
			}
			//remove arrows with no time left
			/*if (typeid(*entities[i]) == typeid(ProjectileEntity)) {
				shared_ptr<ProjectileEntity> pe = dynamic_pointer_cast<ProjectileEntity>(entities[i]);
				if (pe->timeLeft < 0.0) {
					entities.erase(entities.begin() + i);
					i--;
				}
			}*/
			if (!entities[i]->should_draw) {
				entities.erase(entities.begin() + i);
				i--;
			}
		}
		for (int i = 0; i < in_view.size(); i++)
		{
			if (!in_view[i]->should_draw) {
				in_view.erase(in_view.begin() + i);
				i--;
			}
		}
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaders.at("defShader")->getProgramID());
		glViewport(0, 0, screen_width, screen_height);
		shaders.at("defShader")->draw(camera, in_view, lights);
	}

    glUseProgram(0);
	if (debug_enabled)
	{
		glUseProgram(debugShader->getProgramID());
		for (int i = 0; i < in_view.size(); i++)
		{
			shared_ptr<BoundingBox> box = in_view.at(i)->getTransformedOuterBoundingBox();
			shared_ptr<vector<pair<vec3, vec3>>> points = box->get_line_segments();
			for (int j = 0; j < points->size(); j++)
			{
				draw_line(points->at(j).first, points->at(j).second, vec3(1.f, 0.f, 0.f));
			}
			vector<pair<glm::vec3, glm::vec3>> planes = box->getPlanes();
			for (int k = 0; k < planes.size(); k++)
			{
				draw_line(planes.at(k).first, planes.at(k).first + box->getMaxWidth(5.0f) * planes.at(k).second, vec3(0, 1.f, 0));
			}
			draw_sphere(in_view.at(i)->getCenter(), in_view.at(i)->getRadius(), vec3(1.f, 1.f, 0.f), 3.f);
		}
		glUseProgram(0);
	}

	glUseProgram(debugShader->getProgramID());
	for (int i = debugShaderQueue.size() - 1; i >= 0; i--)
	{
		debugShaderQueue.at(i)();
	}
	if (!time_stopped)
	{
		debugShaderQueue.clear();
	}
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

void World::enable_debugging()
{
	if (keys[GLFW_KEY_Z])
		debug_enabled = true;
	if (keys[GLFW_KEY_X])
		debug_enabled = false;
}

void World::update_key_callbacks()
{
    camera->movement(chewy);
    change_camera();
	enable_debugging();
	stop_time();
    x_offset = 0;
    y_offset = 0;
}

void World::stop_time()
{
	if (keys[GLFW_KEY_T])
		time_stopped = true;
	if (keys[GLFW_KEY_Y])
		time_stopped = false;
}

void World::update()
{
	static float start_time = 0.0;

	float end_time = glfwGetTime();
	shootArrows();

	for (int i = 0; i < entities.size(); i++)
		entities[i]->update();
	if (!time_stopped)
	{
		seconds_passed = end_time - start_time;
	}
	else
	{
		seconds_passed = 0.f;
	}
	start_time = glfwGetTime();
    std::vector<shared_ptr<GameEntity>> v1(entities.begin() + 1, entities.end() );
	OctTree* world_oct_tree = new OctTree(Voxel(vec3(-1000.f, -1000.f, -1000.f), vec3(1000.f, 1000.f, 1000.f)), v1, nullptr);
	collision_handler(world_oct_tree->collision_pairs);
    update_key_callbacks();
	_skybox->update();
}

void World::scroll_callback(GLFWwindow* window, double x_pos, double y_pos)
{
    shared_ptr<PlayerCamera> radius_changer = dynamic_pointer_cast<PlayerCamera>(camera);
    if (radius_changer)
        radius_changer->update_radius(y_pos);
}

void World::draw_line(vec3 p1, vec3 p2, vec3 color)
{
	glUseProgram(debugShader->getProgramID());
	debugShaderQueue.push_back([=](){debugShader->drawLine(p1, p2, color, camera->getViewMatrix()); });
	glUseProgram(0);
}

void World::draw_point(vec3 p, vec3 color, float radius)
{
	glUseProgram(debugShader->getProgramID());
	debugShaderQueue.push_back([=](){debugShader->drawPoint(p, color, radius, camera->getViewMatrix());  });
	glUseProgram(0);
}

void World::draw_sphere(vec3 center, float radius, vec3 color, float delta)
{
	glUseProgram(debugShader->getProgramID());
	vector<float> points;
	for (float theta = 0.f; theta < 360.f; theta +=delta)
	{
		float x = radius * cos(radians(theta)) * cos(radians(0.f));
		points.push_back(x + center.x);
		float y = radius * sin(radians(0.f));
		points.push_back(y + center.y);
		float z = radius * sin(radians(theta)) * cos(radians(0.f));
		points.push_back(z + center.z);
	}
	//assert(points.size() == 360.f / delta);
	debugShaderQueue.push_back([=](){debugShader->drawPoints(points, color, camera->getViewMatrix()); });
	points.clear();
	for (float phi = 0.f; phi < 360.f; phi +=delta)
	{
		float x = radius * cos(radians(0.f)) * cos(radians(phi));
		points.push_back(x + center.x);
		float y = radius * sin(radians(phi));
		points.push_back(y + center.y);
		float z = radius * sin(radians(0.f)) * cos(radians(phi));
		points.push_back(z + center.z);
	}
	//assert(points.size() == 360.f / delta);
	debugShaderQueue.push_back([=](){debugShader->drawPoints(points, color, camera->getViewMatrix()); });
	points.clear();
	for (float phi = -180.f; phi < 180.f; phi += delta)
	{
		float x = radius * cos(radians(90.f)) * cos(radians(phi));
		points.push_back(x + center.x);
		float y = radius * sin(radians(phi));
		points.push_back(y + center.y);
		float z = radius * sin(radians(90.f)) * cos(radians(phi));
		points.push_back(z + center.z);
	}
	//assert(points.size() == 360.f / delta);
	debugShaderQueue.push_back([=](){debugShader->drawPoints(points, color, camera->getViewMatrix()); });
	glUseProgram(0);
}