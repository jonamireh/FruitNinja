#include "DoorEntity.h"
#include "ChewyEntity.h"
#include "World.h"
#include "main.h"
#include "AudioManager.h"
using namespace std;

DoorEntity::DoorEntity()
{
}


DoorEntity::DoorEntity(glm::vec3 position, MeshSet* mesh, bool open) : GameEntity(position, mesh)
{
    this->open = open;
}

void DoorEntity::update()
{
	GameEntity::update();
}

void DoorEntity::collision(GameEntity* entity)
{
    ChewyEntity* chewy_check = dynamic_cast<ChewyEntity*>(entity);
    if (chewy_check != nullptr && open)
    {
		AudioManager::instance()->play2D(assetPath + "WW_LargeChest_Open1.wav", false, 0.3f);
        world->setup_next_courtyard();
    }
}