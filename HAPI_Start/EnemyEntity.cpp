#include "pch.h"
#include "EnemyEntity.h"
#include "AI.h"


EnemyEntity::EnemyEntity(std::string _associatedSprite, EEnemyType _enemyType, Visualisation &v, int _numFrames) : ShipEntity(_associatedSprite, v, _numFrames)
{
	m_enemytype = _enemyType;
	switch (m_enemytype) {
	case EEnemyType::eBoss: //Populate data based on enemy type
		m_AI = new AI(true);
		m_animationTime = 100;
		m_fireRate = 400;
		m_worth = 250;
		break;
	case EEnemyType::eBasic:
		m_AI = new AI(false);
		m_fireRate = rand() % 1500 + 1000;
		break;
	}
	srand((unsigned int)time(NULL));
	Initialize(v);		
}

EnemyEntity::~EnemyEntity()
{
	delete m_AI;
}

void EnemyEntity::Initialize(Visualisation & v)
{
	m_isAlive = false;
	m_speed = 13;	
	m_side = ESide::eEnemy;	
	m_entityRectangle = (v.GetTexRect(m_spriteName));
}

void EnemyEntity::SetAIPath(std::vector<Vector2D> _paths)
{
	for (auto p : _paths) {
		m_AI->AddToPath(p);
	}
	SetPosition(_paths[0]);
}

void EnemyEntity::Update(Visualisation &v)
{
	//Movement AI	
	//AI Logic	
	if (GetIsAlive()) {
		SetPosition(GetPosition() + (m_AI->Run(GetPosition(), (int)m_speed) * m_speed));
	}

	//Make sure the projectiles are ontop of their respective ship.
	for (auto p : m_projectileVector) {
		if (!p->GetIsAlive() && !p->GetIsExplosion()) {
			if (m_enemytype == EEnemyType::eBoss) {
				if (m_bossCurrentCannon == ECannonSide::eLeft) {
					p->SetPosition(Vector2D(GetPosition().x - 20, GetPosition().y + m_entityRectangle.GetCentreHeight()));
				}
				else if (m_bossCurrentCannon == ECannonSide::eRight) {
					p->SetPosition(Vector2D(GetPosition().x + 20, GetPosition().y + m_entityRectangle.GetCentreHeight()));
				}
			}
			else {
				p->SetPosition(Vector2D(GetPosition().x, GetPosition().y + m_entityRectangle.GetCentreHeight()));
			}
		}
	}
	
	Fire(v);
}

void EnemyEntity::Fire(Visualisation &v)
{
	if (GetIsAlive()) {
		if (HAPI.GetTime() - m_lastTimeUpdatedFireRate >= m_fireRate)
		{
			for (auto p : m_projectileVector) {
				if (!p->GetIsAlive() && !p->GetIsExplosion()) {
					if (m_enemytype == EEnemyType::eBoss) { //Different starting positions based on if its the boss or not.
						if (m_bossCurrentCannon == ECannonSide::eLeft) {
							p->SetPosition(Vector2D(GetPosition().x - 20, GetPosition().y + m_entityRectangle.GetCentreHeight()));
							m_bossCurrentCannon = ECannonSide::eRight;
						}
						else if (m_bossCurrentCannon == ECannonSide::eRight) {
							p->SetPosition(Vector2D(GetPosition().x + 20, GetPosition().y + m_entityRectangle.GetCentreHeight()));
							m_bossCurrentCannon = ECannonSide::eLeft;
						}
					}
					else {
						p->SetPosition(Vector2D(GetPosition().x, GetPosition().y + m_entityRectangle.GetCentreHeight()));
					}
					p->SetIsAlive(1);
					break;
				}
			}
			m_lastTimeUpdatedFireRate = HAPI.GetTime();
		}
	}
}

void EnemyEntity::DetectCollision(Visualisation & v, Entity & other)
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

	if (!thisRect.IsFullyOutsideOf(otherRect)) {
		//Collision
		other.HasCollided(v, *this);
		HasCollided(v, other);
	}

}

void EnemyEntity::HasCollided(Visualisation & v, Entity &other)
{
	TakeDamage(other.GetDamage(), v);
}

void EnemyEntity::Render(Visualisation &v, float s)
{
	if (!m_isAlive)
		return;

	Vector2D interPos{ GetOldPos() + (GetPosition() - Vector2D((float)m_entityRectangle.GetCentreWidth(), (float)m_entityRectangle.GetCentreHeight()) - GetOldPos()) * s };

	v.Render(m_spriteName, (int)interPos.x, (int)interPos.y, m_frameNumber);

	if (HAPI.GetTime() - m_lastTimeUpdatedAnimation >= m_animationTime)
	{
		m_frameNumber++;
		if (m_frameNumber >= m_numFrames)
			m_frameNumber = 0;

		m_lastTimeUpdatedAnimation = HAPI.GetTime();
	}

}


void EnemyEntity::TakeDamage(int amount, Visualisation &v)
{
	m_health -= amount;

	if (m_health <= 0) {
		v.PlaySound("explode2");
		v.AddToScore(m_worth);
     		SetIsAlive(0);
	}
}
