#include "MeshSet.h"

class Voxel
{
public:
    glm::vec3 lower_bound;
    glm::vec3 upper_bound;

    Voxel();
    Voxel(glm::vec3, glm::vec3);
    std::vector<Voxel> split();
    bool contains(glm::vec3, float);
};