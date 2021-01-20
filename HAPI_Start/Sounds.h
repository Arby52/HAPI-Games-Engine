#pragma once

using namespace HAPISPACE;

class Sounds
{
private:

	std::string m_filepath;
	std::string m_name;
	int m_instanceId;
	HAPI_TSoundOptions m_soundOptions;
	
public:
	Sounds();
	~Sounds();

	bool LoadSound(const std::string &_name, const std::string &_filepath);
	bool PlaySound() const;
	void CreateStreamedMedia(const std::string &_name, const std::string & _filepath);
	bool PlayStreamedMedia();
	bool StopStreamedMedia();

	void SetVolume(float volume = 1); //Float between 0 and 1.
	void SetIsLooped(bool l = false);

};

