#include "ChewyAnimationComponent.h"
#include "World.h"
#include "ChewyEntity.h"


void ChewyAnimationComponent::update()
{
	//ChewyEntity *chewy = static_cast<ChewyEntity *>(entity);
	//std::vector<Mesh*> chewyMeshes = chewy->mesh->getMeshes();
	//frameTime += seconds_passed;
	//if (frameTime > chewy->current_animation->mDuration)
	//{
	//	while (frameTime - chewy->current_animation->mDuration > 0)
	//		frameTime -= chewy->current_animation->mDuration;
	//}

	//frameTime = max(1 / 24.0f, frameTime);

	//aiMatrix4x4 identity = aiMatrix4x4();
	//calculateAnimationTransforms(chewy->mesh->bone_tree->mChildren[0], identity);
	//
	//// Get bones for each mesh
	//for (int i = 0; i < chewyMeshes.size(); i++)
	//{
	//	// Get transformations for each bone
	//	for (int j = 0; j < chewyMeshes[i]->bones.size(); j++)
	//	{
	//		string boneName = chewyMeshes[i]->bones[j].mName.C_Str();
	//		chewyMeshes[i]->boneTransformations[j] = convertAiMatrix4x4ToMat4(chewy->mesh->boneInfo.at(boneName)->transformation) * convertAiMatrix4x4ToMat4(chewyMeshes[i]->bones[j].mOffsetMatrix);
	//	}
	//}
}

void ChewyAnimationComponent::calculateAnimationTransforms(aiNode *node, aiMatrix4x4 parentTransform)
{
	ChewyEntity *chewy = static_cast<ChewyEntity *>(entity);
	string boneName = node->mName.C_Str();

	aiMatrix4x4 nodeTransform(node->mTransformation);

	// Gets the animation info for the bone
	BoneInfo *info = chewy->mesh->boneInfo.at(boneName);
	aiNodeAnim *bone = info->bone_anim->at(chewy->current_animation);

	if (bone)
	{
		// Interpolate scaling and generate scaling transformation matrix
		aiVector3D Scaling;
		CalcInterpolatedScaling(Scaling, bone);
		aiMatrix4x4 ScalingM;
		aiMatrix4x4::Scaling(Scaling, ScalingM);

		// Interpolate rotation and generate rotation transformation matrix
		aiQuaternion RotationQ;
		CalcInterpolatedRotation(RotationQ, bone);
		aiMatrix4x4 RotationM = aiMatrix4x4(RotationQ.GetMatrix());

		// Interpolate translation and generate translation transformation matrix
		aiVector3D Translation;
		CalcInterpolatedTranslation(Translation, bone);
		aiMatrix4x4 TranslationM;
		aiMatrix4x4::Translation(Translation, TranslationM);

		// Combine the above transformations
		nodeTransform = TranslationM * RotationM * ScalingM;
	}

	aiMatrix4x4 globalTransformation = parentTransform * nodeTransform;

	chewy->mesh->boneInfo.at(boneName)->transformation = chewy->mesh->inverseMat * globalTransformation;

	for (int i = 0; i < node->mNumChildren; i++)
	{
		calculateAnimationTransforms(node->mChildren[i], globalTransformation);
	}
}

void ChewyAnimationComponent::CalcInterpolatedRotation(aiQuaternion& Out, const aiNodeAnim* pNodeAnim)
{
	// we need at least two values to interpolate...
	if (pNodeAnim->mNumRotationKeys == 1) {
		Out = pNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	glm::uint RotationIndex = FindRotation(pNodeAnim);
	glm::uint NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
	float DeltaTime = pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime;
	float Factor = (frameTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
	aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
	Out = Out.Normalize();
}

glm::uint ChewyAnimationComponent::FindRotation(const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumRotationKeys > 0);

	for (glm::uint i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
		if (frameTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
			return i;
		}
	}
	assert(0);
}

void ChewyAnimationComponent::CalcInterpolatedScaling(aiVector3D& Out, const aiNodeAnim* pNodeAnim)
{
	// we need at least two values to interpolate...
	if (pNodeAnim->mNumScalingKeys == 1) {
		Out = pNodeAnim->mScalingKeys[0].mValue;
		return;
	}

	glm::uint ScalingIndex = FindScaling(pNodeAnim);
	glm::uint NextScalingIndex = (ScalingIndex + 1);
	assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
	float DeltaTime = pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime;
	float Factor = (frameTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& StartScalingV = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	const aiVector3D& EndScalingV = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
	Assimp::Interpolator<aiVector3D> interpolator;
	interpolator.operator()(Out, StartScalingV, EndScalingV, Factor);
	Out = Out.Normalize();
}

glm::uint ChewyAnimationComponent::FindScaling(const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumRotationKeys > 0);

	for (glm::uint i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
		if (frameTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
			return i;
		}
	}
	assert(0);
}

void ChewyAnimationComponent::CalcInterpolatedTranslation(aiVector3D& Out, const aiNodeAnim* pNodeAnim)
{
	// we need at least two values to interpolate...
	if (pNodeAnim->mNumPositionKeys == 1) {
		Out = pNodeAnim->mPositionKeys[0].mValue;
		return;
	}

	glm::uint TranslationIndex = FindTranslation(pNodeAnim);
	glm::uint NextTranslationIndex = (TranslationIndex + 1);
	assert(NextTranslationIndex < pNodeAnim->mNumPositionKeys);
	float DeltaTime = pNodeAnim->mPositionKeys[NextTranslationIndex].mTime - pNodeAnim->mPositionKeys[TranslationIndex].mTime;
	float Factor = (frameTime - (float)pNodeAnim->mPositionKeys[TranslationIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& StartTranslationV = pNodeAnim->mPositionKeys[TranslationIndex].mValue;
	const aiVector3D& EndTranslationV = pNodeAnim->mPositionKeys[NextTranslationIndex].mValue;
	Assimp::Interpolator<aiVector3D> interpolator;
	interpolator.operator()(Out, StartTranslationV, EndTranslationV, Factor);
	Out = Out.Normalize();
}

glm::uint ChewyAnimationComponent::FindTranslation(const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumPositionKeys > 0);

	for (glm::uint i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
		if (frameTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
			return i;
		}
	}
	assert(0);
}

glm::mat4 ChewyAnimationComponent::convertAiMatrix4x4ToMat4(aiMatrix4x4 inMat)
{
	return glm::mat4(inMat.a1, inMat.b1, inMat.c1, inMat.d1,
		inMat.a2, inMat.b2, inMat.c2, inMat.d2,
		inMat.a3, inMat.b3, inMat.c3, inMat.d3,
		inMat.a4, inMat.b4, inMat.c4, inMat.d4);
}

ChewyAnimationComponent::ChewyAnimationComponent(GameEntity *chewy)
{
	this->entity = chewy;
}


ChewyAnimationComponent::~ChewyAnimationComponent()
{
}
