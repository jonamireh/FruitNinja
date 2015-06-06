#include "OctTree.h"
#include <iostream>
#include <thread> 
using namespace std;

#define MINIMUM_DIMENSION 5.f

OctTree::OctTree()
{
}


void OctTree::handle_collisions()
{
    for (auto it = collision_pairs.begin(); it != collision_pairs.end(); ++it)
    {
        it->first->collision(it->second);
        it->second->collision(it->first);
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

    while (!q.empty())
    {
        MyNode* cur = q.front();
        q.pop();
        min_radius = FLT_MAX;
       /* for (int i = 0; i < cur->objects.size(); i++)
        {
            float temp = cur->objects[i]->getRadius();
            if (temp < min_radius)
                min_radius = temp;
        }*/
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
                vector<GameEntity*> subset_objects;
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
