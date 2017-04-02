#ifndef sprite_h
#define sprite_h

#include <SDL2/SDL.h>

struct Sprite
{
    int start;
    int end;
    int num_of_sprites;
    int current_frame;
    SDL_bool animated;
    float time;
    float update_freq;
};

Sprite *create_sprite(int start, int end, SDL_bool animated);

void update_sprite(Sprite *sprite, float delta);

#endif
