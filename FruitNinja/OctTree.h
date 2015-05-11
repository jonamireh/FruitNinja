#define MIN_SIZE 1.0
#include <memory>
#include "GameEntity.h"
#include "Voxel.h"
#include <set>
#include <functional>

class OctTree
{
public:
    Voxel voxel;
    std::vector<std::shared_ptr<GameEntity>> objects;
    std::shared_ptr<OctTree> root;
	std::shared_ptr<std::set<std::pair<std::shared_ptr<GameEntity>, std::shared_ptr<GameEntity>>>> collision_pairs;
    float min_radius;
    // store the pairs of collisions in here too

    OctTree();
    OctTree(Voxel, std::vector<std::shared_ptr<GameEntity>>, std::shared_ptr<OctTree>);
    void branch();
    std::vector<OctTree> children;
};