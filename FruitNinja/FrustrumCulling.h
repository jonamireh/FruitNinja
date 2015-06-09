#pragma once
#include <vector>
#include <memory>
#include "GameEntity.h"
#include "World.h"
#include <glm/glm.hpp>
#include <iostream>
#include "ChewyEntity.h"
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_access.inl>

static glm::vec4 normalize_plane(glm::vec4& p1)
{
	glm::vec4 p1_c(p1);
	float mag = glm::length(glm::vec3(p1_c));
	return p1_c / mag;
}
/*
p is normalized
*/
static int PlaneAABBIntersect(EntityBox bb, glm::vec4& p)
{

    glm::vec3 c = bb.center;
	glm::vec3 h = glm::vec3(bb.half_width, bb.half_height, bb.half_depth); // CHECK HERE IF FRUSTUM BREAKS
	float d = abs(glm::dot(glm::vec3(p), c) + p.w);

	float e = h.x * abs(p.x) + h.y * abs(p.y) + h.z * abs(p.z);
	float s = glm::dot(c, glm::vec3(p)) + d;

	if ((s - e) > 0)
		return 0;
	if ((s + e) < 0)
		return 1;
	return 2;
}

static std::vector<GameEntity*> get_objects_in_view(std::vector<GameEntity*> entities, glm::mat4& view_mat, bool chewy_check = false)
{
	glm::vec4 p_planes[6];
	std::vector<GameEntity*> toReturn;
	glm::mat4 combo_mat;
	if (chewy_check)
		combo_mat = guard_projection * view_mat;
	else
		combo_mat = projection * view_mat;

	glm::vec4 columns[4];
	columns[0] = glm::column(combo_mat, 0);
	columns[1] = glm::column(combo_mat, 1);
	columns[2] = glm::column(combo_mat, 2);
	columns[3] = glm::column(combo_mat, 3);

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

	//prelimnary tests show these normals are point outside frustrum, not inside as expected
	PlayerCamera* c_test = dynamic_cast<PlayerCamera*>(camera);
	if (c_test != nullptr && c_test->in_use)
	{
		/*cout << "left: " << glm::to_string(glm::normalize(glm::vec3(p_planes[0]))) << endl;
		cout << "right: " << glm::to_string(glm::normalize(glm::vec3(p_planes[1]))) << endl;
		cout << "top: " << glm::to_string(glm::normalize(glm::vec3(p_planes[2]))) << endl;
		cout << "bottom: " << glm::to_string(glm::normalize(glm::vec3(p_planes[3]))) << endl;
		cout << "near: " << glm::to_string(glm::normalize(glm::vec3(p_planes[4]))) << endl;
		cout << "far: " << glm::to_string(glm::normalize(glm::vec3(p_planes[5]))) << endl;	*/
	}

	bool chewy_not_found = 0;
	int z_test_results = 0;
	int o_test_results = 0;
	int t_test_results = 0;
	int total_checked = 0;
	for (int i = 0; i < entities.size(); i++)
	{
		bool in_frustrum = true;
		GameEntity* entity = entities.at(i);
		if ((chewy_check && HIDE_BEHIND(entity->list)) || !chewy_check)
		{
			for (int j = 0; j < 6; j++)
			{
				int intersection_test = PlaneAABBIntersect(entity->bounding_box, normalize_plane(p_planes[j]));
				/*if (intersection_test == 2)
				{
					in_frustrum = false;
					break;
				}*/
				/*if (!OBB_plane(entity->bounding_box, p_planes[j]))
				{
					in_frustrum = false;
				}*/
				if (!chewy_check)
				{
					j++;
					j--;
				}
			}
			if (in_frustrum)
			{
				toReturn.push_back(entity);
			}
			else
			{
				ChewyEntity* chewy = dynamic_cast<ChewyEntity*>(entity);
				if (chewy != nullptr)
				{
					chewy_not_found = true;
				}
			}
		}
	}
	if (chewy_check && chewy_not_found)
		toReturn.clear();
	return toReturn;
}