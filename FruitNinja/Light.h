#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>
#include "MeshSet.h"

class Light {
public:
	Light(glm::vec3 pos, glm::vec3 color, float intensity, MeshSet* mesh);
	glm::vec3 pos;
	glm::vec3 color;
	float intensity;
	MeshSet* mesh;
	GLuint IND();
	GLuint VAO();
	size_t indices_size();
	glm::mat4 transform();
	glm::mat4 _transform;
	glm::mat4 calc_scale_mat();
private:
	
	

	GLuint _IND;
	GLuint _VAO;
	size_t _ind_size;
	
};