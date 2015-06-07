#version 330

//in vec3 aPosition;
//in vec2 aTextCoord;

layout (location = 0) in vec3 aPosition; 
//layout (location = 1) in vec3 aNormal;
//layout (location = 2) in vec2 aTextCoord; 
layout (location = 3) in ivec4 boneId;
layout (location = 4) in vec4 boneWeight;

const int MAX_BONES = 100;

uniform mat4 uBones[MAX_BONES];
uniform int uBoneFlag;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

//out vec2 vTextCoord;
//out vec3 vWorldPos;
//out vec3 vNormal;

void main()
{
	mat4 boneTransform = mat4(1, 0, 0, 0,
							  0, 1, 0, 0,
							  0, 0, 1, 0,
							  0, 0, 0, 1);
	if (uBoneFlag == 1)
	{
		boneTransform = boneWeight[0] * uBones[boneId[0]];
		boneTransform += boneWeight[1] * uBones[boneId[1]];
		boneTransform += boneWeight[2] * uBones[boneId[2]];
		boneTransform += boneWeight[3] * uBones[boneId[3]];
	}

	//vWorldPos = vec3(uModelMatrix * boneTransform * vec4(aPosition, 1.0));
	//vNormal = vec3(uModelMatrix * boneTransform * vec4(normalize(aNormal), 0.0));
	gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * boneTransform * vec4(aPosition, 1.0);
	//vTextCoord = aTextCoord;
}
