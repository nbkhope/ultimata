#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <SDL2/SDL.h>

#include "constants.h"

class Camera
{
public:
	Camera();
	~Camera();

	SDL_Rect* getCamera();
private:
	SDL_Rect camera;
};

#endif
