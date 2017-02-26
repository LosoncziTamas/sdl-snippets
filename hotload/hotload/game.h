#ifndef game_h
#define game_h

#include <SDL2/SDL.h>

struct offscreen_buffer
{
    SDL_Texture *texture;
    void        *bitmap_memory;
    int         width;
    int         height;
    int         bytes_per_pixel;
};

extern "C" void game_update(offscreen_buffer *buffer);

#endif
