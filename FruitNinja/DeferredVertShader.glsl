#version 330

//in vec3 aPosition;
//in vec2 aTextCoord;

layout (location = 0) in vec3 aPosition; 
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextCoord; 

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

out vec2 vTextCoord;
out vec3 vWorldPos;
out vec3 vNormal;

void main()
{
	vWorldPos = vec3(uModelMatrix * vec4(aPosition, 1.0));
	vNormal = vec3(uModelMatrix * vec4(normalize(aNormal), 0.0));
	gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * vec4(aPosition, 1.0);
	vTextCoord = aTextCoord;
}
