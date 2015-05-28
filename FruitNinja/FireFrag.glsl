#version 330

uniform sampler2D Utex;

in vec2 texCoord;

out vec4 finalColor;

void main() {
	finalColor = texture(Utex, texCoord);
}
