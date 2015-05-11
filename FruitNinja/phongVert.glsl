#version 330

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTextCoord;
layout(location = 3) in ivec4 boneId1;
layout(location = 4) in vec4 boneWeight1;
layout(location = 5) in ivec4 boneId2;
layout(location = 6) in vec4 boneWeight2;

const int MAX_BONES = 100;

uniform mat4 uBones[MAX_BONES];
uniform int uBoneFlag;

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
	if (uBoneFlag == 1)
	{
		boneTransform = uBones[boneId1[0]] * boneWeight1[0];
		boneTransform += uBones[boneId1[1]] * boneWeight1[1];
		boneTransform += uBones[boneId1[2]] * boneWeight1[2];
		boneTransform += uBones[boneId1[3]] * boneWeight1[3];
		boneTransform += uBones[boneId2[0]] * boneWeight2[0];
		boneTransform += uBones[boneId2[1]] * boneWeight2[1];
		boneTransform += uBones[boneId2[2]] * boneWeight2[2];
		boneTransform += uBones[boneId2[3]] * boneWeight2[3];
	}

	gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * boneTransform * vec4(aPosition, 1.0);
	vNormals = vec3(uModelMatrix * boneTransform * vec4(normalize(aNormal), 0.0));
	vLight = uLightPos - vec3(uModelMatrix * vec4(aPosition, 1.0));
	vTextCoord = aTextCoord;
}