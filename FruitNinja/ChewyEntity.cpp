#include "ChewyEntity.h"
#include "PlayerCamera.h"
#include "World.h"
#include <glm/gtx/string_cast.inl>
#include <glm/gtx/intersect.hpp>

using namespace glm;

ChewyEntity::ChewyEntity() : moveComponent(*this, std::shared_ptr<Camera>(new PlayerCamera())), animComponent(this)
{
	
}

ChewyEntity::ChewyEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh, std::shared_ptr<Camera> camera) : GameEntity(position, mesh), moveComponent(*this, camera), animComponent(this)
{
	current_animation = &mesh->getAnimations()[0];
}

void ChewyEntity::update()
{
	std::vector<Mesh*> meshes = mesh->getMeshes();
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes.at(i)->mat = meshes.at(i)->bMat;
	}
	moveComponent.update();
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
	shared_ptr<BoundingBox> bb = entity->getTransformedOuterBoundingBox();
	vector<pair<glm::vec3, glm::vec3>> planes = bb->getPlanes();

    vector<vec3> chewy_points = getTransformedOuterBoundingBox()->get_points();
    vector<vec3> other_entity_points = bb->get_points();
    vec3 contained_point;
    float closest_distance = 100.f;
    bool point_is_contained = false;
    for (int i = 0; i < chewy_points.size(); i++)
    {
        if (bb->contains_point(chewy_points.at(i)))
        {
            float check_distance = glm::distance((bb->lower_bound + bb->upper_bound) / 2.f, chewy_points.at(i));
            if (check_distance < closest_distance)
            {
                contained_point = chewy_points.at(i);
                closest_distance = check_distance;
            }
            point_is_contained = true;
        }
    }

    bool other_contained_in_chewy = false;
    for (int i = 0; i < other_entity_points.size(); i++)
    {
        if (getTransformedOuterBoundingBox()->contains_point(other_entity_points.at(i)))
        {
            other_contained_in_chewy = true;
        }
    }

    float distance_changed = glm::distance(last_position, position);

    vec3 check_to;
    if (point_is_contained)
        check_to = contained_point - distance_changed * moveComponent.direction;

    if (other_contained_in_chewy && !point_is_contained)
    {
        contained_point = entity->getCenter();
        check_to = getCenter();
        point_is_contained = true;
    }

    if (!point_is_contained)
        return;

    pair<glm::vec3, glm::vec3> collision_plane;


    for (int i = 0; i < planes.size(); i++)
    {
        vec3 d;
        int intersection = intersect3D_SegmentPlane(contained_point, check_to, planes.at(i), &d);
        if (intersection)
        {
            collision_plane = planes.at(i);
        }
    }

    vec3 cancel_direction = vec3(1.f, 1.f, 1.f);
    if (collision_plane.second.x)
        cancel_direction.x = 0.f;
    if (collision_plane.second.y)
        cancel_direction.y = 0.f;
    if (collision_plane.second.z)
        cancel_direction.z = 0.f;

    position = last_position;
    position += moveComponent.direction * cancel_direction * CHEWY_MOVE_SPEED * seconds_passed;

	/*if (fabs(velocity.y) < 0.00001 && keys[GLFW_KEY_SPACE])
	{
		velocity.y += 30;
	}*/

}

/*void ChewyEntity::box_collision(std::shared_ptr<BoundingBox> bb)
{
    shared_ptr<BoundingBox> my_bb = getTransformedOuterBoundingBox();

    float lowest = FLT_MAX;

    glm::vec3 top;
    glm::vec3 bottom;
    int axis;

    if ((my_bb->upper_bound.x - bb->lower_bound.x) < lowest)
    {
        lowest = (my_bb->upper_bound.x - bb->lower_bound.x);
        top = my_bb->upper_bound;
        bottom = bb->lower_bound;
        axis = 1;

    }
    if ((bb->upper_bound.x - my_bb->lower_bound.x) < lowest)
    {
        lowest = (bb->upper_bound.x - my_bb->lower_bound.x);
        top = bb->upper_bound;
        bottom = my_bb->lower_bound;
        axis = 1;
    }
    if ((my_bb->upper_bound.y - bb->lower_bound.y) < lowest)
    {
        lowest = (my_bb->upper_bound.y - bb->lower_bound.y);
        top = my_bb->upper_bound;
        bottom = bb->lower_bound;
        axis = 2;
    }
    if ((bb->upper_bound.y - my_bb->lower_bound.y) < lowest)
    {
        lowest = (bb->upper_bound.y - my_bb->lower_bound.y);
        top = bb->upper_bound;
        bottom = my_bb->lower_bound;
        axis = 2;
    }
    if ((my_bb->upper_bound.z - bb->lower_bound.z) < lowest)
    {
        lowest = (my_bb->upper_bound.z - bb->lower_bound.z);
        top = my_bb->upper_bound;
        bottom = bb->lower_bound;
        axis = 3;
    }
    if ((bb->upper_bound.z - my_bb->lower_bound.z) < lowest)
    {
        lowest = (bb->upper_bound.z - my_bb->lower_bound.z);
        top = bb->upper_bound;
        bottom = my_bb->lower_bound;
        axis = 3;
    }

    //glm::vec3 d = top - bottom;

    switch (axis)
    {
    case 1:
        bottom.y = top.y;
        bottom.z = top.z;
        break;
    case 2:
        bottom.x = top.x;
        bottom.z = top.z;
        break;
    case 3:
        bottom.x = top.x;
        bottom.y = top.y;
        break;
    }

    glm::vec3 n_face = glm::normalize(top - bottom);
    cout << glm::to_string(n_face) << endl;
    n_face.y = 0.f;
    moveComponent.direction -= glm::dot(moveComponent.direction, n_face);
    moveComponent.direction.y = 0.f;
    cout << glm::to_string(moveComponent.direction) << endl;
    position = last_position + moveComponent.direction;
}*/
