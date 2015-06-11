#version 330

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aVelocity; 
layout (location = 2) in int aFrame;

uniform int uColumns;
uniform int uRows;
uniform float uTime;
uniform float uTimePerFrame;
uniform vec3 uAcceleration;

out vec2 cellPos;

void main()
{
	int frame = int(aFrame + uTime / uTimePerFrame) % (uColumns * uRows);
	cellPos = vec2(frame % uColumns / float(uColumns), frame / uColumns / float(uRows));

	vec3 pos = aPosition + aVelocity * uTime + 0.5 * uAcceleration * uTime * uTime;
	gl_Position = vec4(pos, 1.0);
}
