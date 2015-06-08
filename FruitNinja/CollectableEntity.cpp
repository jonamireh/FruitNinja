#include "CollectableEntity.h"
#include "ChewyEntity.h"
#include "World.h"
#include "AudioManager.h"

CollectableEntity::CollectableEntity() {

}

CollectableEntity::~CollectableEntity() {

}

CollectableEntity::CollectableEntity(glm::vec3 position, MeshSet* mesh, int numArrows) : GameEntity(position, mesh) {
	center = getPosition();
	number = numArrows;
}

void CollectableEntity::update() {
	total_secs += seconds_passed;
	setPosition(center + glm::vec3(0.0, BOUNCE_HEIGHT * sin(total_secs * BOUNCE_SPEED), 0.0));
	setRotations(glm::vec3(0.0, total_secs * SPIN_SPEED, 0.0));
}

void CollectableEntity::collision(GameEntity* entity) {
	if (typeid(*entity) == typeid(ChewyEntity)) {
		AudioManager::instance()->play3D(assetPath + "Get_Item.wav", center, 10.0f, false);
		arrow_count += number;
		list = UNSET_DRAW(list);
	}
}