#ifndef __CHAT_MESSAGE_H__
#define __CHAT_MESSAGE_H__

#include <string>
#include "SDL2/SDL.h"

/**
 * Structure to hold chat message data
 */
struct ChatMessage
{
    std::string text;
    Uint32 timestamp;
    int duration; // in milliseconds
    
    ChatMessage() : text(""), timestamp(0), duration(5000) {}
    
    ChatMessage(const std::string& msg, int dur = 5000) 
        : text(msg), timestamp(SDL_GetTicks()), duration(dur) {}
    
    bool isExpired() const
    {
        return (SDL_GetTicks() - timestamp) > duration;
    }
};

#endif
