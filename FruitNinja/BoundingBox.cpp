#include "BoundingBox.h"
#include <iostream>
using namespace std;
using namespace glm;
BoundingBox::BoundingBox(glm::vec3 lower_bound, glm::vec3 upper_bound) : lower_bound(lower_bound), upper_bound(upper_bound) {}

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

pair<vec3, vec3> BoundingBox::getTransformedBounds(glm::mat4& model)
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
	assert(assertTest.x && assertTest.y && assertTest.z);
	assertTest = equal(m_upper_bound, toReturn.second);
	assert(assertTest.x && assertTest.y && assertTest.z);

	less = lessThan(toReturn.first, m_upper_bound);
	greater = greaterThan(toReturn.second, m_lower_bound);
	toReturn.first.x = less.x ? toReturn.first.x : m_upper_bound.x;
	toReturn.first.y = less.y ? toReturn.first.y : m_upper_bound.y;
	toReturn.first.z = less.z ? toReturn.first.z : m_upper_bound.z;
	toReturn.second.x = greater.x ? toReturn.second.x : m_lower_bound.x;
	toReturn.second.y = greater.y ? toReturn.second.y : m_lower_bound.y;
	toReturn.second.z = greater.z ? toReturn.second.z : m_lower_bound.z;
	
	/*
	//lb.x lb.y lb.z
	vec3 temp = m_lower_bound;
	vec3 result = lessThan(temp, toReturn.first);
	toReturn.first = result.x && result.y && result.z ? temp : toReturn.first;

	result = greaterThan(temp, toReturn.second);
	toReturn.second = result.x && result.y && result.z ? temp : toReturn.second;

	//ub.x, lb.y, lb.z
	temp = vec3(m_upper_bound.x, m_lower_bound.y, m_lower_bound.z);
	result = lessThan(temp, toReturn.first);
	toReturn.first = result.x && result.y && result.z ? temp : toReturn.first;

	result = greaterThan(temp, toReturn.second);
	toReturn.second = result.x && result.y && result.z ? temp : toReturn.second;

	//lb.x, up.y, lb.z
	//temp.x = m_lower_bound.x;
	//temp.y = m_upper_bound.y;
	temp = vec3(m_lower_bound.x, m_upper_bound.y, m_lower_bound.z);
	result = lessThan(temp, toReturn.first);
	toReturn.first = result.x && result.y && result.z ? temp : toReturn.first;

	result = greaterThan(temp, toReturn.second);
	toReturn.second = result.x && result.y && result.z ? temp : toReturn.second;

	//lb.x, lb.y, ub.z
	//temp.y = m_lower_bound.y;
	//temp.z = m_upper_bound.z;
	temp = vec3(m_lower_bound.x, m_lower_bound.y, m_upper_bound.z);
	result = lessThan(temp, toReturn.first);
	toReturn.first = result.x && result.y && result.z ? temp : toReturn.first;

	result = greaterThan(temp, toReturn.second);
	toReturn.second = result.x && result.y && result.z ? temp : toReturn.second;

	//ub.x, ub.y, lb.z
	//temp.x = m_upper_bound.x;
	//temp.y = m_upper_bound.y;
	//temp.z = m_lower_bound.z;
	temp = vec3(m_upper_bound.x, m_upper_bound.y, m_lower_bound.z);
	result = lessThan(temp, toReturn.first);
	toReturn.first = result.x && result.y && result.z ? temp : toReturn.first;

	result = greaterThan(temp, toReturn.second);
	toReturn.second = result.x && result.y && result.z ? temp : toReturn.second;

	//ub.x, lb.y, ub.z
	//temp.y = m_lower_bound.y;
	//temp.z = m_upper_bound.z;
	temp = vec3(m_upper_bound.x, m_lower_bound.y, m_upper_bound.z);
	result = lessThan(temp, toReturn.first);
	toReturn.first = result.x && result.y && result.z ? temp : toReturn.first;

	result = greaterThan(temp, toReturn.second);
	toReturn.second = result.x && result.y && result.z ? temp : toReturn.second;

	//lb.x, up.y, ub.z
	//temp.x = m_lower_bound.x;
	//temp.y = m_upper_bound.y;
	temp = vec3(m_lower_bound.x, m_upper_bound.y, m_upper_bound.z);
	result = lessThan(temp, toReturn.first);
	toReturn.first = result.x && result.y && result.z ? temp : toReturn.first;

	result = greaterThan(temp, toReturn.second);
	toReturn.second = result.x && result.y && result.z ? temp : toReturn.second;

	//ub.x, ub.y, ub.z
	temp = m_upper_bound;
	result = lessThan(temp, toReturn.first);
	toReturn.first = result.x && result.y && result.z ? temp : toReturn.first;

	result = greaterThan(temp, toReturn.second);
	toReturn.second = result.x && result.y && result.z ? temp : toReturn.second;
	*/
	assertTest = equal(toReturn.first, vec3(FLT_MAX, FLT_MAX, FLT_MAX));
	assert(!assertTest.x && !assertTest.y && !assertTest.z);
	assertTest = equal(toReturn.second, vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX));
	assert(!assertTest.x && !assertTest.y && !assertTest.z);
	assertTest = lessThan(toReturn.first, toReturn.second);
	assert(assertTest.x && assertTest.y && assertTest.z);

	return toReturn;
}

void BoundingBox::applyTransformedBounds(glm::mat4& model)
{
	pair<vec3, vec3> bounds = getTransformedBounds(model);
	lower_bound = bounds.first;
	upper_bound = bounds.second;

}

