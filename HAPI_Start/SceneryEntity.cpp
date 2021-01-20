#include "pch.h"
#include "SceneryEntity.h"


SceneryEntity::SceneryEntity(std::string _associatedSprite, Visualisation &v) : Entity(_associatedSprite, v)
{
	m_side = ESide::eNeutral;
}

SceneryEntity::~SceneryEntity()
{
}

void SceneryEntity::DetectCollision(Visualisation & v, Entity & other)
{
	//No use at the moment.
}

void SceneryEntity::HasCollided(Visualisation & v, Entity & other)
{
	//No use at the moment.
}
