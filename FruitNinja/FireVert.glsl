#version 330

#define HEIGHT 6
#define WIDTH 6

layout (location = 0) in vec3 aPosition; 
layout (location = 1) in int aFrame;

out vec2 cellPos;

void main()
{
	cellPos = vec2(aFrame % WIDTH / 6.0, aFrame / HEIGHT / 6.0);

	gl_Position = vec4(aPosition, 1.0);
}
