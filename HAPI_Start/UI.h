#pragma once
#include "Vector2D.h"
using namespace HAPISPACE;

class Visualisation;

class UI
{
private:
	int m_score{ 0 };
	bool m_gameOver{ false }; //Used, essentially, to act as a pause for the score going up automatically when the game has ended and is waiting user response.
	DWORD m_lastTimeUpdated{ 0 };
	DWORD m_everySecond{ 1000 }; //Represents a second of time.

public:
	UI();
	~UI();

	void DisplayScore(int _x, int _y, HAPI_TColour _fillColour, HAPI_TColour _outlineColour, float _outlineThickness, int _textSize);
	void DisplayHighScore(int _x, int _y, HAPI_TColour _fillColour, HAPI_TColour _outlineColour, float _outlineThickness, int _textSize);
	void DisplayHP(Visualisation& _v, int _x, int _y, std::string _spriteName,  int _playerHP) const;

	void GameOver(int _x, int _y, HAPI_TColour _fillColour, HAPI_TColour _outlineColour, float _outlineThickness, int _textSize, std::string _text);
	void DisplayText(int _x, int _y, HAPI_TColour _fillColour, HAPI_TColour _outlineColour, float _outlineThickness, int _textSize, std::string _text) const;
	void SaveScore();
	void AddToScore(int x);
};

