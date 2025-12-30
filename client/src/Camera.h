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
	void centerOnTarget(int targetX, int targetY, int mapWidth, int mapHeight);
	int getX() const { return camera.x; }
	int getY() const { return camera.y; }
private:
	SDL_Rect camera;
};

#endif
