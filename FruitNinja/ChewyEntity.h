#include "GameEntity.h"

class ChewyEntity : public GameEntity
{
public:
	ChewyEntity(MeshSet& mesh, Material material);
	void update();
};