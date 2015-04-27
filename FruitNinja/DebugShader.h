#include <glm/glm.hpp>
#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class DebugShader : public Shader
{
	GLuint VAO;
	GLuint VBO;

	std::vector<VertexData> data;

public:
	DebugShader(std::string vertShader, std::string fragShader);
	void drawLine(glm::vec3 p1, glm::vec3 p2, glm::vec3 color, glm::mat4& view_mat);
};