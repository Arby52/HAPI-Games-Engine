#include "pch.h"
#include "BackgroundEntity.h"


BackgroundEntity::BackgroundEntity(std::string _associatedSprite, Visualisation &v) : SceneryEntity(_associatedSprite, v)
{	
	Initialize(v);
}

BackgroundEntity::~BackgroundEntity()
{
}

void BackgroundEntity::Initialize(Visualisation & v)
{
	m_entityRectangle = (v.GetTexRect(m_spriteName));
	m_speed = 5;
	SetTopLeftPosition2(Vector2D(GetTopLeftPosition().x, GetTopLeftPosition().y - m_entityRectangle.GetHeight()));
}

void BackgroundEntity::Update(Visualisation &v)
{
	if (GetTopLeftPosition().y > v.GetScreenDimensions().y)
		SetTopLeftPosition(Vector2D(GetTopLeftPosition().x, GetTopLeftPosition2().y - m_entityRectangle.GetHeight()));

	if (GetTopLeftPosition2().y > v.GetScreenDimensions().y)
		SetTopLeftPosition2(Vector2D(GetTopLeftPosition2().x, GetTopLeftPosition().y - m_entityRectangle.GetHeight()));

	SetTopLeftPosition(GetTopLeftPosition() - Vector2D(0,-1)*m_speed);
	SetTopLeftPosition2(GetTopLeftPosition2() - Vector2D(0, -1)*m_speed);
}

void BackgroundEntity::Render(Visualisation &v, float s)
{
	if (!m_isAlive)
		return;

	Vector2D interPos{ GetOldPos() + (GetTopLeftPosition() - GetOldPos()) * s };
	Vector2D interPos2{ GetOldPos2() + (GetTopLeftPosition2()  - GetOldPos2()) * s };

	//Render twice.
	v.Render(m_spriteName, (int)interPos.x, (int)interPos.y, m_frameNumber);
	v.Render(m_spriteName, (int)interPos2.x, (int)interPos2.y , m_frameNumber);

	if (HAPI.GetTime() - m_lastTimeUpdatedAnimation >= m_animationTime)
	{
		m_frameNumber++;
		if (m_frameNumber >= m_numFrames)
			m_frameNumber = 0;

		m_lastTimeUpdatedAnimation = HAPI.GetTime();
	}

}