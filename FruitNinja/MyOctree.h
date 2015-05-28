#define MIN_SIZE 1.0
#include <memory>
#include "GameEntity.h"
#include "Voxel.h"
#include <set>
#include <functional>
#include <queue>

//decided to not have children because we don't reuse the same tree twice
struct MyNode {
	std::vector<std::shared_ptr<GameEntity>> objects;
	Voxel voxel;

	MyNode(std::vector<std::shared_ptr<GameEntity>> objs, Voxel vox) {
		objects = objs;
		voxel = vox;
	}
};

class MyOctree
{
	void handleNode(MyNode* node);
public:
	std::set<std::pair<std::shared_ptr<GameEntity>, std::shared_ptr<GameEntity>>> collision_pairs;
	void handle_collisions();
	float min_radius;
	// store the pairs of collisions in here too

	MyOctree();
	~MyOctree();
	MyOctree(Voxel param_box, std::vector<std::shared_ptr<GameEntity>> objects_in_section);
};