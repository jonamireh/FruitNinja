#version 330

uniform sampler2D posMap;
uniform sampler2D norMap;
uniform sampler2D colMap;
uniform sampler2D shadowMap;

uniform mat4 uShadowVP;
uniform vec2 uSize;
uniform vec3 uEye;
uniform vec3 uPos;

out vec4 FragColor;

#define AMBIENT_INTENSITY 0.3
#define DIFFUSE_INTENSITY 0.5
#define SPEC_INTENSITY 1.0
#define SHINY 20


vec2 poissonDisk[12] = vec2[](
	vec2(-0.5560945f, -0.1039571f),
	vec2(-0.03921358f, 0.2479071f),
	vec2(0.3701732f, -0.06092868f),
	vec2(-0.7933782f, 0.5168048f),
	vec2(-0.2650119f, -0.7896724f),
	vec2(-0.07208196f, 0.7526851f),
	vec2(0.595312f, 0.4872388f),
	vec2(0.7884521f, -0.5025175f),
	vec2(0.2916443f, -0.6768091f),
	vec2(0.9375908f, 0.003824265f),
	vec2(-0.06717162f, -0.3103488f),
	vec2(-0.750717f, -0.5788005f)
);

float random(vec3 pos, int i)
{
	vec4 seed4 = vec4(pos, float(i));
	float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
    return fract(sin(dot_product) * 43758.5453);
}

float calcShadowFactor(vec3 pos)
{
	vec4 lightPos = uShadowVP * vec4(pos, 1.0);
	vec3 projCoords = lightPos.xyz / lightPos.w;
	vec2 uvCoords;
	uvCoords.x = 0.5 * projCoords.x + 0.5;
	uvCoords.y = 0.5 * projCoords.y + 0.5;
	float z = 0.5 * projCoords.z + 0.5;
	
	float visibility = 1.0;
	for (int i = 0; i < 12; i++) {
		int index = int(12.0 * random(pos, i)) % 12;
		visibility -= (1.0 / 5.0) * (1.0 - texture(shadowMap, uvCoords + poissonDisk[index] / 700.0).x);
		visibility = max(0, visibility);

		/*float depth = texture(shadowMap, uvCoords + poissonDisk[i] / 1000.0).x;
		if (depth < (z + 0.00001)) {
			visibility -= (1.0 / 13.0);
		}*/
	}

	return visibility;
}

vec4 calcLightInternal(vec3 lightDir, vec3 worldPos, vec3 normal, float shadowFactor)
{
	//white light
	vec3 color = vec3(1.0, 1.0, 1.0);
    vec4 ambientColor = vec4(color, 1.0) * AMBIENT_INTENSITY;
    float diffuseFactor = dot(normal, -lightDir);

    vec4 diffuseColor = vec4(0, 0, 0, 0);
    vec4 specularColor = vec4(0, 0, 0, 0);

    if (diffuseFactor > 0.0) {
        diffuseColor = vec4(color, 1.0) * DIFFUSE_INTENSITY * diffuseFactor;

        vec3 vertexToEye = normalize(uEye - worldPos);
        vec3 lightReflect = normalize(reflect(lightDir, normal));
        float specularFactor = dot(vertexToEye, lightReflect);
        specularFactor = pow(specularFactor, SHINY);
        if (specularFactor > 0.0) {
            specularColor = vec4(color, 1.0) * SPEC_INTENSITY * specularFactor;
        }
    }

    return (ambientColor + shadowFactor * (diffuseColor + specularColor));
}

vec4 calcDirLight(vec3 worldPos, vec3 normal)
{
    vec3 lightDir = uPos;
    lightDir = normalize(lightDir);
	float shadowFactor = calcShadowFactor(worldPos);

    vec4 color = calcLightInternal(lightDir, worldPos, normal, shadowFactor);

    return color;
}

vec2 calcTexCoord()
{
   return gl_FragCoord.xy / uSize;
}

void main()
{
   	vec2 texCoord = calcTexCoord();
   	vec3 worldPos = texture(posMap, texCoord).xyz;
   	vec3 color = texture(colMap, texCoord).xyz;
   	vec3 normal = texture(norMap, texCoord).xyz;
   	normal = normalize(normal);

   	FragColor = vec4(color, 1.0) * calcDirLight(worldPos, normal) * .6;
}