#include "PlayerCamera.h"
#include "World.h"
#include "ChewyEntity.h"
#include <glm/gtx/string_cast.inl>

using namespace glm;

const float PlayerCamera::MinVerticalAngle = 0.0f;
const float max_radius = 27.f;
const float min_radius = 5.f;
/*
Default constructor.
*/
PlayerCamera::PlayerCamera()
{
	lookAtPoint = vec3(0);
    radius = 17.0f;
}

/*
Specify the lookAt point (i.e. the player character)
*/
PlayerCamera::PlayerCamera(vec3 lookAt)
{
	this->lookAtPoint = lookAt;
}

/*
Destructor.
*/
PlayerCamera::~PlayerCamera()
{

}

/*
Call the glfwSetCursorPosCallback to the window and this function.
*/
void PlayerCamera::mouse_update()
{
	float sensitivity = 0.1;

    theta -= x_offset * sensitivity;
    phi -= y_offset * sensitivity;

    if (phi > MaxVerticalAngle)
    {
        phi = MaxVerticalAngle;
        y_offset = 0;

    }
    if (phi < -MaxVerticalAngle)
    {
        phi = -MaxVerticalAngle;
        y_offset = 0;
    }

    cameraFront = rotateY(cameraFront, radians(x_offset * sensitivity));
    cameraFront = rotate(cameraFront, -radians(y_offset * sensitivity), cross(cameraFront, cameraUp));
    cameraFront = normalize(cameraFront);
}

/**
Call this at the end of the draw loop to update for strafing.
*/
void PlayerCamera::movement(GameEntity* chewy)
{
    mouse_update();

    cameraPosition = chewy->bounding_box.center + vec3(0.f, chewy->bounding_box.half_height, 0.f) - radius * cameraFront;
    lookAtPoint = chewy->bounding_box.center + vec3(0.f, chewy->bounding_box.half_height, 0.f);
}

mat4 PlayerCamera::getViewMatrix()
{
	//cout << "pos: " << glm::to_string(cameraPosition) << endl;
	//cout << glm::to_string(lookAtPoint) << endl;
	return lookAt(cameraPosition, lookAtPoint, cameraUp);
}

void PlayerCamera::update_radius(float delta)
{
    float new_radius = radius - 0.5 * delta;
    if (new_radius > max_radius)
        new_radius = max_radius;
    else if (new_radius < min_radius)
        new_radius = min_radius;

    radius = new_radius;
}

pair<bool, float> obb_ray(vec3 origin, vec3 direction, EntityBox bb)
{
    vec3 center = bb.center;
    vec3 h = vec3(bb.half_width, bb.half_height, bb.half_depth); // IF VIEW FRUSTUM WAS OFF THIS IS OFF IN THE SAME WAY

    float tMin = FLT_MIN;
    float tMax = FLT_MAX;
    vec3 p = center - origin;
    for (int i = 0; i < 3; i++)
    {
        vec3 ai(0.f);
        if (i == 0)
            ai = vec3(1.f, 0.f, 0.f);
        if (i == 1)
            ai = vec3(0.f, 1.f, 0.f);
        if (i == 2)
            ai = vec3(0.f, 0.f, 1.f);

        float e = dot(ai, p);
        float f = dot(ai, direction);

        if (abs(f) > FLT_EPSILON)
        {
            float t1 = (e + h[i]) / f;
            float t2 = (e - h[i]) / f;

            if (t1 > t2)
            {
                //swap
                float temp = t2;
                t2 = t1;
                t1 = temp;
            }

            if (t1 > tMin)
                tMin = t1;
            if (t2 < tMax)
                tMax = t2;
            if (tMin > tMax)
                return pair<bool, float>(false, 0.f);
            if (tMax < 0)
                return pair<bool, float>(false, 0);
        }
        else if (-e - h[i] > 0 || -e + h[i] < 0)
            return pair<bool, float>(false, 0);
    }
    if (tMin > 0)
        return pair<bool, float>(true, tMin);
    else
        return pair<bool, float>(false, tMax);
}

