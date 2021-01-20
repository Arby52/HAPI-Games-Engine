#pragma once
#include "Rectangle.h"
using namespace HAPISPACE;


class Textures
{

private:
	BYTE* m_texturePointer{ nullptr };
	Rectangle m_textureRect;
	Rectangle m_frameRect;
	int m_numFrames{ 1 };

public:
	Textures();
	~Textures();

	bool LoadTexture(const std::string &_filepath, int _numFrames=1);
	bool Render(const Rectangle& _screenRect, BYTE* _screen, int _posX, int _posY, int _frameNum = 0);

	BYTE* GetPointer() const { return m_texturePointer; }
	Rectangle GetRect() const { return m_frameRect; }

};

