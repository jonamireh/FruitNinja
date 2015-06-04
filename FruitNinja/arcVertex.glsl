#version 330

layout(location = 0) in vec3 aPosition;
layout(location = 1) in mat4 aTransMat;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

void main()
{  
	gl_Position = uProjMatrix * uViewMatrix * aTransMat * uModelMatrix * vec4(aPosition, 1.0);
}