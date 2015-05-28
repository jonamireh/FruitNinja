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

void ChewyEntity::collision(std::shared_ptr<GameEntity> entity)
{
    // if no collision then stop.
    if (!entity->bounding_box.box_collision(this->bounding_box))
        return;

    vec3 pos = getPosition();
    setPosition(vec3(pos.x, last_position.y, pos.z));

    if (!entity->bounding_box.box_collision(this->bounding_box))
    {
        _falling = false;
        return;
    }

    setPosition(vec3(last_position.x, pos.y, pos.z));

    if (!entity->bounding_box.box_collision(this->bounding_box))
    {
        return;
    }

    setPosition(vec3(pos.x, pos.y, last_position.z));
}
