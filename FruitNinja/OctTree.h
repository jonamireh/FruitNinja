#define MIN_SIZE 1.0
#include <memory>
#include "GameEntity.h"
#include "Voxel.h"

class OctTree
{
public:
    Voxel voxel;
    std::vector<std::shared_ptr<GameEntity>> objects;
    std::shared_ptr<OctTree> root;
    float min_radius;
    // store the pairs of collisions in here too

    OctTree();
    OctTree(Voxel, std::vector<std::shared_ptr<GameEntity>>, std::shared_ptr<OctTree>);
    void branch();
};