
#include "ExplosiveEntity.h"
#include "FireArrowEntity.h"
#include "World.h"
#include "main.h"
#include "GuardEntity.h"

ExplosiveEntity::ExplosiveEntity() {}

ExplosiveEntity::ExplosiveEntity(glm::vec3 position, MeshSet* mesh, float scale_val) : GameEntity(position, mesh)
{
	expScale = scale_val;
	exploding = false;
	ttl = 0.0; //doesn't matter
}

ExplosiveEntity::~ExplosiveEntity() {}

void ExplosiveEntity::update() 
{
	if (exploding) {
		ttl -= seconds_passed;
		if (ttl <= 0.0) {
			list = UNSET_DRAW(list);
		}
	}
}

void ExplosiveEntity::collision(GameEntity* entity) {
	if (!exploding && (typeid(*entity) == typeid(FireArrowEntity) || typeid(*entity) == typeid(ExplosiveEntity))) {
		exploding = true;
		ttl = explosion_length;
		//scaling up box and enabling collision response are a huge performance hit
		bounding_box.half_width *= expScale;
		bounding_box.half_height *= expScale / 2.0;
		bounding_box.half_depth *= expScale;
		collision_response = true;
		world->addExplosion(getPosition());
	}
	if (exploding) {
		GuardEntity* guard = dynamic_cast<GuardEntity*>(entity);
		if (guard && !guard->is_dying()) {
			guard->goAheadAndKillYourself();
		}

	}
}