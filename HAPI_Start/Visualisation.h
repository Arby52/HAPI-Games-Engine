#pragma once
#include "Rectangle.h"
#include "Vector2D.h"

using namespace HAPISPACE;

class Textures;
class Sounds;
class UI;

class Visualisation
{

private:
	UI *m_UI{ nullptr };

	std::unordered_map<std::string, Textures*>m_textureMap; //Store all the textures.
	std::unordered_map<std::string, Sounds*>m_soundMap; //Store all the sounds.
	Rectangle m_screenRect;
	BYTE* m_screenPtr{ nullptr }; //Creates a pointer variable called screen.

	int m_screenWidth{ 1000 }; //Set window width.
	int m_screenHeight{ 800 }; //Set window height.
	Vector2D m_screenDimensions;
	int m_totalBytes{ m_screenWidth * m_screenHeight * 4 }; //Used in memset when affecting every pixel in the window.
	
public:
	Visualisation(int _width, int _height);
	~Visualisation();

	bool Initialize(int _width, int _height);	
	  
	Vector2D GetScreenDimensions() const {  return m_screenDimensions; }
	int GetCentreWidth() const { return m_screenWidth / 2; }
	int GetCentreHeight() const { return m_screenHeight / 2; }
	Rectangle GetTexRect(std::string _name) const;
	Rectangle GetScreenRect() const { return m_screenRect; }

	//Texture Functions
	bool CreateTexture(const std::string &_name, const std::string &_filepath, int _numFrames = 1);
	bool DeleteTexture(const std::string &_uniqueName);
	bool CheckTexture(const std::string &_uniqueName);
	bool Render(const std::string &_name, int _posX, int _posY, int _frameNum = 0) const;
	void DeleteAllTextures();
	bool SetIcon(const std::string &_uniqueName);

	//Sound Functions
	bool LoadSound(const std::string &_name, const std::string &_filepath);
	bool PlaySound(const std::string &_name);
	void CreateStreamedSound(const std::string &_name, const std::string &_filepath);
	void DeleteAllSounds();
	bool PlayStreamedSound(const std::string &_name);
	bool StopStreamedSound(const std::string &_name);

	bool SetVolume(const std::string &_name, float _vol = 1); //Float between 0 and 1.
	bool SetIsLooped(const std::string & _name, bool _l = false);
	//Could implement more sound option functions based on growing needs.

	//UI Functions
	void DisplayPlayerHP(Visualisation& _v, int _playerHP, std::string _spriteName);
	void AddToScore(int _x) ;
	void SaveScore();
	void DisplayHighScore();
	void DisplayScore(HAPI_TColour _fillColour, HAPI_TColour _outlineColour, float _outlineThickness, int _textSize);
	void DisplayScore(int _x, int _y, HAPI_TColour _fillColour, HAPI_TColour _outlineColour, float _outlineThickness, int _textSize); //Incase you want to manually change score position for some reason.
	void GameOver(int _i); //Takes in an int to determine which kind of game over happened.

	//Screen colour and pixel functions.
	void SetPixelColor(int _x, int _y, HAPI_TColour _color) const; //Set a specific pixel to a specific color. Defaults to white.
	void SetPixelColor(int _x, int _y, int _r, int _g, int _b) const;
	void WipeScreenToBlack() const;
	void WipeScreenToColor(int _r, int _g, int _b) const; //Make all pixels in the window a specific color.
	void WipeScreenToColor(HAPI_TColour _color) const;
	//This is where the scrolling background function would go if I placed it into the visualisation class, which is probably a good idea.

	//Color Declarations. More can be made as needed. Could be a good idea to make these private and create a funciton to return them based on a string.
	const HAPI_TColour h_black{ 0, 0, 0 }; //Black
	const HAPI_TColour h_white{ 255, 255, 255 }; //White
	const HAPI_TColour h_blue{ 0, 0, 255 }; //Blue
	const HAPI_TColour h_green{ 0, 255, 0 }; //Green
	const HAPI_TColour h_red{ 255, 0, 0 }; //Red
	const HAPI_TColour h_gray{ 170, 170, 170 }; //Gray
	const HAPI_TColour h_darkgray{ 60, 60, 60 }; //DarkGray	
};

