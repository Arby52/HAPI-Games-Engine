#include "pch.h"
#include "ProjectileEntity.h"

ProjectileEntity::ProjectileEntity(std::string _associatedSprite, std::string _explodeSprite, Visualisation &v, int _numFrames, int _explosionFrames) : Entity(_associatedSprite, v, _numFrames)
{
	Initialize(v);
	m_explosionFrames = _explosionFrames;
	m_explosionName = _explodeSprite;
}

ProjectileEntity::~ProjectileEntity()
{
}

void ProjectileEntity::Initialize(Visualisation & v)
{
	m_isAlive = false;
	m_animationTime = 100;
	m_speed = 25;
	m_damage = 10;
	m_entityRectangle = (v.GetTexRect(m_spriteName));
}

void ProjectileEntity::Update(Visualisation &v)
{
	//If player shoots, go up. If enemy shoots, go down. Would need to change if enemies gain functionality to change direction.
	Vector2D direction;
	if (m_side == ESide::ePlayer) 
		direction = Vector2D(0, -1); //Up
	else if (m_side == ESide::eEnemy)
		direction = Vector2D(0, 1); //Down

	SetPosition(GetPosition());

	//If it isn't exploding, move.
	if (!m_isExplosion) {
		if (GetIsAlive()) {
			SetPosition(GetPosition() + (direction * m_speed));
			if (GetPosition().y <= 0 || GetPosition().y >= v.GetScreenDimensions().y) {
				SetIsAlive(0);
				m_isExplosion = false;
			}
		}
	}
}

void ProjectileEntity::DetectCollision(Visualisation & v, Entity & other)
{
	if (!GetIsAlive() || !other.GetIsAlive())
		return;

	if (!CanCollideWith(GetSide(), other.GetSide()))
		return;

	Rectangle thisRect{ m_entityRectangle };

	int w{ thisRect.GetWidth() };
	int h{ thisRect.GetHeight() };

	//Decrease hitbox slightly
	thisRect.left += w / 15;
	thisRect.right -= w / 15;
	thisRect.top += w / 15;
	thisRect.bottom -= w / 15;

	Rectangle otherRect{ v.GetTexRect(other.GetSpriteName()) };

	thisRect.Translate((int)GetTopLeftPosition().x, (int)GetTopLeftPosition().y);
	otherRect.Translate((int)other.GetTopLeftPosition().x, (int)other.GetTopLeftPosition().y);

	if (!m_isExplosion) {
		if (!thisRect.IsFullyOutsideOf(otherRect)) {
			//Collision
			other.HasCollided(v, *this);
			HasCollided(v, other);
		}
	}
}

void ProjectileEntity::HasCollided(Visualisation & v, Entity &other)
{
	v.PlaySound("explode1");
	m_isExplosion = true;
	m_isAlive = false;
	m_frameNumber = 0;
}

void ProjectileEntity::SetSide(ESide side)
{
	m_side = side;
}

void ProjectileEntity::Render(Visualisation &v, float s)
{
	if (!m_isAlive && !m_isExplosion)
		return;

	Vector2D interPos{ GetOldPos() + (GetPosition() - Vector2D((float)m_entityRectangle.GetCentreWidth(), (float)m_entityRectangle.GetCentreHeight()) - GetOldPos()) * s };
	if (!m_isExplosion) {
		v.Render(m_spriteName, (int)interPos.x, (int)interPos.y, m_frameNumber);
	}
	else {
		v.Render(m_explosionName, (int)GetPosition().x, (int)GetPosition().y, m_frameNumber);
	}
	

	if (HAPI.GetTime() - m_lastTimeUpdatedAnimation >= m_animationTime)
	{
		m_frameNumber++;
		if (!m_isExplosion) {
			if (m_frameNumber >= m_numFrames)
				m_frameNumber = 0;
		}
		else { //If projectile is exploding, use different animation variables.
			if (m_frameNumber >= m_explosionFrames) {
				SetIsAlive(0);
				m_isExplosion = false;
			}
		}	

		m_lastTimeUpdatedAnimation = HAPI.GetTime();
	}

}