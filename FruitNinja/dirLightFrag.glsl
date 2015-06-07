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

#define AMBIENT_INTENSITY 0.2
#define DIFFUSE_INTENSITY 0.4
#define SPEC_INTENSITY 0.8
#define SHINY 20

/*vec2 poissonDisk[4] = vec2[](
  vec2( -0.94201624, -0.39906216 ),
  vec2( 0.94558609, -0.76890725 ),
  vec2( -0.094184101, -0.92938870 ),
  vec2( 0.34495938, 0.29387760 )
);*/

/*vec2 poissonDisk[12] = vec2[](
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
);*/

/*vec2 poissonDisk[16] = vec2[](
	vec2(0.6516818f, 0.6328417f),
	vec2(0.1121163f, 0.1513577f),
	vec2(0.213241f, 0.5424011f),
	vec2(0.7370221f, 0.006750573f),
	vec2(0.002864853f, 0.9401486f),
	vec2(-0.5591842f, 0.5755382f),
	vec2(-0.2809204f, 0.2612968f),
	vec2(-0.9051749f, 0.3720252f),
	vec2(-0.3889992f, -0.1379283f),
	vec2(0.6545967f, -0.3989382f),
	vec2(-0.09291586f, -0.5723442f),
	vec2(0.2760926f, -0.9004849f),
	vec2(-0.9116462f, -0.1122659f),
	vec2(-0.7701244f, -0.6338872f),
	vec2(-0.2006904f, -0.9768721f),
	vec2(0.2112095f, -0.2383357f)
);*/

float random(vec3 pos, int i)
{
	vec4 seed4 = vec4(pos, float(i));
	float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
    return fract(sin(dot_product) * 43758.5453);
}

float calcShadowFactor(vec3 pos, float cosTheta)
{
	cosTheta = clamp(cosTheta, 0.0, 1.0);
	vec4 lightPos = uShadowVP * vec4(pos, 1.0);
	vec3 projCoords = lightPos.xyz / lightPos.w;
	vec2 uvCoords;
	uvCoords.x = 0.5 * projCoords.x + 0.5;
	uvCoords.y = 0.5 * projCoords.y + 0.5;
	float z = 0.5 * projCoords.z + 0.5;

	float depth = texture(shadowMap, uvCoords).x;
	float bias = 0.005 * tan(acos(cosTheta));
	if (depth < (z - bias)) {
		return 0.0;
	}
	else {
		return 1.0;
	}

	/*
	float visibility = 1.0;
	for (int i = 0; i < 4; i++) {
		//int index = int(16.0 * random(pos, i)) % 16;
		//float depth = texture(shadowMap, uvCoords + poissonDisk[index] / 2000.0).x;
		//if (depth < (z - bias)) {
		//	visibility -= (1.0 / 5.0);
		//}

		//int index = int(4.0 * random(pos, i)) % 4;
		//visibility -= 0.4 * (1.0 - texture(shadowMap, uvCoords + poissonDisk[index] / 700.0).x);
		//visibility = max(0, visibility);

		//int index = int(12.0 * random(pos, i)) % 12;
		//visibility -= (1.0 / 5.0) * (1.0 - texture(shadowMap, uvCoords + poissonDisk[index] / 700.0).x);
		//visibility = max(0, visibility);

		//float depth = texture(shadowMap, uvCoords + poissonDisk[i] / 1000.0).x;
		//if (depth < (z + 0.00001)) {
		//	visibility -= (1.0 / 5.0);
		//}
	}

	return visibility;*/
}

vec4 calcLightInternal(vec3 lightDir, vec3 worldPos, vec3 normal)
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

	float shadowFactor = calcShadowFactor(worldPos, diffuseFactor);

    return (ambientColor + shadowFactor * (diffuseColor + specularColor));
}

vec4 calcDirLight(vec3 worldPos, vec3 normal)
{
    vec3 lightDir = uPos;
    lightDir = normalize(lightDir);

    vec4 color = calcLightInternal(lightDir, worldPos, normal);

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