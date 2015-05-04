#version 330

uniform vec3 UdirLight;
uniform vec3 UaColor;
uniform vec3 UdColor;
uniform vec3 UsColor;
uniform float Ushine;
uniform sampler2D Utex;
uniform int Uflag;

in vec3 vNormals;
in vec3 vLight;
in vec2 vTextCoord;

out vec4 finalColor;

void main()
{
	vec3 dirDiffuse = UdColor * max(0.0, dot(normalize(vNormals), normalize(UdirLight)));
	vec3 dirSpecular = UsColor * max(0.0, pow(dot(normalize(vNormals), normalize(UdirLight)), Ushine));
	vec3 specular = UsColor * max(0.0, pow(dot(normalize(vNormals), normalize(vLight)), Ushine));
	if(Uflag == 1) {
		vec4 texColor = texture(Utex, vTextCoord);
		vec3 diffuse = vec3(texColor) * max(0.0, dot(normalize(vNormals), normalize(vLight)));
		finalColor = vec4(diffuse + specular + UaColor, 1);
	} else {
		vec3 diffuse = UdColor * max(0.0, dot(normalize(vNormals), normalize(vLight)));
		finalColor = vec4(dirDiffuse + dirSpecular + diffuse + specular + UaColor, 1);
	}
}
