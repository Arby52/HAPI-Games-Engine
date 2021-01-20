#pragma once
#include "Vector2D.h"

class Entity;
class EnemyEntity;
class PlayerEntity;
class Visualisation;

class WorldModel
{
private:
	Visualisation *m_visualisation{ nullptr };
	PlayerEntity *m_player{ nullptr };
	EnemyEntity *m_boss{ nullptr };
	std::vector<Entity*> m_entityVector;	
	bool m_gameOver{ false };

	//Used to stagger the spawning of enemy entities.
	std::deque<EnemyEntity*> m_enemySpawnStack;

	bool LoadLevel();
	bool Update(bool& _restart);	
	void FreeMemory(); //Upon Restarting the game, or Exiting the game, free all memory and tie up other loose ends.
	bool GameOverCheck() const;

public:
	WorldModel(Visualisation* _vis);
	~WorldModel();

	void Run(bool& _restart);	
};
