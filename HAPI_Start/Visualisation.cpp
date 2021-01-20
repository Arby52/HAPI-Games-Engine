#include "pch.h"
#include "Rectangle.h"
#include "Visualisation.h"
#include "Textures.h"
#include "Sounds.h"
#include "UI.h"

Visualisation::Visualisation(int _width, int _height)
{
	Initialize(_width, _height);
}

Visualisation::~Visualisation()
{
	delete m_UI;
	m_UI = nullptr;

}

void Visualisation::DeleteAllTextures() {
	for (auto& p : m_textureMap) //Delete remaining texture pointers.
	{
		delete p.second; //Delete the pointer.
		p.second = nullptr;
	}
	m_textureMap.clear();
}

void Visualisation::DeleteAllSounds() {
	for (auto& p : m_soundMap) //Delete remaining texture pointers.
	{
		delete p.second; //Delete the pointer.
		p.second = nullptr;
	}
	m_soundMap.clear();
}

bool Visualisation::Initialize(int _width, int _height) 
{
	m_screenWidth = _width;
	m_screenHeight = _height;
	m_totalBytes = m_screenWidth * m_screenHeight * 4;
	m_screenDimensions = Vector2D((float)m_screenWidth, (float)m_screenHeight);
	
	if (!HAPI.Initialise(m_screenWidth, m_screenHeight, "Space Shooter", eDefaults)) {  //Create window.
		std::cerr << "HAPI failed to initialise" << std::endl;
		return false; //If initialisation didn't work, end program.
	}
	m_screenRect = Rectangle(m_screenWidth, m_screenHeight);
	m_screenPtr = HAPI.GetScreenPointer();

	HAPI.SetShowFPS(false, 25, 25); //Show an FPS counter in the top left.

	m_UI = new UI;

	return true;
}

bool Visualisation::CreateTexture(const std::string &_name, const std::string &_filepath, int _numFrames) { //Loading in a sprite using the Textures class, giving it a name, and put it into the spritemap.
	Textures* texture = new Textures();
	if (!texture->LoadTexture(_filepath, _numFrames)) { //If the texture didn't load for any reason, close program.
		std::cerr << _name + "texture failed to load." << std::endl;
		HAPI.UserMessage(_name + " texture failed to load.", "Error!");
		delete texture;
		return false;
	}
	m_textureMap[_name] = texture; //Loads the pointer into the map.
	return true;
}

bool Visualisation::SetIcon(const std::string &_uniqueName) {
	if (m_textureMap.find(_uniqueName) == m_textureMap.end()) { //If the name cannot be found, it returns .end()
		std::cerr << _uniqueName << " isn't in the texture map." << std::endl;
		return false;
	}
	HAPI.SetIcon(m_textureMap[_uniqueName]->GetPointer(), m_textureMap[_uniqueName]->GetRect().GetWidth(), m_textureMap[_uniqueName]->GetRect().GetHeight());
	return true;
}

bool Visualisation::DeleteTexture( const std::string &_uniqueName)  { //Used to delete individual textures mid-way through the program.
	delete m_textureMap.find(_uniqueName)->second; //Delete the pointer.
	m_textureMap.erase(_uniqueName); //Delete the entry.
	return true;
}

bool Visualisation::CheckTexture(const std::string & _uniqueName)
{
	if (m_textureMap.find(_uniqueName) == m_textureMap.end()) { //If the name cannot be found, it returns .end()
		std::cerr << _uniqueName << " isn't in the texture map." << std::endl;
		return false;
	}
	return true;
}


bool Visualisation::Render(const std::string &_name, int _posX, int _posY, int _frameNum) const { //Blit
	if (m_textureMap.find(_name) == m_textureMap.end()) { //If the name cannot be found, it returns .end()
		std::cerr << _name << " isn't in the texture map." << std::endl;
		return false;
	}
	m_textureMap.at(_name)->Render(m_screenRect, m_screenPtr, _posX, _posY, _frameNum);
	return true;
}

