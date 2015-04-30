#include "OctTree.h"
#include <iostream>

OctTree::OctTree()
{
    
}

OctTree::OctTree(Voxel param_box, std::vector<std::shared_ptr<GameEntity>> objects_in_section, std::shared_ptr<OctTree> root_ref)
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
        root = std::shared_ptr<OctTree>(this);
    else
        root = root_ref;

    branch();
}

/**
This needs to do it based off of bounding boxes rather than points, but this should suffice for now.
*/
void OctTree::branch()
{
    if (glm::distance(voxel.lower_bound, voxel.upper_bound) < 2.0f * min_radius)
    {
        std::cout << "collision!" << std::endl;

        return;
    }

    std::vector<Voxel> subset_voxels = voxel.split();

    for (int i = 0; i < 8; i++)
    {
        std::vector<std::shared_ptr<GameEntity>> subset_objects;

        for (int j = 0; j < objects.size(); j++)
        {
            if (subset_voxels.at(i).contains(objects.at(j)->getCenter(), objects.at(j)->getRadius()))
                subset_objects.push_back(objects.at(j));
        }

        if (subset_objects.size() > 1)
        {
            OctTree(subset_voxels.at(i), subset_objects, root);
        }

    }
}