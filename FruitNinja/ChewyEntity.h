#include "GameEntity.h"

class ChewyEntity : public GameEntity
{
public:
	ChewyEntity();
    ChewyEntity(std::shared_ptr<MeshSet> mesh, Material material);
	void update();
};