bool Visualisation::LoadSound(const std::string & _name, const std::string & _filepath)
{
	Sounds* sound = new Sounds();
	if (!sound->LoadSound(_name, _filepath)) { //Error reporting done in the Sounds class.
		delete sound;
		return false;
	}
	m_soundMap[_name] = sound; //Loads the pointer into the map.
	return true;
}

bool Visualisation::PlaySound(const std::string & _name)
{
	if (m_soundMap.find(_name) == m_soundMap.end()) { //If the name cannot be found, it returns .end()
		std::cerr << _name << " isn't in the sound map." << std::endl;
		return false;
	}
	m_soundMap.at(_name)->PlaySound();
	return true;
}

bool Visualisation::SetVolume(const std::string & _name, float _vol)
{
	if (m_soundMap.find(_name) == m_soundMap.end()) { //If the name cannot be found, it returns .end()
		std::cerr << _name << " isn't in the sound map." << std::endl;
		return false;
	}
	m_soundMap.at(_name)->SetVolume(_vol);
	return true;
}

bool Visualisation::SetIsLooped(const std::string & _name, bool _l)
{
	if (m_soundMap.find(_name) == m_soundMap.end()) { //If the name cannot be found, it returns .end()
		std::cerr << _name << " isn't in the sound map." << std::endl;
		return false;
	}
	m_soundMap.at(_name)->SetIsLooped(_l);
	return true;
}

void Visualisation::CreateStreamedSound(const std::string & _name, const std::string & _filepath)
{
	Sounds* sound = new Sounds();
	sound->CreateStreamedMedia(_name, _filepath);
	m_soundMap[_name] = sound; //Loads the pointer into the map.
	std::clog << "Stream sound created" << std::endl;
}

bool Visualisation::PlayStreamedSound(const std::string & _name)
{
	if (m_soundMap.find(_name) == m_soundMap.end()) { //If the name cannot be found, it returns .end()
		std::cerr << _name << " isn't in the sound map." << std::endl;
		return false;
	}
	if (!m_soundMap.at(_name)->PlayStreamedMedia()) {
		return false;
	}
	return true;
}

bool Visualisation::StopStreamedSound(const std::string & _name)
{
	if (m_soundMap.find(_name) == m_soundMap.end()) { //If the name cannot be found, it returns .end()
		std::cerr << _name << " isn't in the sound map." << std::endl;
		return false;
	}
	if (!m_soundMap.at(_name)->StopStreamedMedia()) {
		return false;
	}
	return true;
}

void Visualisation::AddToScore(int _x)
{
	m_UI->AddToScore(_x);
}

void Visualisation::SaveScore()
{
	m_UI->SaveScore();
}

void Visualisation::DisplayScore(HAPI_TColour _fillColour, HAPI_TColour _outlineColour, float _outlineThickness, int _textSize)
{
	int x = 0;
	int y = 0;
	m_UI->DisplayScore(x, y, _fillColour, _outlineColour, _outlineThickness, _textSize);
}

void Visualisation::DisplayHighScore()
{
	m_UI->DisplayHighScore((int)(GetScreenDimensions().x * 0.425), (int)(GetScreenDimensions().y * 0.4), h_white, h_black, 3, 24);
}

void Visualisation::DisplayPlayerHP(Visualisation& _v, int _playerHP, std::string _spriteName)
{
	int x = 0;
	int y = (int)GetScreenDimensions().y - (int)GetScreenDimensions().y / 12;
	m_UI->DisplayHP(_v, x, y,_spriteName, _playerHP);
}

void Visualisation::DisplayScore(int _x, int _y, HAPI_TColour _fillColour, HAPI_TColour _outlineColour, float _outlineThickness, int _textSize)
{
	m_UI->DisplayScore(_x, _y, _fillColour, _outlineColour, _outlineThickness, _textSize);
}

