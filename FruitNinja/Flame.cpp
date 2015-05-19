#include "Flame.h"

using namespace glm;
using namespace std;

Flame::Flame(glm::vec3 origin, float height, float max_radius) : origin(origin), height(height), max_radius(max_radius)
{
	init();
}

void Flame::init()
{
	for (int i = 0; i < NUM_PARTICLES; i++)
	{
		float y = random_float(0.f, height);
		
		//float input = y * ((2 * M_PI) / height);
		//float radius = max_radius * (cos(1.25 * input) + sin(input - M_PI_2));
		float input = y * -1.f * (M_PI_2 / height);
		float radius = max_radius * cos(input);
		if (input > 0 && input < TURNING_POINT)
			radius *= -1.f;
		
		float theta = random_float(0.f, M_PI * 2.f);
		float phi = random_float(0.f, M_PI);
		
		float x = radius * sin(theta) * sin(phi);
		float z = radius * sin(theta) * cos(phi);	

		pos.push_back(x + origin.x);
		pos.push_back(y + origin.y);
		pos.push_back(z + origin.z);
	}


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Flame::update()
{

}