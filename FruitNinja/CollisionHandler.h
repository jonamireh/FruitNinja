#pragma once
#include "GameEntity.h"
#include <set>

static void collision_handler(std::shared_ptr<std::set<std::pair<std::shared_ptr<GameEntity>, std::shared_ptr<GameEntity>>>> collision_pairs)
{
	for (std::set<std::pair<std::shared_ptr<GameEntity>, std::shared_ptr<GameEntity>>>::iterator it = collision_pairs->begin(); it != collision_pairs->end(); ++it)
	{
		(*it).first->collision((*it).second->getTransformedOuterBoundingBox());
		(*it).second->collision((*it).first->getTransformedOuterBoundingBox());
	}
}