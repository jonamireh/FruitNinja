#include "MeshSet.h"

class Light {
public:
	Light(glm::vec3 pos, glm::vec3 color, float intensity, std::shared_ptr<MeshSet> mesh) : pos(pos), color(color), intensity(intensity),
		mesh(mesh), scale(calc_scale_mat()){}
	glm::vec3 pos;
	glm::vec3 color;
	float intensity;
	std::shared_ptr<MeshSet> mesh;
	GLuint IND();
	GLuint VAO();
	size_t indices_size();
	glm::mat4 transform();
private:
	glm::mat4 scale;
	glm::mat4 calc_scale_mat();
};