#include "ChewyEntity.h"
#include "PlayerCamera.h"
#include "World.h"
#include "SpikeEntity.h"
#include <glm/gtx/string_cast.inl>
#include <glm/gtx/intersect.hpp>

using namespace glm;

ChewyEntity::ChewyEntity() : moveComponent(*this, new PlayerCamera(),
	new PlayerCamera()), animComponent(this), isCaught(false)
{
	
}

ChewyEntity::ChewyEntity(glm::vec3 position, MeshSet* mesh, Camera* player_cam, Camera* archery_cam)
	: GameEntity(position, mesh, true), moveComponent(*this, player_cam, archery_cam), animComponent(this), isCaught(false)
{
	current_animation = mesh->getAnimations()[0];
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
	GameEntity::update();
}

void ChewyEntity::set_material(Material material)
{
	std::vector<Mesh*> meshes = mesh->getMeshes();
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes.at(i)->mat = material;
	}
}

void ChewyEntity::collision(GameEntity* entity)
{
    // if no collision then stop.
    if (!entity->bounding_box.box_collision(bounding_box))
        return;

    // If you hit a spiky entity... you lost
    SpikeEntity* spike_check = dynamic_cast<SpikeEntity*>(entity);
    if (spike_check != nullptr)
    {
        spike_check->world->lose_condition();
        return;
    }

    vec3 pos = getPosition();


    setPosition(vec3(last_position.x, pos.y, pos.z));

    if (!entity->bounding_box.box_collision(bounding_box))
    {
        return;
    }

    setPosition(vec3(pos.x, pos.y, last_position.z));

    if (!entity->bounding_box.box_collision(bounding_box))
    {
        return;
    }

    setPosition(vec3(pos.x, last_position.y, pos.z));

    if (!entity->bounding_box.box_collision(bounding_box))
    {
        if (entity->bounding_box.center.y > bounding_box.center.y)
            velocity.y = 0.f;
        else
            _falling = false;
        return;
    }



    setPosition(vec3(pos.x, pos.y, last_position.z));
}

std::vector<std::vector<glm::mat4>>* ChewyEntity::getBoneTrans() {
	return animComponent.basicAnimation.getBoneTransformations();
}