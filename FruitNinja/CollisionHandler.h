#pragma once
#include <set>
#include "GameEntity.h"

static void collision_handler(std::shared_ptr<std::set<std::pair<std::shared_ptr<GameEntity>, std::shared_ptr<GameEntity>>>> collision_pairs)
{
	for (auto it = collision_pairs->begin(); it != collision_pairs->end(); ++it)
	{
		it->first->collision(it->second);
		it->second->collision(it->first);
	}
}