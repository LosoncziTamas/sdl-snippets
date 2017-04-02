#include "io.h"

char *read_file(const char* file_name)
{
    char *file_content = NULL;
    SDL_RWops *file = SDL_RWFromFile(file_name, "r");
    if (file)
    {
        Sint64 file_size = SDL_RWsize(file);
        file_content = (char*) malloc(file_size + 1);
        if(SDL_RWread(file, file_content, sizeof(char), file_size))
        {
            file_content[file_size] = '\0';
        }
        else
        {
            printf( "Error: Unable to read file! SDL Error: %s\n", SDL_GetError());
            free(file_content);
            file_content = NULL;
        }
        SDL_RWclose(file);
    }
    else
    {
        printf( "Error: Unable to open file! SDL Error: %s\n", SDL_GetError());
    }
    return file_content;
}

SDL_Texture *load_image(const char *path, SDL_Renderer *renderer)
{
    SDL_Surface *surface = IMG_Load(path);
    
    if (surface == NULL)
    {
        printf( "IMG_Load failed! SDL_image Error: %s\n", IMG_GetError() );
        exit(1);
    }
    
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    
    if (texture == NULL)
    {
        printf( "CreateTextureFromSurface failed! SDL_image Error: %s\n", IMG_GetError() );
        exit(1);
    }
    
    SDL_FreeSurface(surface);
    
    return texture;
}
