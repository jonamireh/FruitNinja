#version 330

uniform sampler2D posMap;
uniform sampler2D norMap;
uniform sampler2D colMap;

uniform vec2 uSize;
uniform vec3 uEye;

out vec4 FragColor;

#define AMBIENT_INTENSITY 0.3
#define DIFFUSE_INTENSITY 0.5
#define SPEC_INTENSITY 1.0
#define SHINY 20

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

    return (ambientColor + diffuseColor + specularColor);
}

vec4 calcDirLight(vec3 worldPos, vec3 normal)
{
    vec3 lightDir = vec3(0.0, -1.0, 0.2);
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