#include "CinematicCamera.h"
#include <GL/glew.h>
#include "Global.h"
#include "World.h"
#include "ChewyEntity.h"

using namespace glm;

/*
Default constructor.
*/
CinematicCamera::CinematicCamera()
{

}

/*
Destructor.
*/
CinematicCamera::~CinematicCamera()
{

}

static void update_lookAt(void* reference, float direction, float time_elapsed)
{
	CinematicCamera *cin_camera = (CinematicCamera *) reference;
	assert(time_elapsed <= 1.f);
	cin_camera->lookAtPoint += direction * time_elapsed * cin_camera->change;
}

void CinematicCamera::init(vec3 startLookAt, vec3 center, vector<vec3> control_points, float move_speed)
{
	this->startLookAt = startLookAt;
	this->center = center;
	this->lookAtPoint = startLookAt;
	cameraPosition = control_points.at(0);
	pathing = CinematicPathingComponent(control_points, move_speed, this, update_lookAt);
	this->change = center - startLookAt;
}
/*
Call the glfwSetCursorPosCallback to the window and this function.
*/
void CinematicCamera::mouse_update()
{
	
}

/**
Call this at the end of the draw loop to update for strafing.
*/
void CinematicCamera::movement(shared_ptr<GameEntity> chewy)
{
	vec3 direction = pathing.get_direction();
	cameraPosition += seconds_passed * pathing.get_move_speed() * direction;
}

mat4 CinematicCamera::getViewMatrix()
{
	return lookAt(cameraPosition, lookAtPoint, cameraUp);
}