void PlayerCamera::reorient(vector<GameEntity*> entities, ChewyEntity* chewy)
{
	cameraPosition = handle_collision_zoom(5.f, get_near_plane_corners(), entities, chewy);
}

glm::vec3* PlayerCamera::get_near_plane_corners()
{
	float Hnear = 2 * tan(radians(PLAYER_FOV) / 2) * PLAYER_NEAR;
	float Wnear = Hnear * (screen_width / screen_height);
	vec3 up = vec3(0.f, 1.f, 0.f);
	vec3 v = glm::normalize(cameraPosition - lookAtPoint);
	vec3 w = glm::cross(v, up);
	vec3 Cnear = cameraPosition + v * PLAYER_NEAR;

	//shared_ptr<vec3[]> near_plane_corners = shared_ptr<vec3[]>(new vec3[4]);
	vec3* sp = new vec3[4];
	//top left
	sp[0] = Cnear + (up * (Hnear / 2)) - (w * (Wnear / 2));
	World::draw_point(sp[0], vec3(1.0f, 0.f, 0.f), 10.f);
	//top right
	sp[1] = Cnear + (up * (Hnear / 2)) + (w * (Wnear / 2));
	World::draw_point(sp[1], vec3(1.0f, 0.f, 0.f), 10.f);
	//bottom left
	sp[2] = Cnear - (up * (Hnear / 2)) - (w * (Wnear / 2));
	World::draw_point(sp[2], vec3(1.0f, 0.f, 0.f), 10.f);
	//bottom right
	sp[3] = Cnear - (up * (Hnear / 2)) + (w * (Wnear / 2));
	World::draw_point(sp[3], vec3(1.0f, 0.f, 0.f), 10.f);
	return sp;
}


float static cast_ray(vec3 ray_start, vec3 ray_end, vector<GameEntity*> entities, float qualifying_distance)
{
	float dist = -1.0f;
	float ray_dist = glm::distance(ray_start, ray_end);
	for (int i = 0; i < entities.size(); i++)
	{
		ChewyEntity* c_test = dynamic_cast<ChewyEntity*>(entities.at(i));
		if (c_test == nullptr && IN_OCTTREE(entities.at(i)->list))
		{
			pair<bool, float> result = obb_ray(ray_start, glm::normalize(ray_end - ray_start), entities.at(i)->bounding_box);
			if (result.first && result.second < ray_dist)
			{
				//cout << "camera hit made" << endl;
				if (result.second / ray_dist > dist)
				{
					dist = result.second / ray_dist;
				}
			}
		}
	}
	if (dist < 0.f)
		dist = 1.f;
	assert(dist > 0.f);
	return dist;
}


vec3 PlayerCamera::handle_collision_zoom(float minOffsetDist, vec3* frustumNearCorners, vector<GameEntity*> entities, ChewyEntity* chewy)
{
	float offsetDist = glm::length(lookAtPoint - cameraPosition);
	float qualifying_distance = glm::distance(cameraPosition, lookAtPoint);
	float raycastLength = offsetDist - minOffsetDist;
	if (raycastLength < 0.f)
	{
		// camera is already too near the lookat target
		return cameraPosition;
	}

	vec3 camOut = glm::normalize(lookAtPoint - cameraPosition);
	vec3 nearestCamPos = lookAtPoint - camOut * minOffsetDist;
	float minHitFraction = 1.f;

	for (int i = 0; i < 4; i++)
	{
		const vec3& corner = frustumNearCorners[i];
		vec3 offsetToCorner = corner - cameraPosition;
		vec3 rayStart = nearestCamPos + offsetToCorner;
		vec3 rayEnd = corner;
		// a result between 0 and 1 indicates a hit along the ray segment
		float hitFraction = cast_ray(rayStart, rayEnd, entities, qualifying_distance);
		minHitFraction = glm::min(hitFraction, minHitFraction);
	}

	if (minHitFraction < 1.f)
	{
		return nearestCamPos - camOut * (raycastLength * minHitFraction);
	}
	else
	{
		return cameraPosition;
	}
}