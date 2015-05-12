#pragma once
#include <memory>
#include <vector>
#include <glm/glm.hpp>

class BoundingBox
{
public:
    glm::vec3 lower_bound;
    glm::vec3 upper_bound;

    BoundingBox();
    BoundingBox(glm::vec3, glm::vec3);
    std::shared_ptr<std::vector<std::pair<glm::vec3, glm::vec3>>> get_line_segments();

    std::vector<std::pair<glm::vec3, glm::vec3>> getPlanes();
    std::vector<std::pair<std::pair<glm::vec3, glm::vec3>, std::pair<glm::vec3, glm::vec3>>> getFaces();
	float getMaxWidth(float protrudingLength);
    std::vector<glm::vec3> get_points();
    bool contains_point(glm::vec3 check_point);
    bool boxes_collide(BoundingBox other_box);
};