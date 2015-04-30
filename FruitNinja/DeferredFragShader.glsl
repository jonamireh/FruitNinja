#version 150

uniform sampler2D Utex;
uniform int Uflag;

in vec2 vTextCoord;

out vec4 finalColor;

void main()
{
	if(Uflag == 1) {
		finalColor = texture(Utex, vTextCoord);
	} else {
		finalColor = vec4(0, .5, 6, 1);
	}
}
