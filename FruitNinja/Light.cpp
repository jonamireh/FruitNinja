#include "Light.h"
#include "World.h"



GLuint Light::IND()
{
	return mesh->getMeshes()[0]->IND;
}

GLuint Light::VAO()
{
	return mesh->getMeshes()[0]->VAO;
}

size_t Light::indices_size()
{
	return mesh->getMeshes()[0]->indices.size();
}

glm::mat4 Light::transform()
{
	glm::mat4 t(1.0f);
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), pos);
	return trans*scale;
}

//calculate how big the size needs to be based on intensity
glm::mat4 Light::calc_scale_mat()
{
	float MaxChannel = fmax(fmax(color.x, color.y), color.z);

	float linear = 0.0;
	float exp = 0.3;
	float constant = 0.0;

	float r = (-linear + sqrtf(linear * linear -
		4 * exp * (constant - 256 * MaxChannel * intensity)))
		/ 2 * exp;
	return glm::scale(glm::mat4(1.0f), glm::vec3(r, r, r));
}