#include "pch.h"
#include "Textures.h"
#include "Rectangle.h"

Textures::Textures()
{
}

Textures::~Textures()
{
	delete m_texturePointer;
	m_texturePointer = nullptr;
}

bool Textures::LoadTexture(const std::string &_filepath, int _numFrames) { //Loading in a Texture from a file and creating a pointer for it.

	int w, h;
	if (!HAPI.LoadTexture(_filepath, &m_texturePointer, w, h)) {
		return false;
	}

	m_numFrames = _numFrames;
	m_textureRect = Rectangle(w, h);

	//Texture frames must be the same size and laid out horizontally. Frames can be any size rectangle.
	m_frameRect = Rectangle(m_textureRect.GetWidth() / _numFrames, m_textureRect.GetHeight());

	return true;
}

bool Textures::Render(const Rectangle& _screenRect, BYTE* _screen, int _posX, int _posY, int _frameNum)
{
	Rectangle clippedRect(m_frameRect);

	clippedRect.Translate(_posX, _posY);

	//If the sprite is fully off the screen
	if (clippedRect.IsFullyOutsideOf(_screenRect)) {
		return true;
	}

	//If the sprite is fully on the screen
	if (!clippedRect.IsFullyContainedIn(_screenRect)) {
		clippedRect.ClipTo(_screenRect);
	}

	clippedRect.Translate(-_posX, -_posY);

	if (_posX < 0) _posX = 0;
	if (_posY < 0) _posY = 0;

	clippedRect.Translate(m_frameRect.left, m_frameRect.top);   //frameNum * m_frameRect.getWidth(), 0. Used to have but caused an error where the final frames of an animation would be raised by a line of pixels.
	
	BYTE* screenPntr = _screen + (_posX + _posY * _screenRect.GetWidth()) * 4; //Where we want to draw to.	
	BYTE* texPntr = m_texturePointer + (clippedRect.left + clippedRect.top * m_textureRect.GetWidth()) * 4 + (m_frameRect.GetWidth() * _frameNum) * 4; //Where we want to draw from (the texture).

	int endOfLineScreenIncrement = (_screenRect.GetWidth() - clippedRect.GetWidth()) * 4; //Add to screen pointer at the end of a line.
	int endOfLineTextureIncrement = ((m_frameRect.GetWidth() - clippedRect.GetWidth()) + m_textureRect.GetWidth() - m_frameRect.GetWidth()) * 4; //Add to texture pointer at the end of a line.

	//Rendering
	for (int y = 0; y < clippedRect.GetHeight(); y++) {
		for (int x = 0; x < clippedRect.GetWidth(); x++) {
			BYTE blue = texPntr[0];
			BYTE green = texPntr[1];
			BYTE red = texPntr[2];
			BYTE alpha = texPntr[3];

			if (alpha == 0) {
				//Keep screen colour for that pixel.
			}
			else if (alpha == 255) {
				memcpy(screenPntr, texPntr, 4); //Copy an entire line of data from the texture into the screen.
			}
			else {
				screenPntr[0] = screenPntr[0] + ((alpha*(blue - screenPntr[0])) >> 8); //Result = Destination+(Alpha*(Source-Destination))>>8. >>8 is similar to diving the number by 2^8 (bit shifting). Since that is 256, not 255, it leaves a small inaccuraccy.
				screenPntr[1] = screenPntr[1] + ((alpha*(green - screenPntr[1])) >> 8);
				screenPntr[2] = screenPntr[2] + ((alpha*(red - screenPntr[2])) >> 8);
			}

			texPntr += 4; //Advance texture pointer to next pixel.
			screenPntr += 4; //Advance screen pointer to next pixel
		}
		texPntr += endOfLineTextureIncrement;
		screenPntr += endOfLineScreenIncrement; //Move the screen pointer down to the next line.
	}
	return true;
}

