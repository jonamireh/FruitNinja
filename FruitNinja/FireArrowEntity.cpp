#include "FireArrowEntity.h"

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