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
    //std::vector<BoundingBox> split();
    bool contains(glm::vec3);
    std::shared_ptr<std::vector<std::pair<glm::vec3, glm::vec3>>> get_points();
	std::pair<glm::vec3, glm::vec3> getTransformedBounds(glm::mat4& model);
	void applyTransformedBounds(glm::mat4& model);
};