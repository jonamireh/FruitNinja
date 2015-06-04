#version 330

layout (location = 0) in vec3 aPosition; 
layout (location = 1) in int aFrame;

uniform int uWidth;
uniform int uHeight;

out vec2 cellPos;

void main()
{
	cellPos = vec2(aFrame % uWidth / float(uWidth), aFrame / uHeight / float(uWidth));

	gl_Position = vec4(aPosition, 1.0);
}
