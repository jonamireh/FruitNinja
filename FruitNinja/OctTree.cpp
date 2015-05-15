#include "OctTree.h"
#include <iostream>
#include "Timer.h"

using namespace std;

OctTree::OctTree()
{
    
}

OctTree::OctTree(Voxel param_box, vector<shared_ptr<GameEntity>> objects_in_section, shared_ptr<OctTree> root_ref)
{
    voxel = param_box;
    objects = objects_in_section;
    min_radius = FLT_MAX;
    for (int i = 0; i < objects.size(); i++)
    {
        float temp = objects.at(i)->getRadius();
        if (temp < min_radius)
        {
            min_radius = temp;
        }
    }

    if (root_ref == nullptr)
    {
        root = shared_ptr<OctTree>(this);
		collision_pairs = shared_ptr<set<pair<shared_ptr<GameEntity>, shared_ptr<GameEntity>>>>(new set<pair<shared_ptr<GameEntity>, shared_ptr<GameEntity>>>());
    }
    else
        root = root_ref;

    branch();
}

/**
This needs to do it based off of bounding boxes rather than points, but this should suffice for now.
*/
void OctTree::branch()
{
    Timer timer = Timer();


    if (glm::distance(voxel.lower_bound, voxel.upper_bound) < min_radius)
    {
        for (int i = 0; i < objects.size() - 1; i++)
        {
			root->collision_pairs->insert(pair<shared_ptr<GameEntity>, shared_ptr<GameEntity>>(objects.at(i), objects.at(i + 1)));
        }
        return;
    }

    vector<Voxel> subset_voxels = voxel.split();

    for (int i = 0; i < 8; i++)
    {
        bool has_collision_response = false;
        vector<shared_ptr<GameEntity>> subset_objects;
        for (int j = 0; j < objects.size(); j++)
        {
            if (subset_voxels.at(i).contains(objects.at(j)->getCenter(), objects.at(j)->getRadius()))
            {
                if (objects.at(j)->collision_response)
                    has_collision_response = true;
                subset_objects.push_back(objects.at(j));
            }
        }

        if (subset_objects.size() > 1 && has_collision_response)
        {
            OctTree(subset_voxels.at(i), subset_objects, root);
        }
    }
}