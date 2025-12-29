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

SDL_Rect* Camera::getCamera()
{
	return &camera;
}

void Camera::centerOnTarget(int targetX, int targetY, int mapWidth, int mapHeight)
{
	// Center camera on target position
	camera.x = targetX - SCREEN_WIDTH / 2;
	camera.y = targetY - SCREEN_HEIGHT / 2;
	
	// Clamp camera to map boundaries
	if (camera.x < 0)
	{
		camera.x = 0;
	}
	if (camera.y < 0)
	{
		camera.y = 0;
	}
	if (camera.x > mapWidth - SCREEN_WIDTH)
	{
		camera.x = mapWidth - SCREEN_WIDTH;
	}
	if (camera.y > mapHeight - SCREEN_HEIGHT)
	{
		camera.y = mapHeight - SCREEN_HEIGHT;
	}
}
