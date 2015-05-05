#include "ChewyAnimationComponent.h"
#include "World.h"
#include "ChewyEntity.h"


void ChewyAnimationComponent::update()
{
	ChewyEntity *chewy = static_cast<ChewyEntity *>(entity);
	std::vector<Mesh*> chewyMeshes = chewy->mesh->getMeshes();
	frameTime += seconds_passed;
	if (frameTime > chewy->current_animation->mDuration)
	{
		frameTime -= chewy->current_animation->mDuration;
	}

	// Calculate bones for each mesh
	for (int i = 0; i < chewyMeshes.size(); i++)
	{
		glm::mat4 identity = glm::mat4();
		// Calculate transformations for each bone
		for (int j = 0; j < chewyMeshes[i]->bones.size(); j++)
		{
			// Gets the animation info for the bone
			aiNodeAnim *bone = getBone(chewy->current_animation, chewyMeshes[i]->bones[j].mName.C_Str());

			// Makes sure that the bone was found
			if (bone)
			{
				
			}
		}
	}
}

aiNodeAnim* ChewyAnimationComponent::getBone(aiAnimation *animation, std::string boneName)
{
	for (int i = 0; i < animation->mNumChannels; i++)
	{
		if (animation->mChannels[i]->mNodeName.C_Str() == boneName)
		{
			return animation->mChannels[i];
		}
	}
	return nullptr;
}

ChewyAnimationComponent::ChewyAnimationComponent(GameEntity *chewy)
{
	this->entity = chewy;
}


ChewyAnimationComponent::~ChewyAnimationComponent()
{
}
