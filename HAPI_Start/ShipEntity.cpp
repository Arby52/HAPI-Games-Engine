#include "pch.h"
#include "ShipEntity.h"


ShipEntity::ShipEntity(std::string _associatedSprite, Visualisation &v, int _numFrames) : Entity(_associatedSprite, v, _numFrames)
{
}


ShipEntity::~ShipEntity()
{
}
