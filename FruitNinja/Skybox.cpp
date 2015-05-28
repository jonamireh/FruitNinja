#include "Skybox.h"
#include "Camera.h"


void Skybox::update()
{
    setPosition((*_camera)->cameraPosition + glm::vec3(0.f, bounding_box.half_height, 0.f));
}

Skybox::Skybox(std::shared_ptr<Camera>* camera, std::shared_ptr<MeshSet> mesh) : GameEntity(glm::vec3(0), mesh)
{
	_camera = camera;
}

Skybox::~Skybox()
{
}
