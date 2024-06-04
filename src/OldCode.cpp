/**
 * Old code
 */

//main

/*	if (!initialize())
		exit(1);*/
	// This function did not work... why? window closes
	// The pointer was passed by value, so the actual pointer never got changed.
	// changed fn. to pass by reference.
	//setupScreen(screen);

/*	// Video display set up
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

	// Adds a title to the program window
	SDL_WM_SetCaption("A SDL program", NULL);
*/

	// Finally found out the source of my seg. fault! We need a buffer before
	// we can use displayformat to convert the image to our display!!!
	// So place everything that uses it (like our classes) after setting up
	// the screen with SDL_SetVideoMode()


	// Blits all the surfaces onto the screen.
/*	applySurface(0, 0, background, screen);
	applySurface(320, 0, background, screen);
	applySurface(0, 240, background, screen);
	applySurface(320, 240, background, screen);
	applySurface(120, 60, character_image, screen);

	// Updates the screen
	SDL_Flip(screen);

	SDL_Delay(2000);*/

/*
	// Fills the screen with tile number 3
	for (int i = 0; i < 480; i += 32)
		for (int j = 0; j < 640; j += 32)
			applySurface(j, i, tileset, screen, &tile[2]);
*/

/*		tileoffset->x = (rand() % (TILESET_SIZE - TILESIZE + 1)) / TILESIZE * TILESIZE;
		tileoffset->y = (rand() % (TILESET_SIZE - TILESIZE + 1)) / TILESIZE * TILESIZE;
	
		int randx = (rand() % (map.getWidth() - TILESIZE + 1)) / TILESIZE * TILESIZE;
		int randy = (rand() % (map.getHeight() - TILESIZE + 1)) / TILESIZE * TILESIZE;
	
		map.applyTile(tile, randx, randy);*/

/*
	// Updates the screen
	SDL_Flip(screen);
			*/

/*
	applySurface(10, 10, message, screen);

	SDL_Flip(screen);

*/
//	int direction = SOUTH;
/*

bool hasCollided(SDL_Rect &A, SDL_Rect &B)
{
	// We assume we have a collision
	bool collision = true;

	int topA, topB;
	int bottomA, bottomB;
	int rightA, rightB;
	int leftA, leftB;

	// Determines the position of each side of our squares

	topA = A.y;
	bottomA = A.y + A.h;
	rightA = A.x + A.w;
	leftA = A.x;

	topB = B.y;
	bottomB = B.y + B.h;
	rightB = B.x + B.w;
	leftB = B.x;

	// Check if there was not any collision

	if (topA >= bottomB)
		collision = false;
	
	if (bottomA <= topB)
		collision = false;

	if (rightA <= leftB)
		collision = false;

	if (leftA >= rightB)
		collision = false; 

	// Returns the final state of our boolean variable.

	return collision;
}

bool hasCollided(Circle A, Circle B)
{
	   //
}

bool hasCollided(SDL_Rect &A, SDL_Rect &B);
bool hasCollided(Circle A, Circle B);



// This function does not let us manipulate the coordinates so as to center
// the message or place on the corner, etc
void displayMessage(SDL_Surface* surface, const string &msg, int x, int y, TTF_Font* font);

void displayMessage(SDL_Surface* surface, const string &msg, int x, int y)//, TTF_Font* font)
{
/*	if (font == NULL)
	{
		TTF_Font defaultFont = TTF_OpenFont(DEFAULT_FONT, FONTSIZE);
		// check error??
	}*//*
	TTF_Font* font = NULL;
	
	font = TTF_OpenFont(DEFAULT_FONT.c_str(), FONTSIZE); 
	
	SDL_Surface* message = NULL;

	SDL_Color txtcolor = { 0, 0, 0 }; // black

	SDL_Rect offset;
	offset.x = x;
	offset.y = y;

	message = TTF_RenderText_Solid(font, msg.c_str(), txtcolor);

	SDL_BlitSurface(message, NULL, surface, &offset);

	SDL_FreeSurface(message);

	TTF_CloseFont(font);
}
*/
