#include "TestSphere.h"


void TestSphere::update()
{
	animComponent.update();
}

TestSphere::TestSphere(MeshSet* mesh) : GameEntity(glm::vec3(5, 5, 5), mesh), animComponent(this)
{
	current_animation = mesh->getAnimations()[0];
}


TestSphere::~TestSphere()
{
}
