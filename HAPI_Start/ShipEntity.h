#pragma once
#include "Entity.h"
#include "ProjectileEntity.h"
class ShipEntity :	public Entity
{

protected:
	std::vector<ProjectileEntity*> m_projectileVector;
	int m_health{ 30 };
	DWORD m_fireRate{ 420 };
	DWORD m_lastTimeUpdatedFireRate{ 0 };
public:
	ShipEntity(std::string _associatedSprite, Visualisation &v, int _numFrames = 1);
	~ShipEntity();

	virtual void DetectCollision(Visualisation &v, Entity &other) = 0;
	virtual void TakeDamage(int amount, Visualisation &v) = 0;
	virtual void Fire(Visualisation &v) = 0;
	virtual int GetHP()const { return m_health; }

	void AddToProjVector(ProjectileEntity *e) { m_projectileVector.push_back(e); } //Used to add projectiles to a ship.
};

