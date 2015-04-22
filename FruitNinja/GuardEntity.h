#include "GameEntity.h"
#include "ChewyMovementComponent.h"

class GuardEntity : public GameEntity
{
public:
	GuardEntity();
	GuardEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh);
	void update();
private:
};