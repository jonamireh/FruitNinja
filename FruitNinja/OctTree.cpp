#include "OctTree.h"
#include <iostream>
#include <thread> 
#include "ChewyEntity.h"
#include "PlatformEntity.h"
using namespace std;

#define MINIMUM_DIMENSION 5.f

OctTree::OctTree()
{
}


void OctTree::handle_collisions()
{
	/*if (collision_pairs.size() > 1) {
		std::cout << "Collisions this frame: " << collision_pairs.size() << std::endl;
	}*/
    for (auto it = collision_pairs.begin(); it != collision_pairs.end(); ++it)
    {
        if (it->first->bounding_box.box_collision(it->second->bounding_box))
        {
            it->first->collision(it->second);
            it->second->collision(it->first);
        }
        else if (typeid(PlatformEntity) == typeid(*(it->first)) || typeid(PlatformEntity) == typeid(*(it->second)))
        {
            EntityBox delta_check = it->first->bounding_box;
            delta_check.half_height += 0.1f;
            if (delta_check.box_collision(it->second->bounding_box))
            {
                it->first->collision(it->second);
                it->second->collision(it->first);
            }
        }
    }
}

OctTree::OctTree(Voxel vox, vector<GameEntity*> objects_in_section)
{
    vector<GameEntity*> objs;
    for (int i = 0; i < objects_in_section.size(); i++)
    {
        if (IN_OCTTREE(objects_in_section[i]->list))
            objs.push_back(objects_in_section[i]);
    }
    queue<MyNode*> q;
    q.push(new MyNode(objs, vox));
	vector<GameEntity*> subset_objects;
	subset_objects.reserve(objects_in_section.size());

    while (!q.empty())
    {
        MyNode* cur = q.front();
        q.pop();
        if (cur->voxel.upper_bound.x - cur->voxel.lower_bound.x <= MINIMUM_DIMENSION)
        {
            for (int i = 0; i < cur->objects.size() - 1; i++)
            {
                for (int j = i + 1; j < cur->objects.size(); j++)
                    collision_pairs.insert(pair<GameEntity*, GameEntity*>(cur->objects.at(i), cur->objects.at(j)));
            }
        }
        else
        {
            vector<Voxel> subset_voxels = cur->voxel.split();

            for (int i = 0; i < 8; i++)
            {
                bool has_collision_response = false;
				subset_objects.clear();
				for (int j = 0; j < cur->objects.size(); j++)
                {
                    if (subset_voxels[i].contains(cur->objects[j]))
                    {
                        if (cur->objects[j]->collision_response)
                            has_collision_response = true;
                        subset_objects.push_back(cur->objects[j]);
                    }
                }

                if (subset_objects.size() > 1 && has_collision_response)
                {
                    q.push(new MyNode(subset_objects, subset_voxels[i]));
                }
            }
        }
        delete cur;
    }

}

OctTree::~OctTree()
{
}
