#pragma once
#include "Vector2D.h"
#include "Visualisation.h"

enum class ESide {
	ePlayer,
	eEnemy,
	eNeutral
};

class Visualisation;

class Entity
{
private:
	Vector2D m_pos;
	Vector2D m_oldPos;

protected:
	virtual void Initialize(Visualisation &v) = 0;

	std::string m_spriteName{ "Null" };
	Rectangle m_entityRectangle; //So an entity knows its own dimensions.

	bool m_isAlive{ true }; //Synonymous with isActive. 
	float m_speed{ 20 };
	int m_damage{ 0 }; //If Projectile and Ship inherited from something one step below entity, this would go there.
	ESide m_side{ ESide::eNeutral };

	int m_frameNumber{ 0 };
	int m_numFrames{ 1 };	

	DWORD m_lastTimeUpdatedAnimation{ 0 };
	DWORD m_animationTime{ 500 };
	
public:
	Entity(std::string _associatedSprite, Visualisation &v, int numFrames = 1);
	virtual ~Entity();
	
	//Position. Set and Get Position are in relation to the centre of the image. Set and Get TopLeft are in relation to the TopLeft.
	void SetPosition(Vector2D newPos) { m_oldPos = m_pos; m_pos = newPos - Vector2D((float)m_entityRectangle.GetCentreWidth(), (float)m_entityRectangle.GetCentreHeight()); } //Position relative to the centre of the entity.
	void SetTopLeftPosition(Vector2D newPos) { m_oldPos = m_pos; m_pos = newPos; } //Position relative to the top left of the entity.
	Vector2D GetPosition() const { return m_pos + Vector2D((float)m_entityRectangle.GetCentreWidth(), (float)m_entityRectangle.GetCentreHeight()); }
	Vector2D GetTopLeftPosition() const { return m_pos; }
	Vector2D GetOldPos() const { return m_oldPos; }
	
	bool CanCollideWith(ESide a, ESide b);	

	std::string GetSpriteName() const { return m_spriteName; }
	ESide GetSide() const { return m_side; }
	int GetDamage() const { return m_damage; }
	bool GetIsAlive() const { return m_isAlive; }
	void SetIsAlive(bool a){m_isAlive = a;}

	//Virtuals
	virtual void Update(Visualisation &v) = 0;
	virtual void DetectCollision(Visualisation &v, Entity &other) = 0;
	virtual void HasCollided(Visualisation &v, Entity &other) = 0;
	virtual void Render(Visualisation &v, float s) = 0;
	
	
};

