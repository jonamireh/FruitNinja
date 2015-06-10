#pragma once
#include "fmodex\fmod.hpp"
#include <glm/glm.hpp>
#include <map>

class AudioManager
{
private:
	static AudioManager *amInstance;

	FMOD::System *system;
	FMOD::Sound *ambientSound;
	FMOD::Channel *ambientChannel;
	std::map<std::string, FMOD::Sound*> soundMap;

	void FMODErrorCheck(FMOD_RESULT result);
	FMOD::Sound *preload_internal(std::string filename, bool loop, bool is3d);
	AudioManager();
public:
	static AudioManager* instance();

	void preload(std::string filename, bool loop, bool is3d);
	void playAmbient(std::string filename, float volume);
	void updateListener(glm::vec3 position, glm::vec3 forward, glm::vec3 up);
	FMOD::Channel *play3D(std::string filename, glm::vec3 position, float atten, bool loop);
	FMOD::Channel *play2D(std::string filename, bool loop, float volume);
	void updateChannelPosition(FMOD::Channel *channel, glm::vec3 position);
};