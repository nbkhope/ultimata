#ifndef __EQUIPMENT_OVERLAY_H__
#define __EQUIPMENT_OVERLAY_H__

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "Equipment.h"
#include <string>

/**
 * EquipmentOverlay class to render the equipment UI overlay
 */
class EquipmentOverlay
{
public:
    EquipmentOverlay();
    ~EquipmentOverlay();

    void render(SDL_Renderer* renderer, TTF_Font* font, const Equipment& equipment);
    void toggle();
    bool isVisible() const;

private:
    bool visible;
    int x, y;
    int width, height;

    void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y, SDL_Color color);
};

#endif
