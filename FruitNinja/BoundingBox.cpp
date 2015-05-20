#include "BoundingBox.h"
#include <iostream>
using namespace std;
using namespace glm;


BoundingBox::BoundingBox(vec3 lower_bound, vec3 upper_bound) : lower_bound(lower_bound), upper_bound(upper_bound) {}

shared_ptr<vector<pair<vec3, vec3>>> BoundingBox::get_line_segments()
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

vector<pair<vec3, vec3>> BoundingBox::getPlanes()
{
    vector<pair<vec3, vec3>> toReturn;
    vector<pair<pair<vec3, vec3>, pair<vec3, vec3>>> faces = getFaces();

    for (int i = 0; i < faces.size(); i++)
    {
        pair<vec3, vec3> set1 = faces.at(i).first;
        pair<vec3, vec3> set2 = faces.at(i).second;
        vec3 orig = (set1.first + set1.second + set2.first + set2.second)/4.f;

        vec3 normal = glm::normalize(glm::cross(set1.first - orig, set1.second - orig));

        toReturn.push_back(pair<vec3, vec3>(orig, glm::normalize(normal)));
    }

    return toReturn;
}

vector<pair<pair<vec3, vec3>, pair<vec3, vec3>>> BoundingBox::getFaces()
{
    vector<pair<pair<vec3, vec3>, pair<vec3, vec3>>> toReturn;

    //left face
    toReturn.push_back(pair<pair<vec3, vec3>, pair<vec3, vec3>>(
		pair<vec3, vec3>(vec3(lower_bound.x, lower_bound.y, upper_bound.z), vec3(lower_bound.x, upper_bound.y, upper_bound.z)),
		pair<vec3, vec3>(vec3(lower_bound.x, upper_bound.y, lower_bound.z), vec3(lower_bound))
		));
    //back face
    toReturn.push_back(pair<pair<vec3, vec3>, pair<vec3, vec3>>(
		pair<vec3, vec3>(vec3(lower_bound), vec3(lower_bound.x, upper_bound.y, lower_bound.z)),
		pair<vec3, vec3>(vec3(upper_bound.x, upper_bound.y, lower_bound.z), vec3(upper_bound.x, lower_bound.y, lower_bound.z))
		));
    //right face
    toReturn.push_back(pair<pair<vec3, vec3>, pair<vec3, vec3>>(
		pair<vec3, vec3>(vec3(upper_bound.x, lower_bound.y, lower_bound.z), vec3(upper_bound.x, upper_bound.y, lower_bound.z)),
		pair<vec3, vec3>(vec3(upper_bound), vec3(upper_bound.x, lower_bound.y, upper_bound.z))
		));
    //bottom face
    toReturn.push_back(pair<pair<vec3, vec3>, pair<vec3, vec3>>(
		pair<vec3, vec3>(vec3(lower_bound.x, lower_bound.y, upper_bound.z), vec3(lower_bound)),
		pair<vec3, vec3>(vec3(upper_bound.x, lower_bound.y, lower_bound.z), vec3(upper_bound.x, lower_bound.y, upper_bound.z))
		));
    //front face
    toReturn.push_back(pair<pair<vec3, vec3>, pair<vec3, vec3>>(
		pair<vec3, vec3>(vec3(upper_bound.x, lower_bound.y, upper_bound.z), vec3(upper_bound)),
		pair<vec3, vec3>(vec3(lower_bound.x, upper_bound.y, upper_bound.z), vec3(lower_bound.x, lower_bound.y, upper_bound.z))
		));
    //top face
    toReturn.push_back(pair<pair<vec3, vec3>, pair<vec3, vec3>>(
		pair<vec3, vec3>(vec3(upper_bound), vec3(upper_bound.x, upper_bound.y, lower_bound.z)),
		pair<vec3, vec3>(vec3(lower_bound.x, upper_bound.y, lower_bound.z), vec3(lower_bound.x, upper_bound.y, upper_bound.z))
		));

    return toReturn;
}

float BoundingBox::getMaxWidth(float protrudingLength)
{
	float toReturn = -FLT_MAX;

	toReturn = upper_bound.x - lower_bound.x > toReturn ? upper_bound.x - lower_bound.x : toReturn;
	toReturn = upper_bound.y - lower_bound.y > toReturn ? upper_bound.y - lower_bound.y : toReturn;
	toReturn = upper_bound.z - lower_bound.z > toReturn ? upper_bound.z - lower_bound.z : toReturn;

	return (toReturn / 2.f) + protrudingLength;

}

vector<vec3> BoundingBox::get_points()
{
	if (cache_points.size() == 0)
	{
		cache_points.push_back(lower_bound);
		cache_points.push_back(vec3(lower_bound.x, lower_bound.y, upper_bound.z));
		cache_points.push_back(vec3(lower_bound.x, upper_bound.y, lower_bound.z));
		cache_points.push_back(vec3(upper_bound.x, lower_bound.y, lower_bound.z));
		cache_points.push_back(upper_bound);
		cache_points.push_back(vec3(upper_bound.x, upper_bound.y, lower_bound.z));
		cache_points.push_back(vec3(upper_bound.x, lower_bound.y, upper_bound.z));
		cache_points.push_back(vec3(lower_bound.x, upper_bound.y, upper_bound.z));
	}
    return cache_points;
}


bool BoundingBox::contains_point(vec3 check_point)
{
    return (check_point.x >= lower_bound.x && check_point.x <= upper_bound.x &&
        check_point.y >= lower_bound.y && check_point.y <= upper_bound.y &&
        check_point.z >= lower_bound.z && check_point.z <= upper_bound.z);
}

bool BoundingBox::boxes_collide(BoundingBox other_box)
{
    vector<vec3> other_box_points = other_box.get_points();
    for (int i = 0; i < other_box_points.size(); i++)
    {
        if (contains_point(other_box_points.at(i)))
            return true;
    }

    vector<vec3> my_points = get_points();
    for (int i = 0; i < my_points.size(); i++)
    {
        if (other_box.contains_point(my_points.at(i)))
            return true;
    }
    return false;
}