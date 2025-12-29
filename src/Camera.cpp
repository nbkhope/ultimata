#include "Camera.h"

Camera::Camera()
{
	camera.x = 0;
	camera.y = 0;
	camera.w = SCREEN_WIDTH;
	camera.h = SCREEN_HEIGHT;
}

Camera::~Camera()
{

}

SDL_Rect* getCamera()
{
	return &camera;
}


