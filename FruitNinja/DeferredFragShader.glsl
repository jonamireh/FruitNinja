#version 330

uniform sampler2D Utex;
uniform int Uflag;
uniform vec3 UdColor;


in vec2 vTextCoord;
in vec3 vWorldPos;
in vec3 vNormal;

layout (location = 0) out vec3 WorldPosOut; 
layout (location = 1) out vec3 DiffuseOut; 
layout (location = 2) out vec3 NormalOut; 
layout (location = 3) out vec3 TexCoordOut; 

void main()
{
	WorldPosOut = vWorldPos;
	if(Uflag == 1) {
		DiffuseOut = texture(Utex, vTextCoord).xyz;
	} else {
		DiffuseOut = UdColor;
	}

	NormalOut = vNormal; 
    TexCoordOut = vec3(vTextCoord, 0.0); 
}
