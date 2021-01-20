#pragma once
#include "Vector2D.h"
class AI
{
private:
	Vector2D m_direction;
	Vector2D m_distanceMoved;
	bool m_isStationary{ false }; //If true, dosen't move down. Also dosen't loop path.
	int m_currentStep{ 0 }; //Which step of the path is the AI on.
	bool m_finishedCurrentPath{ false }; //A better idea would be to turn this into an enum.

	std::vector<Vector2D> m_movementPath;
public:
	AI(bool _isStationary);
	~AI();
	
	void AddToPath(Vector2D v);
	void ClearPath(); //Not really needed but it's there, so.

	Vector2D Run(Vector2D _pos, int _speed);	
};

