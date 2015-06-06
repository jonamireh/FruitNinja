#include "EntityBox.h"

using namespace std;
using namespace glm;

bool EntityBox::box_collision(EntityBox other_box)
{
    return (fabs(center.x - other_box.center.x) < half_width + other_box.half_width &&
        fabs(center.y - other_box.center.y) < half_height + other_box.half_height &&
        fabs(center.z - other_box.center.z) < half_depth + other_box.half_depth);
}


vector<pair<vec3, vec3>> EntityBox::get_line_segments()
{
    vector<pair<vec3, vec3>> toReturn;

    vec3 lower_bound = get_lower_bound();
    vec3 upper_bound = get_upper_bound();

    vec3 current_point = lower_bound;
    toReturn.push_back(pair<vec3, vec3>(current_point, vec3(current_point.x, current_point.y, upper_bound.z)));
    toReturn.push_back(pair<vec3, vec3>(current_point, vec3(current_point.x, upper_bound.y, current_point.z)));
    toReturn.push_back(pair<vec3, vec3>(current_point, vec3(upper_bound.x, current_point.y, current_point.z)));

    current_point = vec3(lower_bound.x, lower_bound.y, upper_bound.z);
    toReturn.push_back(pair<vec3, vec3>(current_point, vec3(current_point.x, upper_bound.y, current_point.z)));
    toReturn.push_back(pair<vec3, vec3>(current_point, vec3(upper_bound.x, current_point.y, current_point.z)));

    current_point = vec3(lower_bound.x, upper_bound.y, lower_bound.z);
    toReturn.push_back(pair<vec3, vec3>(current_point, vec3(current_point.x, current_point.y, upper_bound.z)));
    toReturn.push_back(pair<vec3, vec3>(current_point, vec3(upper_bound.x, current_point.y, current_point.z)));

    current_point = upper_bound;
    toReturn.push_back(pair<vec3, vec3>(current_point, vec3(current_point.x, current_point.y, lower_bound.z)));
    toReturn.push_back(pair<vec3, vec3>(current_point, vec3(current_point.x, lower_bound.y, current_point.z)));
    toReturn.push_back(pair<vec3, vec3>(current_point, vec3(lower_bound.x, current_point.y, current_point.z)));

    current_point = vec3(upper_bound.x, upper_bound.y, lower_bound.z);
    toReturn.push_back(pair<vec3, vec3>(current_point, vec3(current_point.x, lower_bound.y, current_point.z)));

    current_point = vec3(upper_bound.x, lower_bound.y, upper_bound.z);
    toReturn.push_back(pair<vec3, vec3>(current_point, vec3(current_point.x, current_point.y, lower_bound.z)));

    return toReturn;
}

vec3 EntityBox::get_lower_bound()
{
    return vec3(center.x - half_width, center.y - half_height, center.z - half_depth);
}

vec3 EntityBox::get_upper_bound()
{
    return vec3(center.x + half_width, center.y + half_height, center.z + half_depth);
}