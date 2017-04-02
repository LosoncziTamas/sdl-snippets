#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include "io.h"
#include "atlas.h"
#include "sprite.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

float seconds_elapsed(Uint64 old, Uint64 current)
{
    return ((float)(current - old) / (float)(SDL_GetPerformanceFrequency()));
}

int main(void)
{
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_Window *window = SDL_CreateWindow("Spritesheet",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH,
                                          SCREEN_HEIGHT,
                                          SDL_WINDOW_RESIZABLE);
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window,
                                                -1,
                                                SDL_RENDERER_PRESENTVSYNC);
    
    bool success = IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG;
        
    if(!success)
    {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
    }
    
    char *json = read_file("player.json");
    
    Atlas *atlas = create_atlas_from_json(json);
    
    SDL_Texture *spritesheet = load_image(atlas->file_name, renderer);
    
    Sprite *dead = create_sprite(0, 0, SDL_FALSE);
    Sprite *shoot = create_sprite(3, 3, SDL_FALSE);
    Sprite *jump = create_sprite(4, 4, SDL_FALSE);
    Sprite *walk = create_sprite(1, 2, SDL_TRUE);
    Sprite *won = create_sprite(5, 6, SDL_TRUE);
    
    SDL_Rect dst_1 = {0, 0, 128, 128};
    SDL_Rect dst_2 = {128, 0, 128, 128};
    SDL_Rect dst_3 = {256, 0, 128, 128};
    SDL_Rect dst_4 = {384, 0, 128, 128};
    SDL_Rect dst_5 = {512, 0, 128, 128};

    Uint64 start_counter = SDL_GetPerformanceCounter();
    Uint64 perf_count_freq = SDL_GetPerformanceFrequency();
    float target_seconds_per_frame = 1.0f / 30.0f;

    SDL_bool running = SDL_TRUE;

    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                {
                    running = SDL_FALSE;
                } break;
            }
        }
        
        while (seconds_elapsed(start_counter, SDL_GetPerformanceCounter()) < target_seconds_per_frame);
        
        Uint64 end_counter = SDL_GetPerformanceCounter();
        
        float delta = (float)(end_counter - start_counter) / (float)perf_count_freq;
        
        update_sprite(walk, delta);
        update_sprite(won, delta);
        
        SDL_SetRenderDrawColor(renderer, 128, 128, 128, 0);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, spritesheet, &atlas->frames[walk->start + walk->current_frame], &dst_1);
        SDL_RenderCopy(renderer, spritesheet, &atlas->frames[won->start + won->current_frame], &dst_2);
        SDL_RenderCopy(renderer, spritesheet, &atlas->frames[dead->start], &dst_3);
        SDL_RenderCopy(renderer, spritesheet, &atlas->frames[shoot->start], &dst_4);
        SDL_RenderCopy(renderer, spritesheet, &atlas->frames[jump->start], &dst_5);
        SDL_RenderPresent(renderer);
        
        start_counter = end_counter;
    }
    
    free(json);
    destroy_atlas(atlas);
    SDL_DestroyTexture(spritesheet);
    
    free(dead);
    free(shoot);
    free(jump);
    free(walk);
    free(won);
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    
    return 0;
}
