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
    vec3 h = vec3(bb.half_width, bb.half_height, bb.half_depth);

	float tMin = std::numeric_limits<float>::min();
	float tMax = std::numeric_limits<float>::max();
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

		if (abs(f) > 0.0001f)
        {
            float t1 = (e + h[i]) / f;
            float t2 = (e - h[i]) / f;

            if (t1 > t2)
            {
                //swap
				float temp = t1;
				t1 = t2;
				t2 = temp;
            }

			tMin = fmax(tMin, t1);
			tMax = fmin(tMax, t2);
			if (tMin > tMax || tMax < 0) {
				return pair<bool, float>(false, 0);
			}
        }
		else if (-1.0f * e - h[i] > 0 || h[i] - e < 0)
            return pair<bool, float>(false, 0);
    }
    if (tMin > 0)
        return pair<bool, float>(true, tMin);
    else
        return pair<bool, float>(false, tMax);
}

float static cast_ray(vec3 ray_start, vec3 ray_end, vector<GameEntity*> entities)
{
	float dist = 1.0f;
	float ray_dist = glm::distance(ray_start, ray_end);
	for (int i = 0; i < entities.size(); i++)
	{
		ChewyEntity* c_test = dynamic_cast<ChewyEntity*>(entities.at(i));
		if (c_test == nullptr && IN_OCTTREE(entities.at(i)->list))
		{
			pair<bool, float> result = obb_ray(ray_start, glm::normalize(ray_end - ray_start), entities.at(i)->bounding_box);
			if (result.first && result.second < ray_dist)
			{
				if (result.second / ray_dist < dist)
				{
					dist = result.second / ray_dist;
				}
			}
		}
	}
	assert(dist > 0.f && dist <= 1.0f);
	return dist;
}

void PlayerCamera::reorient(vector<GameEntity*> entities, ChewyEntity* chewy)
{
	vec3* wNear = get_near_plane_corners();
	float radius = glm::distance(cameraPosition, lookAtPoint);
	vec3 wzNear[5];
	for (int i = 0; i < 5; i++)
	{
		wzNear[i] = wNear[i] + radius * glm::normalize(lookAtPoint - cameraPosition);
	}

	vector<float> distances;
	for (int i = 0; i < 5; i++)
	{
		distances.push_back(cast_ray(wzNear[i], wNear[i], entities));
	}
	float min_zoom = distances.at(std::distance(distances.begin(), min_element(distances.begin(), distances.end()))) * radius;
	cameraPosition = chewy->bounding_box.center + vec3(0.f, chewy->bounding_box.half_height, 0.f) - min_zoom * cameraFront;
	delete[] wNear;
}

glm::vec3* PlayerCamera::get_near_plane_corners()
{
	float Hnear = 2 * tan(radians(PLAYER_FOV) / 2) * PLAYER_NEAR;
	float Wnear = Hnear * (screen_width / screen_height);
	vec3 up = vec3(0.f, 1.f, 0.f);
	vec3 v = glm::normalize(cameraPosition - lookAtPoint);
	vec3 w = glm::cross(v, up);
	vec3 Cnear = cameraPosition + v * PLAYER_NEAR;

	vec3* sp = new vec3[5];
	//top left
	sp[0] = Cnear + (up * (Hnear / 2)) - (w * (Wnear / 2));
	//top right
	sp[1] = Cnear + (up * (Hnear / 2)) + (w * (Wnear / 2));
	//bottom left
	sp[2] = Cnear - (up * (Hnear / 2)) + (w * (Wnear / 2));
	//bottom right
	sp[3] = Cnear - (up * (Hnear / 2)) - (w * (Wnear / 2));
	//center
	sp[4] = (sp[0] + sp[1] + sp[2] + sp[3]) / 4.f;
	return sp;
}