#include "ChewyEntity.h"
#include "PlayerCamera.h"
#include "World.h"


ChewyEntity::ChewyEntity() : moveComponent(*this, std::shared_ptr<Camera>(new PlayerCamera()))
{
	
}

ChewyEntity::ChewyEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh, std::shared_ptr<Camera> camera) : GameEntity(position, mesh), moveComponent(*this, camera)
{
	
}

void ChewyEntity::update()
{
	moveComponent.update();
}

void ChewyEntity::collision(std::shared_ptr<BoundingBox> bb)
{
	shared_ptr<BoundingBox> my_bb = getOuterBoundingBox();
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

	glm::vec3 d = top - bottom;

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
	
	/*if (glm::dot(n_face, glm::normalize(moveComponent.movement)) > glm::dot(-n_face, glm::normalize(moveComponent.movement)))
	{
		n_face = -n_face;
	}*/

	n_face.y = 0.f;
	moveComponent.movement.y = 0.f;
	moveComponent.movement -= glm::dot(moveComponent.movement, n_face);
	position = last_position + moveComponent.movement;
}