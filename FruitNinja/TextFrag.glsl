#version 330

uniform sampler2D uTexture;
uniform vec3 uColor;
uniform float percent;

in vec2 UV;

out vec4 finalColor;

void main(){
	if (UV.y > percent)
		discard;
	vec4 texColor = texture(uTexture, vec2(UV.x, 1 - UV.y));
	finalColor = vec4(uColor, texColor.x);
}