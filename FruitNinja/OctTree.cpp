#include "OctTree.h"
#include <iostream>
#include "Timer.h"

using namespace std;

OctTree::OctTree()
{
    
}

OctTree::OctTree(Voxel param_box, vector<shared_ptr<GameEntity>> objects_in_section)
{
	tree_objects = objects_in_section;
	tree_voxel = param_box;
	collision_pairs = make_shared<set<pair<shared_ptr<GameEntity>, shared_ptr<GameEntity>>>>();
}

void OctTree::find_collisions()
{
	branch(tree_voxel, tree_objects);
}

/**
This needs to do it based off of bounding boxes rather than points, but this should suffice for now.
*/
void OctTree::branch(Voxel voxel, vector<shared_ptr<GameEntity>> objects)
{
    //Timer timer = Timer();

	float min_radius = FLT_MAX;
	for (int i = 0; i < objects.size(); i++)
	{
		float temp = objects.at(i)->getRadius();
		if (temp < min_radius)
		{
			min_radius = temp;
		}
	}

    if (glm::distance(voxel.lower_bound, voxel.upper_bound) < min_radius)
    {
        for (int i = 0; i < objects.size() - 1; i++)
        {
			collision_pairs->insert(pair<shared_ptr<GameEntity>, shared_ptr<GameEntity>>(objects.at(i), objects.at(i + 1)));
        }
        return;
    }

    vector<Voxel> subset_voxels = voxel.split();

    for (int i = 0; i < 8; i++)
    {
        vector<shared_ptr<GameEntity>> subset_objects;
        int k = 0;
        for (int j = 0; j < objects.size(); j++)
        {
            //timer.start_timing("contains check");
            if (subset_voxels.at(i).contains(objects.at(j)->getCenter(), objects.at(j)->getRadius()))
            {
                //timer.start_timing("pushback check");

                k++;

                subset_objects.push_back(objects.at(j));
                //cout << timer.end_timing() << endl;
            }
            //cout << timer.end_timing() << endl;
        }
        //cout << k << endl;

        if (subset_objects.size() > 1)
        {
            //children.push_back(OctTree(subset_voxels.at(i), subset_objects, root));
            branch(subset_voxels.at(i), subset_objects);
        }
    }
}