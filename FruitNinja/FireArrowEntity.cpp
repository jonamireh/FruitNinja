#include "FireArrowEntity.h"
#include "main.h"
#include "LightEntity.h"
#include "CollectableEntity.h"

FireArrowEntity::FireArrowEntity() : ProjectileEntity()
{
}

FireArrowEntity::~FireArrowEntity()
{
	//not called til out of world scope. sort of a mem leak
	//std::cout << "Fire Arrow destroyed!!!!!!!\n";
}



FireArrowEntity::FireArrowEntity(MeshSet* mesh, Camera* camera)
	: ProjectileEntity(mesh, camera)
{
}

void FireArrowEntity::collision(GameEntity* entity) {
	ProjectileEntity::collision(entity);
	//if (typeid(*entity) == typeid(ChewyEntity)) return;

	//if (typeid(*entity) == typeid(LightEntity) && bounding_box.box_collision(entity->inner_bounding_box)) {
	//	world->addExplosion(getPosition());
	//	/*dynamic_cast<LightEntity*>(entity)->light = new Light();
	//	le->setup_inner = false;
	//	le->bounding_box = le->inner_bounding_box;*/
	//}
	//else if (typeid(*entity) != typeid(LightEntity) && typeid(*entity) != typeid(CollectableEntity)) {
	//	world->addExplosion(getPosition());
	//}
}