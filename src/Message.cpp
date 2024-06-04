#include "Message.h"

Message::Message()
{
	font = TTF_OpenFont(DEFAULT_FONT.c_str(), FONTSIZE);	
	color.r = 0; // black
	color.g = 0;
	color.b = 0;
	textsurface = NULL;
}

Message::~Message()
{
	TTF_CloseFont(font);
	SDL_FreeSurface(textsurface);
}

void Message::displayLine(int x, int y, string msg, Graphics& graphics)
{
		textsurface = TTF_RenderText_Solid(font, msg.c_str(), color);
		graphics.drawToBuffer(x, y, textsurface);
}

// not good
void Message::display(stringstream& msg, SDL_Surface* surface)
{
	string sentence;

	int lines = 0;

	for (int i = 0; i < msg.str().length(); i++)
		if (msg.str()[i] == '\n')
			lines++;

	for (int i = lines; i > 0; i--)
	{
		getline(msg, sentence);
		textsurface = TTF_RenderText_Solid(font, sentence.c_str(), color);
		//add pos capability
		applySurface(4, SCREEN_HEIGHT - i * textsurface->h - 32, textsurface, surface);
	}
}

void Message::displayTopLeft(stringstream& msg, Graphics& graphics)
{
	string str;
	int i = 0;

	while (getline(msg, str) && i < graphics.getBuffer()->h)
	{
		textsurface = TTF_RenderText_Solid(font, str.c_str(), color);
		graphics.drawToBuffer(4, i, textsurface);
		i += textsurface->h;
	}

	// Resets the pointer
	msg.clear();
}

void Message::displayBottomLeft(stringstream& msg, Graphics& graphics)
{
	string str;
	int i = 0;

	stack<string> auxStack;

	while (getline(msg, str))
		auxStack.push(str);

	while (!auxStack.empty() && i < graphics.getBuffer()->h)
	{
		str = auxStack.top();
		auxStack.pop();
		textsurface = TTF_RenderText_Solid(font, str.c_str(), color);
		graphics.drawToBuffer(4, graphics.getBuffer()->h - textsurface->h - i, textsurface);
		i += textsurface->h;
	}

	// Resets the pointer
	msg.clear();
}


void Message::displayBottomCenter(stringstream& msg, Graphics& graphics)
{
	string str;
	int i = 0;

	stack<string> auxStack;

	while (getline(msg, str))
		auxStack.push(str);

	while (!auxStack.empty() && i < graphics.getBuffer()->h)
	{
		str = auxStack.top();
		auxStack.pop();

		textsurface = TTF_RenderText_Solid(font, str.c_str(), color);
		graphics.drawToBuffer((graphics.getBuffer()->w - textsurface->w)/2, graphics.getBuffer()->h - textsurface->h - i, textsurface);
		i += textsurface->h;
	}

	// Resets the pointer
	msg.clear();
}

void Message::display(stringstream& msg, SDL_Surface* surface, int x, int y)
{
	textsurface = TTF_RenderText_Solid(font, msg.str().c_str(), color);
	applySurface(x, y, textsurface, surface);
}

