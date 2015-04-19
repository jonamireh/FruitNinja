uniform vec3 UaColor;
uniform vec3 UdColor;
uniform vec3 UsColor;
uniform float Ushine;

varying vec3 vNormals;
varying vec3 vLight;

void main()
{
	vec3 diffuse = UdColor * max(0.0, dot(normalize(vNormals), normalize(vLight)));
	vec3 specular = UsColor * max(0.0, pow(dot(normalize(vNormals), normalize(vLight)), Ushine));
	gl_FragColor = vec4(diffuse + specular + UaColor, 1);
}
