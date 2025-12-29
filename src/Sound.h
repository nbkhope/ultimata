#ifndef _SOUND_H_
#define _SOUND_H_

#include <SDL2/SDL_mixer.h>

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
