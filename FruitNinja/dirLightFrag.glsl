#version 330

uniform sampler2D posMap;
uniform sampler2D norMap;
uniform sampler2D colMap;

uniform vec2 uSize;
uniform vec3 uEye;
uniform vec3 uLightPos;

out vec4 FragColor;

void main()
{
    vec2 texCoord = gl_FragCoord.xy / uSize;
	vec3 worldPos = texture(posMap, texCoord).xyz;
	vec3 color = texture(colMap, texCoord).xyz;
	vec3 normal = texture(norMap, texCoord).xyz;
	normal = normalize(normal);
 
	vec3 lightDir = normalize(uLightPos - worldPos);
	
	vec3 viewDir = normalize(uEye - worldPos);
	vec3 vHalfVector = normalize(lightDir + viewDir);
	
	FragColor = max(dot(normal, lightDir), 0) * vec4(color, 1.0) + pow(max(dot(normal, vHalfVector), 0.0), 100) * 1.5;
}