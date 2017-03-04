#include "game.h"

extern "C" void game_update(Offscreen_Buffer *buffer)
{
    uint8_t *row = (uint8_t *)buffer->bitmap_memory;
    for(int j = 0; j < buffer->height; ++j)
    {
        uint32_t *pixel = (uint32_t *)row;
        for(int i = 0; i < buffer->width; ++i)
        {
            uint8_t blue = j;
            uint8_t green = i;
            
            *pixel++ = ((green << 16) | blue);
            
        }
        row += buffer->width * buffer->bytes_per_pixel;
    }
}
