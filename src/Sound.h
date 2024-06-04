#ifndef _SOUND_H_
#define _SOUND_H_

#include <SDL_mixer/SDL_mixer.h>

class Sound
{
public:
	Sound();
	~Sound();
	
	void play();
	bool isPlaying() const;
private:    
	// Our music structure
	Mix_Music* music;
};

#endif
