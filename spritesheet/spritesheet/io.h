#ifndef io_h
#define io_h

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

char *read_file(const char* file_name);

SDL_Texture *load_image(const char *path, SDL_Renderer *renderer);

#endif
