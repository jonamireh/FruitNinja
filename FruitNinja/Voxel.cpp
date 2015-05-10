#include "Voxel.h"

using namespace glm;

Voxel::Voxel()
{
    
}

Voxel::Voxel(vec3 lower, vec3 upper)
{
    lower_bound = lower;
    upper_bound = upper;
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

bool Voxel::contains(vec3 point, float radius)
{
    vec3 center = vec3(0.5f * (lower_bound.x + upper_bound.x), 0.5f * (lower_bound.y + upper_bound.y), 0.5f * (lower_bound.z + upper_bound.z));
    return (point.x >= lower_bound.x && point.x <= upper_bound.x &&
        point.y >= lower_bound.y && point.y <= upper_bound.y &&
        point.z >= lower_bound.z && point.z <= upper_bound.z) || distance(center, point) < radius;
}

//std::vector<std::pair<vec3, vec3>>