#version 330

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTextCoord;
layout(location = 3) in ivec4 boneIds;
layout(location = 4) in vec4 boneWeights;

const int MAX_BONES = 100;

uniform mat4 uBones[MAX_BONES];
//uniform int uBoneFlag;

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
out vec2 vTextCoord;

void main()
{
	mat4 boneTransform = mat4(1, 0, 0, 0,
							  0, 1, 0, 0,
							  0, 0, 1, 0,
							  0, 0, 0, 1);

	//if (uBoneFlag == 1)
	//{
		//boneTransform = uBones[boneIds[0]] * boneWeights[0];
		//boneTransform += uBones[boneIds[1]] * boneWeights[1];
		//boneTransform += uBones[boneIds[2]] * boneWeights[2];
		//boneTransform += uBones[boneIds[3]] * boneWeights[3];
	//}

	gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * boneTransform * vec4(aPosition, 1.0);
	vNormals = vec3(uModelMatrix * boneTransform * vec4(normalize(aNormal), 0.0));
	vLight = uLightPos - vec3(uModelMatrix * vec4(aPosition, 1.0));
	vTextCoord = aTextCoord;
}