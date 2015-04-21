#include "GameEntity.h"

class ChewyEntity : public GameEntity
{
public:
	ChewyEntity();
	ChewyEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh, Material material);
	void update();
};