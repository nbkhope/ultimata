#include "EquipmentOverlay.h"
#include "Constants.h"
#include <iostream>

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

EquipmentOverlay::EquipmentOverlay()
{
    visible = false;
    width = 300;
    height = 400;
    // Position in top-right corner with some padding
    x = SCREEN_WIDTH - width - 20;
    y = 20;
}

EquipmentOverlay::~EquipmentOverlay()
{
}

void EquipmentOverlay::toggle()
{
    visible = !visible;
    std::cout << "Equipment overlay " << (visible ? "shown" : "hidden") << std::endl;
}

bool EquipmentOverlay::isVisible() const
{
    return visible;
}

void EquipmentOverlay::renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y, SDL_Color color)
{
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

void EquipmentOverlay::render(SDL_Renderer* renderer, TTF_Font* font, const Equipment& equipment)
{
    if (!visible || !renderer || !font)
        return;

    // Update position based on current screen size
    x = SCREEN_WIDTH - width - 20;

    // Draw semi-transparent background
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 220);
    SDL_Rect bgRect = { x, y, width, height };
    SDL_RenderFillRect(renderer, &bgRect);

    // Draw border
    SDL_SetRenderDrawColor(renderer, 180, 180, 100, 255);
    SDL_RenderDrawRect(renderer, &bgRect);

    // Render title
    SDL_Color titleColor = { 255, 220, 100, 255 };
    SDL_Color labelColor = { 200, 200, 200, 255 };
    SDL_Color valueColor = { 150, 255, 150, 255 };

    int lineHeight = 20;
    int currentY = y + 15;

    renderText(renderer, font, "=== EQUIPMENT ===", x + 50, currentY, titleColor);
    currentY += lineHeight * 2;

    // Render equipment slots
    renderText(renderer, font, "Helmet:", x + 15, currentY, labelColor);
    renderText(renderer, font, equipment.getHelmet(), x + 120, currentY, valueColor);
    currentY += lineHeight + 10;

    renderText(renderer, font, "Armor:", x + 15, currentY, labelColor);
    renderText(renderer, font, equipment.getArmor(), x + 120, currentY, valueColor);
    currentY += lineHeight + 10;

    renderText(renderer, font, "Weapon:", x + 15, currentY, labelColor);
    renderText(renderer, font, equipment.getWeapon(), x + 120, currentY, valueColor);
    currentY += lineHeight + 10;

    renderText(renderer, font, "Shield:", x + 15, currentY, labelColor);
    renderText(renderer, font, equipment.getShield(), x + 120, currentY, valueColor);
    currentY += lineHeight + 10;

    renderText(renderer, font, "Boots:", x + 15, currentY, labelColor);
    renderText(renderer, font, equipment.getBoots(), x + 120, currentY, valueColor);
    currentY += lineHeight + 10;

    renderText(renderer, font, "Ring:", x + 15, currentY, labelColor);
    renderText(renderer, font, equipment.getRing(), x + 120, currentY, valueColor);
    currentY += lineHeight + 10;

    renderText(renderer, font, "Amulet:", x + 15, currentY, labelColor);
    renderText(renderer, font, equipment.getAmulet(), x + 120, currentY, valueColor);
    currentY += lineHeight + 10;

    // Draw footer text
    SDL_Color footerColor = { 150, 150, 150, 255 };
    renderText(renderer, font, "Press 'E' to close", x + 60, y + height - 30, footerColor);
}
