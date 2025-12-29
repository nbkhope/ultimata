#include "ChatOverlay.h"
#include "Constants.h"
#include <iostream>

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

ChatOverlay::ChatOverlay()
{
    visible = false;
    active = false;
    inputText = "";
    width = 500;
    height = 50;
    // Position at bottom center
    x = (SCREEN_WIDTH - width) / 2;
    y = SCREEN_HEIGHT - height - 60;
}

ChatOverlay::~ChatOverlay()
{
}

void ChatOverlay::toggle()
{
    visible = !visible;
    active = visible;
    if (visible)
    {
        inputText.clear();
        SDL_StartTextInput();
        std::cout << "Chat overlay opened" << std::endl;
    }
    else
    {
        SDL_StopTextInput();
        std::cout << "Chat overlay closed" << std::endl;
    }
}

bool ChatOverlay::isVisible() const
{
    return visible;
}

void ChatOverlay::setActive(bool active)
{
    this->active = active;
    if (active)
    {
        SDL_StartTextInput();
    }
    else
    {
        SDL_StopTextInput();
    }
}

bool ChatOverlay::isActive() const
{
    return active;
}

void ChatOverlay::appendChar(char c)
{
    if (inputText.length() < 100) // Max message length
    {
        inputText += c;
    }
}

void ChatOverlay::backspace()
{
    if (!inputText.empty())
    {
        inputText.pop_back();
    }
}

void ChatOverlay::clear()
{
    inputText.clear();
}

std::string ChatOverlay::getMessage() const
{
    return inputText;
}

void ChatOverlay::renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y, SDL_Color color)
{
    if (text.empty())
        return;
        
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    if (surface)
    {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (texture)
        {
            SDL_Rect dstRect = { x, y, surface->w, surface->h };
            SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
            SDL_DestroyTexture(texture);
        }
        SDL_FreeSurface(surface);
    }
}

void ChatOverlay::render(SDL_Renderer* renderer, TTF_Font* font)
{
    if (!visible || !renderer || !font)
        return;

    // Update position based on current screen size
    x = (SCREEN_WIDTH - width) / 2;
    y = SCREEN_HEIGHT - height - 60;

    // Draw semi-transparent background
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 230);
    SDL_Rect bgRect = { x, y, width, height };
    SDL_RenderFillRect(renderer, &bgRect);

    // Draw border
    SDL_SetRenderDrawColor(renderer, 100, 200, 255, 255);
    SDL_RenderDrawRect(renderer, &bgRect);

    // Render label
    SDL_Color labelColor = { 150, 150, 150, 255 };
    renderText(renderer, font, "Chat:", x + 10, y + 15, labelColor);

    // Render input text
    SDL_Color textColor = { 255, 255, 255, 255 };
    std::string displayText = inputText;
    
    // Add blinking cursor if active
    if (active)
    {
        Uint32 ticks = SDL_GetTicks();
        if ((ticks / 500) % 2 == 0) // Blink every 500ms
        {
            displayText += "|";
        }
    }
    
    renderText(renderer, font, displayText, x + 60, y + 15, textColor);
    
    // Render hint text
    SDL_Color hintColor = { 100, 100, 100, 255 };
    renderText(renderer, font, "Press ENTER to send, ESC to cancel", x + 10, y + height + 5, hintColor);
}
