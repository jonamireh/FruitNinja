#include "Material.h"

using namespace glm;

Material::Material()
{
	diffuse = vec3(0);
	specular = vec3(0);
	ambient = vec3(0);
	shininess = 0;
}

Material::Material(vec3 ambientColor, vec3 diffuseColor, vec3 specularColor, float shininessValue)
{
	diffuse = diffuseColor;
	specular = specularColor;
	ambient = ambientColor;
	shininess = shininessValue;
}
