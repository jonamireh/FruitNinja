#include "ChewyEntity.h"
#include "PlayerCamera.h"
#include "World.h"
#include <glm/gtx/string_cast.inl>
#include <glm/gtx/intersect.hpp>

using namespace glm;

ChewyEntity::ChewyEntity() : moveComponent(*this, std::shared_ptr<Camera>(new PlayerCamera()), std::shared_ptr<Camera>(new PlayerCamera())), animComponent(this)
{
	
}

ChewyEntity::ChewyEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh, std::shared_ptr<Camera> player_cam, std::shared_ptr<Camera> archery_cam) : GameEntity(position, mesh, true), moveComponent(*this, player_cam, archery_cam), animComponent(this)
{
	current_animation = &mesh->getAnimations()[0];
}

void ChewyEntity::update()
{
	GameEntity::update();
	std::vector<Mesh*> meshes = mesh->getMeshes();
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes.at(i)->mat = meshes.at(i)->bMat;
	}
	moveComponent.update();
	GameEntity::update();
	animComponent.update();
}

void ChewyEntity::set_material(Material material)
{
	std::vector<Mesh*> meshes = mesh->getMeshes();
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes.at(i)->mat = material;
	}
}

int intersect3D_SegmentPlane(vec3 sP0, vec3 sP1, pair<glm::vec3, glm::vec3> Pn, vec3* I)
{
    vec3 u = sP1 - sP0;
    vec3 w = sP0 - Pn.first;

    float D = dot(Pn.second, u);
    float N = -dot(Pn.second, w);

    if (fabs(D) < -FLT_MAX)
    {           // segment is parallel to plane
        if (N == 0)                      // segment lies in plane
            return 2;
        else
            return 0;                    // no intersection
    }
    // they are not parallel
    // compute intersect param
    float sI = N / D;
    if (sI < 0 || sI > 1)
        return 0;                        // no intersection

    *I = sP0 + sI * u;                  // compute segment intersect point
    return 1;
}

void ChewyEntity::collision(std::shared_ptr<GameEntity> entity)
{
    shared_ptr<BoundingBox> chewy_bounding_box = getTransformedOuterBoundingBox();
    shared_ptr<BoundingBox> other_bounding_box = entity->getTransformedOuterBoundingBox();

    shared_ptr<BoundingBox> smaller_bounding_box;
    shared_ptr<BoundingBox> larger_bounding_box;

    vector<vec3> smaller_box_points;
    vector<vec3> larger_box_points;

    vec3 contained_point;
    vec3 check_to_point;
    bool point_is_contained = false;
    bool falling = true;
    float direction_correction = 1.f;

    pair<vec3, vec3> collision_plane;
    vector<pair<glm::vec3, glm::vec3>> larger_box_planes;

    float distance_from_last_position = glm::distance(last_position, getPosition());

    bool bounding_boxes_are_colliding = false;
    float closest_distance = FLT_MAX;

    if (glm::distance(chewy_bounding_box->upper_bound, chewy_bounding_box->lower_bound) < glm::distance(other_bounding_box->upper_bound, other_bounding_box->lower_bound))
    {
        smaller_bounding_box = chewy_bounding_box;
        larger_bounding_box = other_bounding_box;
    }
    else
    {
        smaller_bounding_box = other_bounding_box;
        larger_bounding_box = chewy_bounding_box;
        direction_correction = -1.f;
    }

    smaller_box_points = smaller_bounding_box->get_points();
    larger_box_points = larger_bounding_box->get_points();
    larger_box_planes = larger_bounding_box->getPlanes();

    for (int i = 0; i < smaller_box_points.size(); i++)
    {
        if (larger_bounding_box->contains_point(smaller_box_points.at(i)))
        {
            float check_distance = glm::distance((larger_bounding_box->lower_bound + larger_bounding_box->upper_bound) / 2.f, smaller_box_points.at(i));
            if (check_distance < closest_distance)
            {
                contained_point = smaller_box_points.at(i);
                check_to_point = contained_point - distance_from_last_position * moveComponent.direction * direction_correction;
                closest_distance = check_distance;
            }
            point_is_contained = true;
        }
    }

    // if the bounding boxes do not intersect then leave this method
    if (!point_is_contained)
        return;

    for (int i = 0; i < smaller_box_points.size(); i++)
    {
        for (int j = 0; j < larger_box_planes.size(); j++)
        {
            vec3 d;
            int intersection = intersect3D_SegmentPlane(smaller_box_points.at(i), smaller_box_points.at(i) - velocity * direction_correction, larger_box_planes.at(j), &d);
            if (intersection)
            {
                falling = false;
            }
        }
    }

    // check for xz collisions
    for (int i = 0; i < larger_box_planes.size(); i++)
    {
        vec3 d;
        int intersection = intersect3D_SegmentPlane(contained_point, check_to_point, larger_box_planes.at(i), &d);
        if (intersection)
        {
            collision_plane = larger_box_planes.at(i);
            falling = true; // may cause issues when platforming, re-evaluate if issues arise.
        }
    }

    vec3 cancel_direction = vec3(1.f, 1.f, 1.f);
    if (collision_plane.second.x)
        cancel_direction.x = 0.f;
    if (collision_plane.second.z)
        cancel_direction.z = 0.f;

	vec3 pos = getPosition();

    if (!falling)
        pos.y = last_position.y;
       
    _falling = falling;

    pos.x = last_position.x;
    pos.z = last_position.z;
    pos += moveComponent.direction * cancel_direction * CHEWY_MOVE_SPEED * seconds_passed;

	setPosition(pos);
}
