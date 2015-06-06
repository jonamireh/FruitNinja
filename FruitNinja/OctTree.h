#define MIN_SIZE 1.0
#include <memory>
#include "GameEntity.h"
#include "Voxel.h"
#include <set>
#include <functional>
#include <queue>

//decided to not have children because we don't reuse the same tree twice
struct MyNode
{
    std::vector<GameEntity*> objects;
    Voxel voxel;

    MyNode(std::vector<GameEntity*> objs, Voxel vox)
    {
        objects = objs;
        voxel = vox;
    }
};

class OctTree
{
public:
    std::set<std::pair<GameEntity*, GameEntity*>> collision_pairs;
    void handle_collisions();
    float min_radius;

    OctTree();
    ~OctTree();
    OctTree(Voxel param_box, std::vector<GameEntity*> objects_in_section);
};