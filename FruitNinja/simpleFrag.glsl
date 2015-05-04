#version 330

uniform sampler2D Utex;

in vec3 vNormals;
in vec2 vTextCoord;

out vec4 finalColor;

void main()
{
	finalColor = texture(Utex, vTextCoord);
}
