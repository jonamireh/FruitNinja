#pragma once

#include "ProjectileEntity.h"
#include "ProjectileMovementComponent.h"

class FireArrowEntity : public ProjectileEntity
{
public:
	FireArrowEntity();
	virtual ~FireArrowEntity();
	FireArrowEntity(MeshSet* mesh, Camera* camera);
	void collision(GameEntity* entity) override;
private:

};