#include "Skybox.h"
#include "Camera.h"


void Skybox::update()
{
	position = (*_camera)->cameraPosition;
}

Skybox::Skybox(std::shared_ptr<Camera>* camera, std::shared_ptr<MeshSet> mesh) : GameEntity(glm::vec3(0), mesh)
{
	_camera = camera;
}

Skybox::~Skybox()
{
}
