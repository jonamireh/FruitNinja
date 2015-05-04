#pragma once
#include "GameEntity.h"

static void collision_handler(std::shared_ptr<std::vector<std::pair<std::shared_ptr<GameEntity>, std::shared_ptr<GameEntity>>>> collision_pairs)
{
    for (int i = 0; i < collision_pairs->size(); i++)
    {
		collision_pairs->at(i).first->collision(collision_pairs->at(i).second->getOuterBoundingBox());
		collision_pairs->at(i).second->collision(collision_pairs->at(i).first->getOuterBoundingBox());
    }
}