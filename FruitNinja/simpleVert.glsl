#version 330

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
in vec2 aTextCoord;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

out vec3 vNormals;
out vec2 vTextCoord;

void main()
{
	gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * vec4(aPosition, 1.0);
	vNormals = vec3(uModelMatrix * vec4(normalize(aNormal), 0.0));
	vTextCoord = aTextCoord;
}