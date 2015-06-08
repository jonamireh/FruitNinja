#include "BasicAnimationComponent.h"
#include "World.h"
#include "ChewyEntity.h"


void BasicAnimationComponent::update()
{
	if (entity->current_animation == nullptr)
		return;

	std::vector<Mesh*> entityMeshses = entity->mesh->getMeshes();
	frameTime += seconds_passed;
	if (looping && frameTime > end_frame_time)
	{
		float duration = end_frame_time - starting_frame_time;
		assert(duration > 0);
		while (frameTime - duration > starting_frame_time)
		{
			frameTime -= duration;
		}
	}
	else if (!looping)
	{
		frameTime = min(frameTime, end_frame_time);
		animationComplete = animationComplete || end_frame_time - frameTime < .0001;
	}

	frameTime = max(starting_frame_time, frameTime);

	aiMatrix4x4 identity = aiMatrix4x4();
	calculateAnimationTransforms(entity->mesh->bone_tree, identity);
	// Get bones for each mesh
	for (int i = 0; i < entityMeshses.size(); i++)
	{
		entity->boneTransformations[i].clear();
		// Get transformations for each bone
		for (int j = 0; j < entityMeshses[i]->bones.size(); j++)
		{
			string boneName = entityMeshses[i]->bones[j].mName.C_Str();
			entity->boneTransformations[i].push_back(convertAiMatrix4x4ToMat4(
				entity->mesh->boneInfo.at(boneName)->transformation * entityMeshses[i]->bones[j].mOffsetMatrix));
			/*entityMeshses[i]->boneTransformations.push_back(convertAiMatrix4x4ToMat4(
				entity->mesh->boneInfo.at(boneName)->transformation * entityMeshses[i]->bones[j].mOffsetMatrix));*///boom
		}
	}
}

void BasicAnimationComponent::calculateAnimationTransforms(aiNode *node, aiMatrix4x4 parentTransform)
{
	string boneName = node->mName.C_Str();

	aiMatrix4x4 nodeTransform(node->mTransformation);

	// Gets the animation info for the bone
	aiNodeAnim *bone = FindAnimationNode(boneName);

	if (bone != nullptr)
	{
		// Interpolate scaling and generate scaling transformation matrix
		aiVector3D Scaling(1, 1, 1);
		CalcInterpolatedScaling(Scaling, bone);
		aiMatrix4x4 ScalingM;
		aiMatrix4x4::Scaling(Scaling, ScalingM);

		// Interpolate rotation and generate rotation transformation matrix
		aiQuaternion RotationQ(1, 0, 0, 0);
		CalcInterpolatedRotation(RotationQ, bone);
		aiMatrix4x4 RotationM = aiMatrix4x4(RotationQ.GetMatrix());

		// Interpolate translation and generate translation transformation matrix
		aiVector3D Translation(0, 0, 0);
		CalcInterpolatedTranslation(Translation, bone);
		aiMatrix4x4 TranslationM;
		aiMatrix4x4::Translation(Translation, TranslationM);

		// Combine the above transformations
		nodeTransform = TranslationM * RotationM * ScalingM;
	}

	aiMatrix4x4 globalTransformation = parentTransform * nodeTransform;

	if (bone != nullptr)
		entity->mesh->boneInfo.at(boneName)->transformation = entity->mesh->inverseMat * globalTransformation;

	for (int i = 0; i < node->mNumChildren; i++)
	{
		calculateAnimationTransforms(node->mChildren[i], globalTransformation);
	}
}

aiNodeAnim* BasicAnimationComponent::FindAnimationNode(string boneName)
{
	if (entity->mesh->boneInfo.find(boneName) == entity->mesh->boneInfo.end())
		return nullptr;
	if (entity->mesh->boneInfo.at(boneName)->bone_anim->size() <= 0)
		return nullptr;
	return entity->mesh->boneInfo.at(boneName)->bone_anim->at(entity->current_animation);
}

void BasicAnimationComponent::CalcInterpolatedRotation(aiQuaternion& Out, const aiNodeAnim* pNodeAnim)
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

glm::uint BasicAnimationComponent::FindRotation(const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumRotationKeys > 0);

	for (glm::uint i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
		if (frameTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
			return i;
		}
	}
	assert(0);
}

void BasicAnimationComponent::CalcInterpolatedScaling(aiVector3D& Out, const aiNodeAnim* pNodeAnim)
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
	//Out = Out.Normalize();
}

glm::uint BasicAnimationComponent::FindScaling(const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumScalingKeys > 0);

	for (glm::uint i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
		if (frameTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
			return i;
		}
	}
	assert(0);
}

void BasicAnimationComponent::CalcInterpolatedTranslation(aiVector3D& Out, const aiNodeAnim* pNodeAnim)
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
	//Out = Out.Normalize();
}

glm::uint BasicAnimationComponent::FindTranslation(const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumPositionKeys > 0);

	for (glm::uint i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
		if (frameTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
			return i;
		}
	}
	assert(0);
}

glm::mat4 BasicAnimationComponent::convertAiMatrix4x4ToMat4(aiMatrix4x4 inMat)
{
	return glm::mat4(inMat.a1, inMat.b1, inMat.c1, inMat.d1,
		inMat.a2, inMat.b2, inMat.c2, inMat.d2,
		inMat.a3, inMat.b3, inMat.c3, inMat.d3,
		inMat.a4, inMat.b4, inMat.c4, inMat.d4);
}

void BasicAnimationComponent::changeToLoopingAnimation(float start, float end)
{
	frameTime = start;
	starting_frame_time = start;
	end_frame_time = end;
	looping = true;
	animationComplete = true;
}

void BasicAnimationComponent::changeToSingleAnimation(float start, float end)
{
	frameTime = start;
	starting_frame_time = start;
	end_frame_time = end;
	looping = false;
	animationComplete = false;
}

void BasicAnimationComponent::changeToSingleInterruptibleAnimation(float start, float end)
{
	frameTime = start;
	starting_frame_time = start;
	end_frame_time = end;
	looping = false;
	animationComplete = true;
}

BasicAnimationComponent::BasicAnimationComponent(GameEntity *entity)
{
	this->entity = entity;
	starting_frame_time = 1.0 / FRAMES_PER_SEC;
	MeshSet* mesh = entity->mesh;
	end_frame_time = mesh->getAnimations().at(0)->mDuration;
	looping = true;
	std::vector<Mesh*> meshes = mesh->getMeshes();
	for (size_t i = 0; i < meshes.size(); i++) {
		boneTransformations.push_back(meshes[i]->getBoneTransformations());
	}
}


BasicAnimationComponent::~BasicAnimationComponent()
{
}

std::vector<std::vector<glm::mat4>>* BasicAnimationComponent::getBoneTransformations() {
	return &boneTransformations;
}
