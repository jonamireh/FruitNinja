#pragma once
#include "Component.h"
#include "GameEntity.h"

class BasicAnimationComponent :
	public Component
{
public:
	void update() override;
	BasicAnimationComponent(GameEntity *chewy);
	~BasicAnimationComponent();
private:
	void BasicAnimationComponent::calculateAnimationTransforms(aiNode *node, aiMatrix4x4 parentTransform);
	aiNodeAnim* FindAnimationNode(std::string boneName);
	void CalcInterpolatedRotation(aiQuaternion& Out, const aiNodeAnim* pNodeAnim);
	glm::uint FindRotation(const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedScaling(aiVector3D& Out, const aiNodeAnim* pNodeAnim);
	glm::uint FindScaling(const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedTranslation(aiVector3D& Out, const aiNodeAnim* pNodeAnim);
	glm::uint FindTranslation(const aiNodeAnim* pNodeAnim);
	glm::mat4 BasicAnimationComponent::convertAiMatrix4x4ToMat4(aiMatrix4x4 inMat);
	GameEntity *entity;
	float frameTime;
};

