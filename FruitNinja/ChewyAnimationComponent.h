#pragma once
#include "Component.h"
#include "GameEntity.h"

class ChewyAnimationComponent :
	public Component
{
public:
	void update() override;
	ChewyAnimationComponent(GameEntity *chewy);
	~ChewyAnimationComponent();
private:
	void ChewyAnimationComponent::calculateAnimationTransforms(aiNode *node, aiMatrix4x4 parentTransform);
	void CalcInterpolatedRotation(aiQuaternion& Out, const aiNodeAnim* pNodeAnim);
	glm::uint FindRotation(const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedScaling(aiVector3D& Out, const aiNodeAnim* pNodeAnim);
	glm::uint FindScaling(const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedTranslation(aiVector3D& Out, const aiNodeAnim* pNodeAnim);
	glm::uint FindTranslation(const aiNodeAnim* pNodeAnim);
	glm::mat4 ChewyAnimationComponent::convertAiMatrix4x4ToMat4(aiMatrix4x4 inMat);
	GameEntity *entity;
	float frameTime;
};

