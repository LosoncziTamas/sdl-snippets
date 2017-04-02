#include "sprite.h"

#define DEFAULT_UPDATE_FREQ 0.2f

Sprite *create_sprite(int start, int end, SDL_bool animated)
{
    SDL_assert(start <= end);
    
    Sprite *sprite = (Sprite*) malloc(sizeof(Sprite));
    
    sprite->start = start;
    sprite->end = end;
    sprite->num_of_sprites = end - start + 1;
    sprite->time = 0.0f;
    sprite->animated = animated;
    sprite->update_freq = DEFAULT_UPDATE_FREQ;
    sprite->current_frame = 0;
    
    return sprite;
}

void update_sprite(Sprite *sprite, float delta)
{
    if(sprite->animated)
    {
        sprite->time += delta;
        if (sprite->time > sprite->update_freq)
        {
            sprite->current_frame++;
            if (sprite->current_frame == sprite->num_of_sprites)
            {
                sprite->current_frame = 0;
            }
            sprite->time = 0;
        }
    }
}
