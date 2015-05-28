#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "AudioManager.h"
#include "fmodex\fmod.hpp"
#include "fmodex\fmod_errors.h"
#include "Camera.h"

using namespace std;

AudioManager *AudioManager::amInstance = nullptr;

void AudioManager::FMODErrorCheck(FMOD_RESULT result)
{
	if (result != FMOD_OK) {
		cout << "FMOD error :(" << endl;
		exit(-1);
	}
}

AudioManager::AudioManager()
{
	FMOD_RESULT result;
	unsigned int version;
	int numdrivers;
	FMOD_SPEAKERMODE speakermode;
	FMOD_CAPS caps;
	char name[256];

	result = FMOD::System_Create(&system);
	FMODErrorCheck(result);

	result = system->getVersion(&version);
	FMODErrorCheck(result);

	if (version < FMOD_VERSION) {
		cout << "Your FMOD DLL is too old!!  You have version " << version << "; this program requires version " << FMOD_VERSION << "." << endl;
		exit(-1);
	}

	result = system->getNumDrivers(&numdrivers);
	FMODErrorCheck(result);

	if (numdrivers == 0) {
		result = system->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
		FMODErrorCheck(result);
	}
	else {
		result = system->getDriverCaps(0, &caps, 0, &speakermode);
		FMODErrorCheck(result);

		result = system->setSpeakerMode(speakermode);
		FMODErrorCheck(result);

		if (caps & FMOD_CAPS_HARDWARE_EMULATED) {
			// No hardware audio acceleration
			result = system->setDSPBufferSize(1024, 10);
			FMODErrorCheck(result);
		}

		result = system->getDriverInfo(0, name, 256, 0);
		FMODErrorCheck(result);

		if (strstr(name, "SigmaTel")) {
			result = system->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0, 0, FMOD_DSP_RESAMPLER_LINEAR);
			FMODErrorCheck(result);
		}
	}

	result = system->init(100, FMOD_INIT_NORMAL, 0);
	if (result == FMOD_ERR_OUTPUT_CREATEBUFFER) {
		result = system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
		FMODErrorCheck(result);

		result = system->init(100, FMOD_INIT_NORMAL, 0);
	}
	FMODErrorCheck(result);

	ambientSound = nullptr;
	ambientChannel = nullptr;

	cout << "Audio successfully initialized!" << endl;
}

AudioManager* AudioManager::instance()
{
	if (amInstance == nullptr)
	{
		amInstance = new AudioManager();
	}

	return amInstance;
}

void AudioManager::playAmbient(string filename, float volume)
{
	FMOD_RESULT result;

	if (ambientChannel != nullptr)
	{
		ambientChannel->stop();
	}

	if (ambientSound != nullptr)
	{
		ambientSound->release();
	}

	result = system->createSound(filename.c_str(), FMOD_DEFAULT | FMOD_LOOP_NORMAL, nullptr, &ambientSound);
	FMODErrorCheck(result);

	result = system->playSound(FMOD_CHANNEL_FREE, ambientSound, true, &ambientChannel);
	FMODErrorCheck(result);

	result = ambientChannel->setVolume(volume);
	FMODErrorCheck(result);

	result = ambientChannel->setPaused(false);
	FMODErrorCheck(result);
}

void AudioManager::updateListener(glm::vec3 position, glm::vec3 forward, glm::vec3 up)
{
	FMOD_VECTOR pos = { position.x, position.y, -position.z };
	FMOD_VECTOR fwd = { forward.x, forward.y, -forward.z };
	FMOD_VECTOR upvec = { up.x, up.y, -up.z };
	FMOD_RESULT result;
	result = system->set3DListenerAttributes(0, &pos, nullptr, &fwd, &upvec);
	FMODErrorCheck(result);

	system->update();
	FMODErrorCheck(result);
}

FMOD::Channel *AudioManager::play3DLoop(string filename, glm::vec3 position, bool loop)
{
	FMOD_RESULT result;
	FMOD::Sound *sound;
	map<string, FMOD::Sound*>::iterator soundItr = soundMap.find(filename);

	if (soundItr == soundMap.end()) {
		cout << "Loading " << filename << "..." << endl;
		result = system->createSound(filename.c_str(), FMOD_3D | (loop ? FMOD_LOOP_NORMAL : 0), nullptr, &sound);
		FMODErrorCheck(result);

		cout << "Loaded " << filename << endl;

		soundMap.insert(pair<string, FMOD::Sound*>(filename, sound));
	}
	else {
		sound = soundItr->second;
	}

	FMOD::Channel *channel;
	result = system->playSound(FMOD_CHANNEL_FREE, sound, true, &channel);
	FMODErrorCheck(result);

	updateChannelPosition(channel, position);

	result = channel->setPaused(false);
	FMODErrorCheck(result);

	return channel;
}

void AudioManager::updateChannelPosition(FMOD::Channel* channel, glm::vec3 position)
{
	FMOD_VECTOR pos = { position.x, position.y, -position.z };
	FMOD_RESULT result;
	result = channel->set3DAttributes(&pos, nullptr);
	FMODErrorCheck(result);
}