#pragma once
#include "Entity.h"
class ProjectileEntity : public Entity
{
private:
	//Explosion Handling
	bool m_isExplosion{ false };
	int m_explosionFrames{1};
	std::string m_explosionName{ "null" };

	void Initialize(Visualisation &v) override final;
public:
	ProjectileEntity(std::string _associatedSprite, std::string _explodeSprite, Visualisation &v, int _numFrames = 1, int _explosionFrames = 1);
	~ProjectileEntity();

	//Overrides
	void Update(Visualisation &v) override final;
	void DetectCollision(Visualisation &v, Entity &other) override final;
	void HasCollided(Visualisation &v, Entity &other) override final;
	void Render(Visualisation &v, float s) override final;

	void SetSide(ESide side);
	bool GetIsExplosion() const { return m_isExplosion; }
};

