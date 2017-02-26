#include "game.h"

extern "C" void game_update(offscreen_buffer *buffer)
{
    uint8_t *row = (uint8_t *)buffer->bitmap_memory;
    for(int j = 0; j < buffer->height; ++j)
    {
        uint32_t *pixel = (uint32_t *)row;
        for(int i = 0; i < buffer->width; ++i)
        {
            uint8_t blue = i;
            uint8_t green = j;
            
            *pixel++ = ((green << 8) | blue);
            
        }
        row += buffer->width * buffer->bytes_per_pixel;
    }
}
