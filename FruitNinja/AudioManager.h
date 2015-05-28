#pragma once
#include "fmodex\fmod.hpp"

class AudioManager
{
	FMOD::System *system;
	void FMODErrorCheck(FMOD_RESULT result);
public:
	AudioManager();
	void play(std::string filename);
};