#version 330

#define HEIGHT 6.0
#define WIDTH 6.0

uniform sampler2D Utex;

in vec2 cellPos;

out vec4 finalColor;

void main() {
	vec2 texCoord = gl_PointCoord;
	
	texCoord.x = texCoord.x / WIDTH + cellPos.x;
	texCoord.y = texCoord.y / HEIGHT + cellPos.y;
	finalColor = texture(Utex, texCoord);
}
