#include "pch.h"
#include "AI.h"

AI::AI(bool _isStationary) : m_isStationary(_isStationary)
{
}

AI::~AI()
{
}

void AI::AddToPath(Vector2D v)
{
	m_movementPath.push_back(v);
	
}

void AI::ClearPath()
{
	m_movementPath.clear();
}

Vector2D AI::Run(Vector2D _pos, int _speed)
{
	if (!m_isStationary) { //Looping AI
		if (!m_finishedCurrentPath) { //If the path hasn't finished
			if ((m_movementPath[m_currentStep] - _pos).Magnitude() <= (Vector2D((float)_speed / 2, (float)_speed / 2)).Magnitude()) { //Check if the ship has reached its path goal
				m_currentStep++; //if it did, set next goal
				if (m_currentStep >= m_movementPath.size()) { //Check if the route has finished.
					m_finishedCurrentPath = true; //If it did, finish path.
					for (auto& p : m_movementPath) { //Shift all vectors down by 150.
						p -= Vector2D(0, -150);
					}
				}
			}
			m_direction = (m_movementPath[m_currentStep] - _pos).Normalize();
		} else { //If the path has finished
			m_currentStep = 1; //Loop around to the start (excluding spawn).
			m_direction = (m_movementPath[m_currentStep] - _pos).Normalize(); //Move toward it.
			if ((m_movementPath[m_currentStep] - _pos).Magnitude() <= (Vector2D((float)_speed / 2, (float)_speed / 2)).Magnitude()) { //Once it hits the start
				m_finishedCurrentPath = false; //Resume path
			}
		}
	}
	else { //If it is marked as stationary
		if (!m_finishedCurrentPath) {
			if ((m_movementPath[m_currentStep] - _pos).Magnitude() <= (Vector2D((float)_speed / 2, (float)_speed / 2)).Magnitude()) {
				m_currentStep++;
				if (m_currentStep >= m_movementPath.size()) {
					m_finishedCurrentPath = true;
				}
			}
			m_direction = (m_movementPath[m_currentStep] - _pos).Normalize();
		}
		else {
			m_currentStep = 1;
			m_direction = Vector2D(0, 0);
		}		
	}
	return m_direction;
}
