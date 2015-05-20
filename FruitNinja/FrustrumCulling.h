#pragma once
#include <vector>
#include <memory>
#include "GameEntity.h"
#include "World.h"
#include <glm/glm.hpp>
#include <iostream>

glm::vec4 normalize_plane(glm::vec4& p1)
{
	glm::vec4 p1_c(p1);
	float mag = glm::length(glm::vec3(p1_c));
	return p1_c / mag;
}

int PlaneAABBIntersect(shared_ptr<BoundingBox> bb, glm::vec4& p)
{
	glm::vec3 c = (bb->upper_bound + bb->lower_bound) / 2.f;
	glm::vec3 h = (bb->upper_bound - bb->lower_bound) / 2.f;

	float e = h.x * fabs(p.x) + h.y * fabs(p.y) + h.z * fabs(p.z);
	float s = glm::dot(c, glm::vec3(p)) + p.w;

	if ((s - e) > 0)
		return 0;
	if ((s + e) > 0)
		return 1;
	return 2;
}

std::vector<std::shared_ptr<GameEntity>> get_objects_in_view(std::vector<std::shared_ptr<GameEntity>> entities, glm::mat4& view_mat)
{
	glm::vec4 p_planes[6];
	std::vector<std::shared_ptr<GameEntity>> toReturn;
	glm::mat4 combo_mat = projection * view_mat;

	//left clipping plane
	p_planes[0].x = combo_mat[0][3] + combo_mat[0][0];
	p_planes[0].y = combo_mat[1][3] + combo_mat[1][0];
	p_planes[0].z = combo_mat[2][3] + combo_mat[2][0];
	p_planes[0].w = combo_mat[3][3] + combo_mat[3][0];
	// Right clipping plane
	p_planes[1].x = combo_mat[0][3] - combo_mat[0][0];
	p_planes[1].y = combo_mat[1][3] - combo_mat[1][0];
	p_planes[1].z = combo_mat[2][3] - combo_mat[2][0];
	p_planes[1].w = combo_mat[3][3] - combo_mat[3][0];
	// Top clipping plane
	p_planes[2].x = combo_mat[0][3] - combo_mat[0][1];
	p_planes[2].y = combo_mat[1][3] - combo_mat[1][1];
	p_planes[2].z = combo_mat[2][3] - combo_mat[2][1];
	p_planes[2].w = combo_mat[3][3] - combo_mat[3][1];
	// Bottom clipping plane
	p_planes[3].x = combo_mat[0][3] + combo_mat[0][1];
	p_planes[3].y = combo_mat[1][3] + combo_mat[1][1];
	p_planes[3].z = combo_mat[2][3] + combo_mat[2][1];
	p_planes[3].w = combo_mat[3][3] + combo_mat[3][1];
	// Near clipping plane
	p_planes[4].x = combo_mat[0][3] + combo_mat[0][2];
	p_planes[4].y = combo_mat[1][3] + combo_mat[1][2];
	p_planes[4].z = combo_mat[2][3] + combo_mat[2][2];
	p_planes[4].w = combo_mat[3][3] + combo_mat[3][2];
	// Far clipping plane
	p_planes[5].x = combo_mat[0][3] - combo_mat[0][2];
	p_planes[5].y = combo_mat[1][3] - combo_mat[1][2];
	p_planes[5].z = combo_mat[2][3] - combo_mat[2][2];
	p_planes[5].w = combo_mat[3][3] - combo_mat[3][2];

	for (int i = 0; i < entities.size(); i++)
	{
		bool in_frustrum = true;
		for (int j = 0; j < 6; j++)
		{
			if (PlaneAABBIntersect(entities.at(i)->getTransformedOuterBoundingBox(), p_planes[j]) == 2)
			{
				in_frustrum = false;
				break;
			}
		}
		if (in_frustrum)
		{
			toReturn.push_back(entities.at(i));
		}
	}
	return toReturn;
}