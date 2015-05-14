#version 330

in vec3 vWorldPos;

uniform vec3 uLightWorldPos;

out float fragColor;

void main()
{
	vec3 lightToVertex = vWorldPos - uLightWorldPos;
	float lightToPixelDistance = length(lightToVertex);
	fragColor = lightToPixelDistance;
}
