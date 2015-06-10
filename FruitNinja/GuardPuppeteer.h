#pragma once
#include "GuardEntity.h"

class GuardPuppeteer {
public:
	GuardPuppeteer(MeshSet* meshset);
	std::vector<std::vector<glm::mat4>>* getWalkerBoneTransform();
	std::vector<std::vector<glm::mat4>>* getIdleBoneTransform();
	void update();
private:
	MeshSet* _meshset;
	GuardEntity* _walker;
	GuardEntity* _idle;
};