#include "BoundingBox.h"
using namespace std;
using namespace glm;
BoundingBox::BoundingBox(glm::vec3 lower_bound, glm::vec3 upper_bound) : lower_bound(lower_bound), upper_bound(upper_bound) {}

shared_ptr<vector<pair<vec3, vec3>>> BoundingBox::get_points(glm::mat4& model)
{
    shared_ptr<vector<pair<vec3, vec3>>> toReturn(new vector<pair<vec3, vec3>>());
    vec3 m_lower_bound = vec3(model * vec4(lower_bound, 1.f));
    vec3 m_upper_bound = vec3(model * vec4(upper_bound, 1.f));

    vec3 d = m_upper_bound - m_lower_bound;
    vec3 current_point = m_lower_bound;
    toReturn->push_back(pair<vec3, vec3>(current_point, vec3(current_point.x, current_point.y, current_point.z + d.z)));
    toReturn->push_back(pair<vec3, vec3>(current_point, vec3(current_point.x, current_point.y + d.y, current_point.z)));
    toReturn->push_back(pair<vec3, vec3>(current_point, vec3(current_point.x + d.x, current_point.y, current_point.z)));

    current_point = vec3(m_lower_bound.x, m_lower_bound.y, m_lower_bound.z + d.z);
    toReturn->push_back(pair<vec3, vec3>(current_point, vec3(current_point.x, current_point.y + d.y, current_point.z)));
    toReturn->push_back(pair<vec3, vec3>(current_point, vec3(current_point.x + d.x, current_point.y, current_point.z)));

    current_point = vec3(m_lower_bound.x, m_lower_bound.y + d.y, m_lower_bound.z);
    toReturn->push_back(pair<vec3, vec3>(current_point, vec3(current_point.x, current_point.y, current_point.z + d.z)));
    toReturn->push_back(pair<vec3, vec3>(current_point, vec3(current_point.x + d.x, current_point.y, current_point.z)));

    current_point = m_upper_bound;
    toReturn->push_back(pair<vec3, vec3>(current_point, vec3(current_point.x, current_point.y, current_point.z - d.z)));
    toReturn->push_back(pair<vec3, vec3>(current_point, vec3(current_point.x, current_point.y - d.y, current_point.z)));
    toReturn->push_back(pair<vec3, vec3>(current_point, vec3(current_point.x - d.x, current_point.y, current_point.z)));

    current_point = vec3(m_upper_bound.x, m_upper_bound.y, m_upper_bound.z - d.z);
    toReturn->push_back(pair<vec3, vec3>(current_point, vec3(current_point.x, current_point.y - d.y, current_point.z)));

    current_point = vec3(m_upper_bound.x, m_upper_bound.y - d.y, m_upper_bound.z);
    toReturn->push_back(pair<vec3, vec3>(current_point, vec3(current_point.x - d.x, current_point.y, current_point.z)));

    return toReturn;

}

