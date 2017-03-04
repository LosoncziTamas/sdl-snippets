#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <iostream>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

static SDL_bool running;

struct Parallax {
    SDL_Rect    src_rect_1;
    SDL_Rect    src_rect_2;
    SDL_Rect    dst_rect_1;
    SDL_Rect    dst_rect_2;
    int         width;
    int         height;
};

void init_parallax(Parallax *parallax, int width, int height)
{
    parallax->width = width;
    parallax->height = height;
    
    SDL_Rect dst_rect_1 = {};
    dst_rect_1.h = height;
    parallax->dst_rect_1 = dst_rect_1;
    
    SDL_Rect dst_rect_2 = {};
    dst_rect_2.w = width;
    dst_rect_2.h = height;
    parallax->dst_rect_2 = dst_rect_2;
    
    SDL_Rect src_rect_1 = {};
    src_rect_1.x = width;
    src_rect_1.h = height;
    parallax->src_rect_1 = src_rect_1;
    
    SDL_Rect src_rect_2 = {};
    src_rect_2.w = width;
    src_rect_2.h = height;
    parallax->src_rect_2 = src_rect_2;
}

void clip_horizontal_parallax(int start_pos, Parallax *parallax)
{
    parallax->dst_rect_1.w = start_pos;
    parallax->dst_rect_2.x = start_pos;
    parallax->dst_rect_2.w = parallax->width - start_pos;
    parallax->src_rect_1.x = parallax->width - start_pos;
    parallax->src_rect_1.w = start_pos;
    parallax->src_rect_2.w = parallax->width - start_pos;
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

int main(void)
{

    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_Window *window = SDL_CreateWindow("Parallax",
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
    
    SDL_Texture *background_texture_1 = load_image("parallax_1.png", renderer);
    SDL_Texture *background_texture_2 = load_image("parallax_2.png", renderer);
    SDL_Texture *foreground_texture = load_image("parallax_3.png", renderer);

    Parallax background = {};
    init_parallax(&background, SCREEN_WIDTH, SCREEN_HEIGHT);
    Parallax foreground = {};
    init_parallax(&foreground, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    running = SDL_TRUE;
    SDL_bool down = SDL_FALSE;
    int foreground_pos = 0;
    int background_pos = 0;
    
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
                case SDL_KEYDOWN:
                {
                    down = SDL_TRUE;
                } break;
                case SDL_KEYUP:
                {
                    down = SDL_FALSE;
                } break;
            }
        }
        
        if (down)
        {
            foreground_pos += 1;
            foreground_pos %=  SCREEN_WIDTH;
            
            background_pos += 2;
            background_pos %= SCREEN_WIDTH;
            
            clip_horizontal_parallax(background_pos, &background);
            clip_horizontal_parallax(foreground_pos, &foreground);
        }
        
        SDL_RenderCopy(renderer, background_texture_1, NULL, NULL);

        SDL_RenderCopy(renderer, background_texture_2, &background.src_rect_1, &background.dst_rect_1);
        SDL_RenderCopy(renderer, background_texture_2, &background.src_rect_2, &background.dst_rect_2);
        
        SDL_RenderCopy(renderer, foreground_texture, &foreground.src_rect_1, &foreground.dst_rect_1);
        SDL_RenderCopy(renderer, foreground_texture, &foreground.src_rect_2, &foreground.dst_rect_2);
        
        SDL_RenderPresent(renderer);
    }
    
    SDL_DestroyTexture(background_texture_1);
    SDL_DestroyTexture(background_texture_2);
    SDL_DestroyTexture(foreground_texture);
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    IMG_Quit();
    SDL_Quit();
    
    return 0;
}
