#pragma once
#include "Entity.h"
class SceneryEntity : public Entity
{
public:
	SceneryEntity(std::string _associatedSprite, Visualisation &v);
	~SceneryEntity();

	//No use at the moment. Override declaration needed to keep base Entity virtuals happy. Could be used if future scenery objects collide.
	void DetectCollision(Visualisation &v, Entity &other) override final;
	void HasCollided(Visualisation &v, Entity &other) override final;
};

