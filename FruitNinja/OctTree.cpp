#include "OctTree.h"

OctTree::OctTree()
{
    
}

OctTree::OctTree(BoundingBox param_box, std::vector<std::shared_ptr<GameEntity>> objects_in_section, std::shared_ptr<OctTree> root_ref)
{
    box = param_box;
    objects = objects_in_section;

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
    if (glm::distance(box.lower_bound, box.upper_bound) < MIN_SIZE)
    {
        // run collision check
        return;
    }

    std::vector<BoundingBox> subset_boxes = box.split();

    for (int i = 0; i < 8; i++)
    {
        std::vector<std::shared_ptr<GameEntity>> subset_objects;

        for (int j = 0; j < objects.size(); j++)
        {
            if (subset_boxes.at(i).contains(objects.at(j)->position))
                subset_objects.push_back(objects.at(j));
        }

        if (subset_objects.size() > 1)
            OctTree(subset_boxes.at(i), subset_objects, root);
    }
}