#include "MyOctree.h"
#include <iostream>
#include <queue>
using namespace std;

MyOctree::MyOctree()
{
}

void do_collision(std::shared_ptr<GameEntity> ent1, std::shared_ptr<GameEntity> ent2)
{
	ent1->collision(ent2);
	ent2->collision(ent1);
}

void MyOctree::handle_collisions()
{
	for (auto it = collision_pairs.begin(); it != collision_pairs.end(); ++it)
	{
		it->first->collision(it->second);
		it->second->collision(it->first);
	}
}

MyOctree::MyOctree(Voxel vox, vector<shared_ptr<GameEntity>> objects_in_section) {
	vector<shared_ptr<GameEntity>> objs;
	for (int i = 0; i < objects_in_section.size(); i++)
	{
		if (IN_OCTTREE(objects_in_section[i]->list))
			objs.push_back(shared_ptr<GameEntity>(objects_in_section[i]));
	}
	queue<MyNode*> q;
	q.push(new MyNode(objs, vox));

	while (!q.empty()) {
		MyNode* cur = q.front();
		q.pop();
		min_radius = FLT_MAX;
		for (int i = 0; i < cur->objects.size(); i++)
		{
			float temp = cur->objects[i]->getRadius();
			if (temp < min_radius)
				min_radius = temp;
		}
		if (glm::distance(cur->voxel.lower_bound, cur->voxel.upper_bound) < min_radius)
		{
			for (int i = 0; i < cur->objects.size() - 1; i++) {
				//do_collision(cur->objects[i], cur->objects[i + 1]);
				collision_pairs.insert(pair<shared_ptr<GameEntity>, shared_ptr<GameEntity>>(cur->objects.at(i), cur->objects.at(i + 1)));
			}
		}
		else {
			vector<Voxel> subset_voxels = cur->voxel.split();

			for (int i = 0; i < 8; i++)
			{
				bool has_collision_response = false;
				vector<shared_ptr<GameEntity>> subset_objects;
				for (int j = 0; j < cur->objects.size(); j++)
				{
					if (subset_voxels[i].contains(cur->objects[j]->getCenter(), cur->objects[j]->getRadius()))
					{
						if (cur->objects[j]->collision_response)
							has_collision_response = true;
						subset_objects.push_back(cur->objects[j]);
					}
				}

				if (subset_objects.size() > 1 && has_collision_response) {
					q.push(new MyNode(subset_objects, subset_voxels[i]));
				}
			}
			delete cur;
		}
	}

}

MyOctree::~MyOctree() {
}
