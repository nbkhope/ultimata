#include "Sound.h"

#include <iostream>

Sound::Sound() : music(nullptr), available(false)
{
	// Initializes sound mixer
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
	{
		std::cerr << "Mix_OpenAudio failed: " << Mix_GetError() << std::endl;
		return;
	}

	music = Mix_LoadMUS("data/sound/music.wav");
	if (!music)
	{
		std::cerr << "Mix_LoadMUS failed: " << Mix_GetError() << std::endl;
		return;
	}

	available = true;
}

Sound::~Sound()
{
	if (music)
	{
		Mix_FreeMusic(music);
		music = nullptr;
	}

	Mix_CloseAudio();
}

void Sound::play()
{
	if (!available)
	{
		return;
	}

	// Plays the soundtrack, looping (-1)
	if (Mix_PlayMusic(music, -1) < 0)
	{
		std::cerr << "Mix_PlayMusic failed: " << Mix_GetError() << std::endl;
	}
}

bool Sound::isPlaying() const
{
	return available && Mix_PlayingMusic();
}
