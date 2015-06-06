#version 330

uniform sampler2D uTexture;

in vec2 UV;

out vec4 finalColor;

void main(){
	float z = texture(uTexture, UV).r;
	float n = 1.0;
	float f = 175.0;
	float c = (2.0 * n) / (f + n - z * (f - n));
	finalColor.rgb = vec3(c); //vec3(z);
}