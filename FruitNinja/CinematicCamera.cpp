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

void CinematicCamera::init(vector<vec3> position_points, vector<vec3> lookAt_points, float move_speed)
{	
	cameraPosition = position_points.at(0);
	lookAtPoint = lookAt_points.at(0);
	pathing = CinematicPathingComponent(position_points, lookAt_points, move_speed);
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
void CinematicCamera::movement(GameEntity* chewy)
{
	pair<vec3, vec3> direction = pathing.get_direction();
	float move_speed = pathing.get_move_speed();
	cameraPosition += seconds_passed * move_speed * direction.first;
	lookAtPoint += seconds_passed * pathing.get_lookat_speed() * direction.second;
}

mat4 CinematicCamera::getViewMatrix()
{
	return lookAt(cameraPosition, lookAtPoint, cameraUp);
}