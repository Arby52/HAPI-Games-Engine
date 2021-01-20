#include "pch.h"
#include "UI.h"
#include "Visualisation.h"

UI::UI()
{
}

UI::~UI()
{
}

void UI::DisplayScore(int _x, int _y, HAPI_TColour _fillColour, HAPI_TColour _outlineColour, float _outlineThickness, int _textSize)
{
	//Every second, add one to the score.
	if (!m_gameOver) {
		if (HAPI.GetTime() - m_lastTimeUpdated >= m_everySecond)
		{
			AddToScore(1);
			m_lastTimeUpdated = HAPI.GetTime();
		}
	}

	HAPI.RenderText(_x, _y, _fillColour, _outlineColour, _outlineThickness, "Score: " + to_string(m_score), _textSize);
}

void UI::DisplayHighScore(int _x, int _y, HAPI_TColour _fillColour, HAPI_TColour _outlineColour, float _outlineThickness, int _textSize)
{
	//Read in the highscore from file.
	std::ifstream scoreFileIn;
	scoreFileIn.open("score.txt");
	if (scoreFileIn.fail()) {
		std::cerr << "Error opening score file." << std::endl;
		m_gameOver = false;
		m_score = 0;
		return;
	}
	int data = 0;
	scoreFileIn >> data;
	//Display Highscore.
	HAPI.RenderText(_x, _y, _fillColour, _outlineColour, _outlineThickness, "Highscore: " + to_string(data), _textSize);
}

void UI::DisplayHP(Visualisation& _v, int _x, int _y, std::string _spriteName, int _playerHP) const
{	
	//Change HP display based on player health.
	int frameNum;
	switch (_playerHP) {
	case 50:
		frameNum = 5;
		break;
	case 40:
		frameNum = 4;
		break;
	case 30:
		frameNum = 3;
		break;
	case 20:
		frameNum = 2;
		break;
	case 10:
		frameNum = 1;
		break;
	case 0:
		frameNum = 0;
		break;
	default:
		frameNum = 0;
		break;
			
	}
	_v.Render(_spriteName, _x, _y, frameNum);
}

void UI::GameOver(int _x, int _y, HAPI_TColour _fillColour, HAPI_TColour _outlineColour, float _outlineThickness, int _textSize, std::string _text)
{
	HAPI.RenderText(_x, _y, _fillColour, _outlineColour, _outlineThickness, _text, _textSize);
	m_gameOver = true;
}

void UI::DisplayText(int _x, int _y, HAPI_TColour _fillColour, HAPI_TColour _outlineColour, float _outlineThickness, int _textSize, std::string _text) const
{
	HAPI.RenderText(_x, _y, _fillColour, _outlineColour, _outlineThickness, _text, _textSize);
}

void UI::SaveScore()
{	 
	//Open score file and read in text.
	std::ifstream scoreFileIn;
	scoreFileIn.open("score.txt");
	if (scoreFileIn.fail()) {
		std::cerr << "Error opening score file." << std::endl;
		m_gameOver = false;
		m_score = 0;
		return;
	}

	int data{0}; //incase file was empty, initialise data to 0;
	int result{ 0 };
	scoreFileIn >> data;
	if (m_score > data) //If the local score is higher than the stored score
		result = m_score; //Store Local
	else
		result = data; //Store Saved
	scoreFileIn.close();

	std::ofstream scoreFileOut;
	scoreFileOut.open("score.txt");
	if (scoreFileOut.fail()) {
		std::cerr << "Error opening score file." << std::endl;
		m_gameOver = false;
		m_score = 0;
		return;
	}

	scoreFileOut << to_string(result); //Store the higher score value.
	scoreFileOut.close();
	m_gameOver = false;
	m_score = 0; //Reset local score for next game.
}

void UI::AddToScore(int _x)
{
	m_score += _x;
}
