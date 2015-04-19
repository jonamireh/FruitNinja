#include "GameEntity.h"

class ChewyEntity : public GameEntity
{
public:
	ChewyEntity();
	ChewyEntity(MeshSet& mesh, Material material);
	void update();
};