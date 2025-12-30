#ifndef __CHAT_OVERLAY_H__
#define __CHAT_OVERLAY_H__

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include <string>

/**
 * ChatOverlay class to handle chat input
 */
class ChatOverlay
{
public:
    ChatOverlay();
    ~ChatOverlay();

    void render(SDL_Renderer* renderer, TTF_Font* font);
    void toggle();
    bool isVisible() const;
    
    void appendChar(char c);
    void backspace();
    void clear();
    std::string getMessage() const;
    
    void setActive(bool active);
    bool isActive() const;

private:
    bool visible;
    bool active;
    std::string inputText;
    int x, y;
    int width, height;
    
    void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y, SDL_Color color);
};

#endif
