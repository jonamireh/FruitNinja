#include "Voxel.h"
#include "World.h"

using namespace glm;
using namespace std;
Voxel::Voxel()
{
    
}

Voxel::Voxel(vec3 lower, vec3 upper)
{
    lower_bound = lower;
    upper_bound = upper;
    voxel_center = vec3(0.5f * (lower_bound.x + upper_bound.x), 0.5f * (lower_bound.y + upper_bound.y), 0.5f * (lower_bound.z + upper_bound.z));

    shared_ptr<vector<pair<vec3, vec3>>> points = get_line_segments();
    for (int j = 0; j < points->size(); j++)
    {
       World::draw_line(points->at(j).first, points->at(j).second, vec3(1.f, 1.f, 1.f));
    }
}

shared_ptr<vector<pair<vec3, vec3>>> Voxel::get_line_segments()
{
    shared_ptr<vector<pair<vec3, vec3>>> toReturn(new vector<pair<vec3, vec3>>());

    vec3 current_point = lower_bound;
    toReturn->push_back(pair<vec3, vec3>(current_point, vec3(current_point.x, current_point.y, upper_bound.z)));
    toReturn->push_back(pair<vec3, vec3>(current_point, vec3(current_point.x, upper_bound.y, current_point.z)));
    toReturn->push_back(pair<vec3, vec3>(current_point, vec3(upper_bound.x, current_point.y, current_point.z)));

    current_point = vec3(lower_bound.x, lower_bound.y, upper_bound.z);
    toReturn->push_back(pair<vec3, vec3>(current_point, vec3(current_point.x, upper_bound.y, current_point.z)));
    toReturn->push_back(pair<vec3, vec3>(current_point, vec3(upper_bound.x, current_point.y, current_point.z)));

    current_point = vec3(lower_bound.x, upper_bound.y, lower_bound.z);
    toReturn->push_back(pair<vec3, vec3>(current_point, vec3(current_point.x, current_point.y, upper_bound.z)));
    toReturn->push_back(pair<vec3, vec3>(current_point, vec3(upper_bound.x, current_point.y, current_point.z)));

    current_point = upper_bound;
    toReturn->push_back(pair<vec3, vec3>(current_point, vec3(current_point.x, current_point.y, lower_bound.z)));
    toReturn->push_back(pair<vec3, vec3>(current_point, vec3(current_point.x, lower_bound.y, current_point.z)));
    toReturn->push_back(pair<vec3, vec3>(current_point, vec3(lower_bound.x, current_point.y, current_point.z)));

    current_point = vec3(upper_bound.x, upper_bound.y, lower_bound.z);
    toReturn->push_back(pair<vec3, vec3>(current_point, vec3(current_point.x, lower_bound.y, current_point.z)));

    current_point = vec3(upper_bound.x, lower_bound.y, upper_bound.z);
    toReturn->push_back(pair<vec3, vec3>(current_point, vec3(current_point.x, current_point.y, lower_bound.z)));

    return toReturn;

}


std::vector<Voxel> Voxel::split()
{
    std::vector<Voxel> boxes;

    //    X     Y    Z
    // front face
    vec3 mid_bottom_front = vec3(0.5f * (lower_bound.x + upper_bound.x), lower_bound.y, lower_bound.z);
    vec3 left_mid_front = vec3(lower_bound.x, 0.5f * (lower_bound.y + upper_bound.y), lower_bound.z);
    vec3 mid_mid_front = vec3(0.5f * (lower_bound.x + upper_bound.x), 0.5f * (lower_bound.y + upper_bound.y), lower_bound.z);
    // mid face
    vec3 left_bottom_mid = vec3(lower_bound.x, lower_bound.y, 0.5f * (lower_bound.z + upper_bound.z));
    vec3 mid_bottom_mid = vec3(0.5f * (lower_bound.x + upper_bound.x), lower_bound.y, 0.5f * (lower_bound.z + upper_bound.z));
    vec3 left_mid_mid = vec3(lower_bound.x, 0.5f * (lower_bound.y + upper_bound.y), 0.5f * (lower_bound.z + upper_bound.z));
    vec3 right_mid_mid = vec3(upper_bound.x, 0.5f * (lower_bound.y + upper_bound.y), 0.5f * (lower_bound.z + upper_bound.z));
    vec3 mid_mid_mid = vec3(0.5f * (lower_bound.x + upper_bound.x), 0.5f * (lower_bound.y + upper_bound.y), 0.5f * (lower_bound.z + upper_bound.z));
    vec3 mid_top_mid = vec3(0.5f * (lower_bound.x + upper_bound.x), upper_bound.y, 0.5f * (lower_bound.z + upper_bound.z));
    vec3 right_top_mid = vec3(upper_bound.x, upper_bound.y, 0.5f * (lower_bound.z + upper_bound.z));
    // back face
    vec3 mid_mid_back = vec3(0.5f * (lower_bound.x + upper_bound.x), 0.5f * (lower_bound.y + upper_bound.y), upper_bound.z);
    vec3 mid_top_back = vec3(0.5f * (lower_bound.x + upper_bound.x), upper_bound.y, upper_bound.z);
    vec3 right_mid_back = vec3(upper_bound.x, 0.5f * (lower_bound.y + upper_bound.y), upper_bound.z);


    boxes.push_back(Voxel(lower_bound, mid_mid_mid));
    boxes.push_back(Voxel(mid_bottom_front, right_mid_mid));
    boxes.push_back(Voxel(left_mid_front, mid_top_mid));
    boxes.push_back(Voxel(mid_mid_front, right_top_mid));

    boxes.push_back(Voxel(left_bottom_mid, mid_mid_back));
    boxes.push_back(Voxel(mid_bottom_mid, right_mid_back));
    boxes.push_back(Voxel(left_mid_mid, mid_top_back));
    boxes.push_back(Voxel(mid_mid_mid, upper_bound));

    return boxes;
}

bool Voxel::contains(vec3 object_center, float radius)
{
    vec3 point = object_center + radius * normalize(voxel_center - object_center);
    return (point.x >= lower_bound.x && point.x <= upper_bound.x &&
        point.y >= lower_bound.y && point.y <= upper_bound.y &&
        point.z >= lower_bound.z && point.z <= upper_bound.z) ||
        (object_center.x >= lower_bound.x && object_center.x <= upper_bound.x &&
        object_center.y >= lower_bound.y && object_center.y <= upper_bound.y &&
        object_center.z >= lower_bound.z && object_center.z <= upper_bound.z);
        
        
        //glm::distance(voxel_center, object_center) < radius;
}
