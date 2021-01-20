#include "pch.h"
#include "Entity.h"
#include "Visualisation.h"


Entity::Entity(std::string _associatedSprite, Visualisation &v, int numFrames) : m_spriteName(_associatedSprite), m_numFrames(numFrames)
{	
}

Entity::~Entity()
{
}

bool Entity::CanCollideWith(ESide a, ESide b) {
	if (a != b && a != ESide::eNeutral && b != ESide::eNeutral)
		return true;
	else
		return false;
}

