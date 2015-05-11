#pragma once
#include "MeshSet.h"


class Voxel
{
public:
    glm::vec3 lower_bound;
    glm::vec3 upper_bound;
    glm::vec3 voxel_center;

    Voxel();
    Voxel(glm::vec3, glm::vec3);
    std::vector<Voxel> split();
    bool contains(glm::vec3, float);
    std::shared_ptr<std::vector<std::pair<glm::vec3, glm::vec3>>> get_line_segments();
};