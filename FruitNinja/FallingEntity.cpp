#include "FallingEntity.h"
#include "SpikeEntity.h"
#include "World.h"
#include <math.h>
#include <glm/gtx/string_cast.hpp>


FallingEntity::FallingEntity(glm::vec3 position, MeshSet* mesh) :
GameEntity(position, mesh)
{
    velocity = glm::vec3(0.f);
    stepped_on = false;
}

void FallingEntity::update()
{
    static float prevRotationX = 0;
    static float rotationX = 0; 
    static float prevRotationZ = 0;
    static float rotationZ = 0;
    static float rotationTime = 0;
    if (stepped_on)
    {
        setPosition(getPosition() + velocity * seconds_passed);
        elapsed_time += seconds_passed;
        rotationTime += seconds_passed;
        
        if (elapsed_time > TIME_TO_FALL)
        {
            setRotations(glm::vec3(0.f));
            velocity.y -= GRAVITY * 0.5f * seconds_passed;
			collision_response = true;
        }
        else
        {
            float newRotationX = prevRotationX + (prevRotationX - rotationX) * (rotationTime / .1f);
            float newRotationZ = prevRotationZ + (prevRotationZ - rotationZ) * (rotationTime / .1f);
            setRotations(glm::vec3(newRotationX, 0.f, newRotationZ));
        }
        if (rotationTime > .1)
        {
            rotationTime = 0;
            prevRotationX = rotationX;
            rotationX = glm::radians( (float) rand() / (float) RAND_MAX * 4.0f - 2.0f);
            prevRotationZ = rotationZ;
            rotationZ = glm::radians((float)rand() / (float)RAND_MAX * 4.0f - 2.0f);
        }
    }
}

void FallingEntity::collision(GameEntity* entity)
{
    if (typeid(ChewyEntity) == typeid(*entity))
        stepped_on = true;

    if (typeid(SpikeEntity) == typeid(*entity))
        list = UNSET_DRAW(list);

}