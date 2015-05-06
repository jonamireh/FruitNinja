#include "BoundingBox.h"
#include <iostream>
using namespace std;
using namespace glm;
BoundingBox::BoundingBox(vec3 lower_bound, vec3 upper_bound) : lower_bound(lower_bound), upper_bound(upper_bound) {}

shared_ptr<vector<pair<vec3, vec3>>> BoundingBox::get_points()
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

pair<vec3, vec3> BoundingBox::getTransformedBounds(mat4& model)
{
	vec3 m_lower_bound = vec3(model * vec4(lower_bound, 1.f));
	vec3 m_upper_bound = vec3(model * vec4(upper_bound, 1.f));

	pair<vec3, vec3> toReturn(pair<vec3, vec3>(vec3(FLT_MAX, FLT_MAX, FLT_MAX), vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX)));
	
	vec3 less = lessThan(m_lower_bound, toReturn.first);
	vec3 greater = greaterThan(m_upper_bound, toReturn.second);
	toReturn.first.x = less.x ? m_lower_bound.x : toReturn.first.x;
	toReturn.first.y = less.y ? m_lower_bound.y : toReturn.first.y;
	toReturn.first.z = less.z ? m_lower_bound.z : toReturn.first.z;
	toReturn.second.x = greater.x ? m_upper_bound.x : toReturn.second.x;
	toReturn.second.y = greater.y ? m_upper_bound.y : toReturn.second.y;
	toReturn.second.z = greater.z ? m_upper_bound.z : toReturn.second.z;
	
	vec3 assertTest = equal(m_lower_bound, toReturn.first);
    if (!(assertTest.x && assertTest.y && assertTest.z))
        cout << "what" << endl;
	assert(assertTest.x && assertTest.y && assertTest.z);
	assertTest = equal(m_upper_bound, toReturn.second);
    if (!(assertTest.x && assertTest.y && assertTest.z))
        cout << "what" << endl;
	assert(assertTest.x && assertTest.y && assertTest.z);

	less = lessThan(toReturn.first, m_upper_bound);
	greater = greaterThan(toReturn.second, m_lower_bound);
	toReturn.first.x = less.x ? toReturn.first.x : m_upper_bound.x;
	toReturn.first.y = less.y ? toReturn.first.y : m_upper_bound.y;
	toReturn.first.z = less.z ? toReturn.first.z : m_upper_bound.z;
	toReturn.second.x = greater.x ? toReturn.second.x : m_lower_bound.x;
	toReturn.second.y = greater.y ? toReturn.second.y : m_lower_bound.y;
	toReturn.second.z = greater.z ? toReturn.second.z : m_lower_bound.z;
	
	assertTest = equal(toReturn.first, vec3(FLT_MAX, FLT_MAX, FLT_MAX));
    if ((assertTest.x && assertTest.y && assertTest.z))
        cout << "what" << endl;
	assert(!assertTest.x && !assertTest.y && !assertTest.z);
	assertTest = equal(toReturn.second, vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX));
    if ((assertTest.x && assertTest.y && assertTest.z))
        cout << "what" << endl;
	assert(!assertTest.x && !assertTest.y && !assertTest.z);
	assertTest = lessThan(toReturn.first, toReturn.second);
    if (!(assertTest.x && assertTest.y && assertTest.z))
        cout << "what" << endl;
	assert(assertTest.x && assertTest.y && assertTest.z);

	return toReturn;
}

void BoundingBox::applyTransformedBounds(mat4& model)
{
	pair<vec3, vec3> bounds = getTransformedBounds(model);
	lower_bound = bounds.first;
	upper_bound = bounds.second;

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

        toReturn.push_back(pair<vec3, vec3>(orig, normal));
    }

    return toReturn;
}

vector<pair<pair<vec3, vec3>, pair<vec3, vec3>>> BoundingBox::getFaces()
{
    vector<pair<pair<vec3, vec3>, pair<vec3, vec3>>> toReturn;

    //left face
    toReturn.push_back(pair<pair<vec3, vec3>, pair<vec3, vec3>>(
        pair<vec3, vec3>(vec3(lower_bound), vec3(lower_bound.x, upper_bound.y, lower_bound.z)), 
		pair<vec3, vec3>(vec3(lower_bound.x, upper_bound.y, upper_bound.z), vec3(lower_bound.x, lower_bound.y, upper_bound.z))));
    //back face
    toReturn.push_back(pair<pair<vec3, vec3>, pair<vec3, vec3>>(
		pair<vec3, vec3>(vec3(upper_bound.x, lower_bound.y, lower_bound.z), vec3(upper_bound.x, upper_bound.y, lower_bound.z)),
		pair<vec3, vec3>(vec3(lower_bound.x, upper_bound.y, lower_bound.z), vec3(lower_bound))));
    //right face
    toReturn.push_back(pair<pair<vec3, vec3>, pair<vec3, vec3>>(
		pair<vec3, vec3>(vec3(upper_bound.x, lower_bound.y, upper_bound.z), vec3(upper_bound)),
		pair<vec3, vec3>(vec3(upper_bound.x, upper_bound.y, lower_bound.z), vec3(upper_bound.x, lower_bound.y, lower_bound.z))));
    //bottom face
    toReturn.push_back(pair<pair<vec3, vec3>, pair<vec3, vec3>>(
		pair<vec3, vec3>(vec3(lower_bound), vec3(lower_bound.x, lower_bound.y, upper_bound.z)),
		pair<vec3, vec3>(vec3(upper_bound.x, lower_bound.y, lower_bound.z), vec3(upper_bound.x, lower_bound.y, upper_bound.z))));
    //front face
    toReturn.push_back(pair<pair<vec3, vec3>, pair<vec3, vec3>>(
		pair<vec3, vec3>(vec3(lower_bound.x, lower_bound.y, upper_bound.z), vec3(lower_bound.x, upper_bound.y, upper_bound.z)),
		pair<vec3, vec3>(vec3(upper_bound), vec3(upper_bound.x, lower_bound.y, upper_bound.z))));
    //top face
    toReturn.push_back(pair<pair<vec3, vec3>, pair<vec3, vec3>>(
		pair<vec3, vec3>(vec3(lower_bound.x, upper_bound.y, upper_bound.z), vec3(lower_bound.x, upper_bound.y, lower_bound.z)),
		pair<vec3, vec3>(vec3(upper_bound.x, upper_bound.y, lower_bound.z), vec3(upper_bound))));

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

