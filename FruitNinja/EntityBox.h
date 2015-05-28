#include <glm/glm.hpp>
#include <memory>
#include <vector>

class EntityBox
{
public:
    glm::vec3 center;
    float half_width;
    float half_height;
    float half_depth;

    EntityBox() : center(0.0f), half_width(0.f), half_height(0.f), half_depth(0.f) {};
    EntityBox(glm::vec3 center, float half_width, float half_height, float half_depth) : center(center), half_width(half_width), half_height(half_height), half_depth(half_depth) {};

    bool box_collision(EntityBox other_box);
    std::shared_ptr<std::vector<std::pair<glm::vec3, glm::vec3>>> get_line_segments();
    glm::vec3 get_lower_bound();
    glm::vec3 get_upper_bound();
};