#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "AudioManager.h"
#include "fmodex\fmod.hpp"
#include "fmodex\fmod_errors.h"

using namespace std;

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

	cout << "Audio successfully initialized!" << endl;
}

void AudioManager::play(string filename)
{
	FMOD_RESULT result;
	FMOD::Sound *sound;
	FMOD::Channel *channel;

	result = system->createSound(filename.c_str(), FMOD_DEFAULT | FMOD_LOOP_NORMAL, nullptr, &sound);
	FMODErrorCheck(result);

	result = system->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
	FMODErrorCheck(result);
}