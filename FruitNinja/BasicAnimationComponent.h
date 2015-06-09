#pragma once
#include "Component.h"
#include "GameEntity.h"
#define FRAMES_PER_SEC 24.0

class BasicAnimationComponent :
	public Component
{
public:
	void update() override;
	BasicAnimationComponent(GameEntity *chewy);
	~BasicAnimationComponent();
	void changeToLoopingAnimation(float start, float end);
	void changeToSingleAnimation(float start, float end);
	void changeToSingleInterruptibleAnimation(float start, float end);
	float frameTime;
	bool animationComplete;
	std::vector<std::vector<glm::mat4>>* getBoneTransformations();
private:
	void calculateAnimationTransforms(aiNode *node, aiMatrix4x4 parentTransform);
	BoneInfo* FindBoneInfo(std::string boneName);
	aiNodeAnim* GetAnimationNode(BoneInfo* boneInfo);
	void CalcInterpolatedRotation(aiQuaternion& Out, const aiNodeAnim* pNodeAnim);
	glm::uint FindRotation(const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedScaling(aiVector3D& Out, const aiNodeAnim* pNodeAnim);
	glm::uint FindScaling(const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedTranslation(aiVector3D& Out, const aiNodeAnim* pNodeAnim);
	glm::uint FindTranslation(const aiNodeAnim* pNodeAnim);
	glm::mat4 convertAiMatrix4x4ToMat4(aiMatrix4x4 inMat);
	GameEntity *entity;
	float starting_frame_time;
	float end_frame_time;
	bool looping;
	std::vector<std::vector<glm::mat4>> boneTransformations;
};

