#version 150

in vec3 aPosition;
uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;

void main()
{
	gl_Position = uProjMatrix * uViewMatrix * vec4(aPosition, 1.0);
}