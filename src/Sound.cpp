#include "Sound.h"

Sound::Sound()
{
	// Initializes sound mixer, with arguments:
	// frequency,
	// sound format,
	// channels,
	// and sample rate.
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);

	// Loads track into memory
	music = Mix_LoadMUS("data/sound/music.wav");
}

Sound::~Sound()
{	
	// Terminates the audio system
	Mix_CloseAudio();
}

void Sound::play()
{
	// Plays the soundtrack, looping (-1)
	Mix_PlayMusic(music, -1);
}

bool Sound::isPlaying() const
{
	return Mix_PlayingMusic();
}
