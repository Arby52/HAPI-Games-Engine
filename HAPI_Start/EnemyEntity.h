#pragma once
#include "ShipEntity.h"
#include "AI.h"

class AI;

enum class EEnemyType{eBasic, eBoss}; //Probably best to make subclasses of EnemyEntity. A boss one, and a regular one, to separate variables.
enum class ECannonSide{eLeft, eRight};

class EnemyEntity :	public ShipEntity
{

private:
	AI *m_AI{ nullptr }; //Each ship has an AI object attached storing movement data.
	EEnemyType m_enemytype;	
	bool m_hasSpawned{ false };
	int m_worth{ 70 }; //How much score the ship will give when it is killed.

	ECannonSide m_bossCurrentCannon = ECannonSide::eLeft; //Used for the boss to match the firing animation.
	void TakeDamage(int amount, Visualisation &v) override final;

	void Initialize(Visualisation &v) override final;
public:
	EnemyEntity(std::string _associatedSprite, EEnemyType _enemyType, Visualisation &v, int _numFrames = 1);
	~EnemyEntity();

	//Overriden Functions
	void Update(Visualisation &v) override final;
	void Render(Visualisation &v, float s) override final;
	void DetectCollision(Visualisation &v, Entity &other) override final;
	void HasCollided(Visualisation &v, Entity &other) override final;

	void SetAIPath(std::vector<Vector2D> _paths);
	void SetHasSpawned(bool _hasSpawned) { m_hasSpawned = _hasSpawned; }
	bool GetHasSpawned() const { return m_hasSpawned; }

	void Fire(Visualisation &v) override final;
};

