#version 150

out vec4 fragColor;
uniform vec3 Ucolor;

void main()
{
	fragColor = vec4(Ucolor, 1.0);
}