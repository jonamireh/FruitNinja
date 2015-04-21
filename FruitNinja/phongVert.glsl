#version 150

in vec3 aPosition;
in vec3 aNormal;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

uniform vec3 uLightPos;
uniform vec3 UaColor;
uniform vec3 UdColor;
uniform vec3 UsColor;
uniform float Ushine;

out vec3 vNormals;
out vec3 vLight;

void main()
{
	gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * vec4(aPosition, 1.0);
	vNormals = vec3(uModelMatrix * vec4(normalize(aNormal), 0.0));
	vLight = uLightPos - vec3(uModelMatrix * vec4(aPosition, 1.0));

}