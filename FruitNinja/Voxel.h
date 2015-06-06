#pragma once
#include "MeshSet.h"
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "GameEntity.h"

class Voxel
{
public:
    glm::vec3 lower_bound;
    glm::vec3 upper_bound;
    glm::vec3 voxel_center;
    EntityBox voxel_entity_box;

    Voxel();
    Voxel(glm::vec3, glm::vec3);
    std::vector<Voxel> split();
    bool contains(GameEntity* entity);
    std::vector<std::pair<glm::vec3, glm::vec3>> get_line_segments();
};