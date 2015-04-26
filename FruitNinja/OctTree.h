#define MIN_SIZE 1
#include <memory>
#include "GameEntity.h"
#include "BoundingBox.h"

class OctTree
{
public:
    BoundingBox box;
    std::vector<std::shared_ptr<GameEntity>> objects;
    std::shared_ptr<OctTree> root;

    // store the pairs of collisions in here too

    OctTree();
    OctTree(BoundingBox, std::vector<std::shared_ptr<GameEntity>>, std::shared_ptr<OctTree>);
    void branch();
};