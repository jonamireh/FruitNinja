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

std::vector<std::shared_ptr<GameEntity>> cull_objects(std::vector<std::shared_ptr<GameEntity>> entities, glm::mat4& view_mat)
{
	glm::vec4 p_planes[6];
	std::vector<std::shared_ptr<GameEntity>> toReturn;
	glm::mat4 combo_mat = glm::perspective((float)glm::radians(45.0), screen_width / screen_height, 0.1f, 800.f) * view_mat;

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
		glm::vec4 center = glm::vec4(entities.at(i)->getCenter(), 1.0f);
		for (int j = 0; j < 6; j++)
		{
			//float point = classify_point(p_planes[j], glm::vec4(center, 1.0f));
			float distance = glm::dot(normalize_plane(p_planes[j]), center);
			if (distance < 0.f && abs(distance + 2.0f) > entities.at(i)->getRadius())
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