#include "GuardPuppeteer.h"

GuardPuppeteer::GuardPuppeteer(MeshSet* meshset) {
	_meshset = meshset;
	_walker = new GuardEntity(glm::vec3(), meshset, nullptr, glm::vec3());
	_walker->animComponent.setCurrentAnimation(WALKIN);
	_idle = new GuardEntity(glm::vec3(), meshset, nullptr, glm::vec3());
}

void GuardPuppeteer::update() {
	_walker->animComponent.update();
	_idle->animComponent.update();
}

std::vector<std::vector<glm::mat4>>* GuardPuppeteer::getWalkerBoneTransform() {
	return _walker->animComponent.basicAnimation.getBoneTransformations();
}

std::vector<std::vector<glm::mat4>>* GuardPuppeteer::getIdleBoneTransform() {
	return _idle->animComponent.basicAnimation.getBoneTransformations();
}