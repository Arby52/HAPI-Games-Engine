#pragma once
#include "ShipEntity.h"
class PlayerEntity : public ShipEntity
{
private:
	void TakeDamage(int amount, Visualisation &v) override final;
	void Fire(Visualisation &v) override final;
	void Initialize(Visualisation &v) override final;
public:
	PlayerEntity(std::string _associatedSprite, Visualisation &v, int _numFrames = 1);
	~PlayerEntity();

	void Update(Visualisation &v) override final;
	void Render(Visualisation &v, float s) override final;
	void DetectCollision(Visualisation &v, Entity &other) override final;
	void HasCollided(Visualisation &v, Entity &other) override final;	

};

