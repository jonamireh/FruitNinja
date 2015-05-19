#include "Flame.h"
#include <iostream>

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
		//float input = y * -1.f * (M_PI_2 / height);
		float input = -y + height;
		float pInput = pow(input, (1.f / 2));
		float pHeight = pow(height, (1.f / 2));
		float radius = (pInput / pHeight) * max_radius;
		//if (input > 0 && input < TURNING_POINT)
		//radius *= -1.f;

		float theta = random_float(0.f, M_PI * 2.f);
		float phi = random_float(0.f, M_PI);

		float x = radius * sin(theta) * sin(phi);
		float z = radius * sin(theta) * cos(phi);

		pos.push_back(x + origin.x);
		pos.push_back(y + origin.y);
		pos.push_back(z + origin.z);

		int branch = 0;


		float hPrime = height / 4.f;
		float rPrime = max_radius / 4.f;
		float k = hPrime / pow(rPrime, 2.f);

		float r, g, b, a;
		float temp;
		//bottom color
		if (sqrt((-y / k) + (height / k)) < radius)
		{
			//#000000
			b = g = r = 0.f;
			//#215174
			temp = pow((radius / rPrime), 2.f);
			r += 0.13 * temp;
			g += 0.32 * temp;
			b += 0.45 * temp;
			branch = 0;
		}
		//second from the bottom
		else if ((hPrime = height / 2.f) && (rPrime = max_radius / 2.f) && (k = hPrime / pow(rPrime, 2.f)) && sqrt((-y / k) + (height / k)) < radius)
		{
			//#000000
			r = 0.13f;
			g = 0.32f;
			b = 0.45f;
			//#B6712B
			temp = pow((radius / rPrime), 2.f);
			r += 0.58 * temp;
			g += 0.12 * temp;
			b -= 0.28 * temp;
			branch = 1;
		}
		//second from the top
		else if ((hPrime = (3.f * height) / 4.f) && (rPrime = (3.f * max_radius) / 4.f) && (k = hPrime / pow(rPrime, 2.f)) && sqrt((-y / k) + (height / k)) < radius)
		{
			//#B6712B
			r = 0.71f;
			g = 0.44f;
			b = 0.17f;
			//#DABC2F
			temp = pow((radius / rPrime), 2.f);
			r += 0.14 * temp;
			g += 0.3 * temp;
			b += 0.01 * temp;
			branch = 2;

		}
		//top color
		else
		{
			//#DABC2F
			r = 0.85f;
			g = 0.74f;
			b = 0.18f;
			//#F4ECC7
			temp = pow((radius / rPrime), 2.f);
			r += 0.11 * temp;
			g += 0.18 * temp;
			b += 0.6 * temp;
			branch = 3;
		}
		a = 1.f;
		if ((radius / max_radius) <= 0.75)
		{
			a -= 1.f - log((radius / max_radius) + M_E);
		}
		else
		{
			a -= (2.f / 3.f) * (M_E - log(0.75 + M_E));
		}

		if (r > 1.f)
			cout << "what";
		if (g > 1.f)
			cout << "what";
		if (b > 1.f)
			cout << "what";
		if (a > 1.f)
			cout << "what";


		assert(r <= 1.f);
		assert(g <= 1.f);
		assert(b <= 1.f);
		assert(a <= 1.f);

		col.push_back(r);
		col.push_back(g);
		col.push_back(b);
		col.push_back(a);

		//cout << r << " " << g << " " << b << " " << a << endl;
	}


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &pVBO);
	glGenBuffers(1, &cVBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, pVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cVBO);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Flame::update()
{

}