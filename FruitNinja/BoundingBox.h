#include "MeshSet.h"

class BoundingBox
{
public:
    glm::vec3 lower_bound;
    glm::vec3 upper_bound;

    BoundingBox();
    BoundingBox(glm::vec3, glm::vec3);
    std::vector<BoundingBox> split();
    bool contains(glm::vec3);
};