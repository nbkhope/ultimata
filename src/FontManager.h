#ifndef _FONT_H_
#define _FONT_H_

#include <string>
#include <map>

#include <SDL_ttf/SDL_ttf.h>

using namespace std;

class FontManager
{
public:
	FontManager();
	~FontManager();

//	string load(string name);
private:

	struct Font
	{
		string name;
		TTF_Font* font;
	};

	string directory;
	map <string, Font> fonts;
};

#endif
