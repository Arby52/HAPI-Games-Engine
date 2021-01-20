#include "pch.h"
#include "Sounds.h"



Sounds::Sounds()
{
}


Sounds::~Sounds()
{
}

bool Sounds::LoadSound(const std::string & _name, const std::string & _filepath)
{
	if (!HAPI.LoadSound(_filepath)) {
		std::cerr << _name + " failed to load!" << std::endl;
		return false;
	}
	m_filepath = _filepath;
	m_name = _name;
		return true;
}

bool Sounds::PlaySound() const
{
	if (!HAPI.PlaySound(m_filepath, m_soundOptions)) {
		std::cerr << m_name + " failed to play!" << std::endl;
		return false;
	}
	return true;
}

void Sounds::CreateStreamedMedia(const std::string & _name, const std::string & _filepath)
{
	m_filepath = _filepath;
	m_name = _name;
}

bool Sounds::PlayStreamedMedia()
{
	int id = 0;
	if (!HAPI.PlayStreamedMedia(m_filepath, m_soundOptions, id)) {
		std::cerr << m_name + " stream failed to play!" << std::endl;
		return false;
	}
	m_instanceId = id;
	return true;
}


bool Sounds::StopStreamedMedia()
{
	if (!HAPI.StopStreamedMedia(m_instanceId)) {
		std::cerr << m_name + " stream failed to stop!" << std::endl;
		return false;
	}
	return true;
}

void Sounds::SetVolume(float volume)
{
	//clamp volume levels between 0 and 1.
	if (volume > 1) volume = 1;
	if (volume < 0) volume = 0;
	m_soundOptions.volume = volume;
}

void Sounds::SetIsLooped(bool l)
{
	m_soundOptions.loop = l;
}