void Visualisation::GameOver(int _i)
{
	DisplayHighScore();
	switch (_i) {
	case 0:
		m_UI->GameOver((int)(GetScreenDimensions().x / 2.25), GetCentreHeight(), h_white, h_black, 3, 24, "You Lose!");
		m_UI->DisplayText((int)(GetScreenDimensions().x / 2.45), (int)(GetCentreHeight()+GetScreenDimensions().y/20), h_white, h_black, 3, 24, "Press ESC to quit!");
		m_UI->DisplayText((int)(GetScreenDimensions().x / 2.45), (int)(GetCentreHeight() +(( GetScreenDimensions().y /20)*2)), h_white, h_black, 3, 24, "Press R to restart!");
		break;
	case 1:
		m_UI->GameOver((int)(GetScreenDimensions().x / 2.25), (int)GetCentreHeight(), h_white, h_black, 3, 24, "You Win!");
		m_UI->DisplayText((int)(GetScreenDimensions().x / 2.45), (int)(GetCentreHeight() + GetScreenDimensions().y / 20), h_white, h_black, 3, 24, "Press ESC to quit!");
		m_UI->DisplayText((int)(GetScreenDimensions().x / 2.45), (int)(GetCentreHeight() + ((GetScreenDimensions().y / 20) * 2)), h_white, h_black, 3, 24, "Press R to restart!");
		break;
	default:
		m_UI->GameOver((int)(GetScreenDimensions().x / 2.25), GetCentreHeight(), h_white, h_black, 3, 24, "This isn't supposed to be here!");
		m_UI->DisplayText((int)(GetScreenDimensions().x / 2.45), (int)(GetCentreHeight() + GetScreenDimensions().y / 20), h_white, h_black, 3, 24, "Press ESC to quit!");
		m_UI->DisplayText((int)(GetScreenDimensions().x / 2.45), (int)(GetCentreHeight() + ((GetScreenDimensions().y / 20) * 2)), h_white, h_black, 3, 24, "Press R to restart!");
		break;

	}
	
}


Rectangle Visualisation::GetTexRect(std::string _name) const
{
	if (m_textureMap.find(_name) == m_textureMap.end()) {
		std::cerr << "Texture does not exist" << std::endl;
		return Rectangle(0, 0);
	}
	Rectangle r(m_textureMap.at(_name)->GetRect());
	return Rectangle(r);
}

void Visualisation::SetPixelColor(int _x, int _y, HAPI_TColour _color) const {
	if (_x > m_screenWidth || _x < 0) //If out of bounds, return.
		return;
	if (_y > m_screenHeight || _y < 0)
		return;
	unsigned int offset = ((_x)+_y * m_screenWidth) * 4; //Translates the 2d x and y values to be changed into a 1d memory value.
	BYTE* pnter = m_screenPtr + offset; //Creates a BYTE sized point in memory at the location equal to x=0,y=0 + offset.
	memcpy(pnter, &_color, 4); //Copy the HAPI_TColor values to the value of the pointer known as pnter. Must specify pointer size which is 4.
}

void Visualisation::SetPixelColor(int _x, int _y, int _r, int _g, int _b) const {
	HAPI_TColour color(_r, _g, _b);
	if (_x > m_screenWidth || _x < 0) //If out of bounds, return.
		return;
	if (_y > m_screenHeight || _y < 0)
		return;
	unsigned int offset = ((_x)+_y * m_screenWidth) * 4; //Translates the 2d x and y values to be changed into a 1d memory value.
	BYTE* pnter = m_screenPtr + offset; //Creates a BYTE sized point in memory at the location equal to x=0,y=0 + offset.
	memcpy(pnter, &color, 4); //Copy the HAPI_TColor values to the value of the pointer known as pnter. Must specify pointer size which is 4.
}

void Visualisation::WipeScreenToBlack() const {
	memset(m_screenPtr, 0, m_totalBytes); //Set all values in the screen pointer to 0, essentially black.
}


void Visualisation::WipeScreenToColor(int _r, int _g, int _b) const {
	HAPI_TColour color(_r, _g, _b);
	BYTE* pnter = m_screenPtr;
	for (int i = 0; i < m_totalBytes; i += 4) {
		memcpy(pnter + i, &color, 4);
	}
}

void Visualisation::WipeScreenToColor(HAPI_TColour _color) const { //Same as above, allowing for HAPI_TColour types.
	BYTE* pnter = m_screenPtr;
	for (int i = 0; i < m_totalBytes; i += 4) {
		memcpy(pnter + i, &_color, 4);
	}
}