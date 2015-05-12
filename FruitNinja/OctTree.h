#define MIN_SIZE 1.0
#include <memory>
#include "GameEntity.h"
#include "Voxel.h"
#include <set>
#include <functional>

class OctTree
{
public:
    Voxel tree_voxel;
    std::vector<std::shared_ptr<GameEntity>> tree_objects;
	std::shared_ptr<std::set<std::pair<std::shared_ptr<GameEntity>, std::shared_ptr<GameEntity>>>> collision_pairs;
    // store the pairs of collisions in here too

    OctTree();
    OctTree(Voxel, std::vector<std::shared_ptr<GameEntity>>);
	void find_collisions();
	void branch(Voxel voxel, std::vector<std::shared_ptr<GameEntity>> objects);
    //std::vector<OctTree> children;
};