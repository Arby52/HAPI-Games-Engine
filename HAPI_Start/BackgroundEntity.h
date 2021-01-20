#pragma once
#include "SceneryEntity.h"
#include "Vector2D.h"
class BackgroundEntity : public SceneryEntity
{
private:
	//Essentially has two positions. Rendered twice. Once one hits the bottom, loops back to the top.
	Vector2D m_pos2;
	Vector2D m_oldPos2;

	void Initialize(Visualisation &v) override final;
public:
	BackgroundEntity(std::string _associatedSprite, Visualisation &v);
	~BackgroundEntity();
	
	void Update(Visualisation &v) override final;
	void Render(Visualisation &v, float s) override final;
		
	void SetTopLeftPosition2(Vector2D newPos2) { m_oldPos2 = m_pos2; m_pos2 = newPos2; }
	Vector2D GetTopLeftPosition2() const { return m_pos2; }
	Vector2D GetOldPos2() const { return m_oldPos2; }
};

