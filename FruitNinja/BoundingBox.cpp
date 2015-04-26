#include "BoundingBox.h"

using namespace glm;

BoundingBox::BoundingBox()
{
    
}

BoundingBox::BoundingBox(vec3 lower, vec3 upper)
{
    lower_bound = lower;
    upper_bound = upper;
}

std::vector<BoundingBox> BoundingBox::split()
{
    std::vector<BoundingBox> boxes;

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


    boxes.push_back(BoundingBox(lower_bound, mid_mid_mid));
    boxes.push_back(BoundingBox(mid_mid_front, right_mid_mid));
    boxes.push_back(BoundingBox(left_mid_front, mid_top_back));
    boxes.push_back(BoundingBox(mid_mid_front, right_top_mid));

    boxes.push_back(BoundingBox(left_bottom_mid, mid_mid_back));
    boxes.push_back(BoundingBox(mid_bottom_mid, right_mid_back));
    boxes.push_back(BoundingBox(left_mid_mid, mid_top_back));
    boxes.push_back(BoundingBox(mid_mid_mid, upper_bound));

    return boxes;
}

bool BoundingBox::contains(vec3 point)
{
    return (point.x >= lower_bound.x && point.x <= upper_bound.x &&
        point.y >= lower_bound.y && point.y <= upper_bound.y &&
        point.z >= lower_bound.z && point.z <= upper_bound.z);
}