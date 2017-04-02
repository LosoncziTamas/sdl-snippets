#ifndef atlas_h
#define atlas_h

#include <SDL2/SDL.h>

#include "jsmn.h"

struct Atlas
{
    int num_of_frames;
    char *file_name;
    SDL_Rect *frames;
};

Atlas *create_atlas_from_json(char *json);

void destroy_atlas(Atlas* atlas);

#endif
