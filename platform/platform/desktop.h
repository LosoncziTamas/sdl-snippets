#ifndef desktop_h
#define desktop_h

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 540

#include <SDL2/SDL.h>

struct Game_Controller_Input
{
    union
    {
        SDL_bool buttons[4];
        struct
        {
            SDL_bool up;
            SDL_bool down;
            SDL_bool left;
            SDL_bool right;
        };
    };
    int32_t mouse_x;
    int32_t mouse_y;
};

#endif