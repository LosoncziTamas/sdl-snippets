#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <iostream>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

static bool running;

typedef struct Parallax {
    SDL_Rect src_rect_1;
    SDL_Rect src_rect_2;
    SDL_Rect dst_rect_1;
    SDL_Rect dst_rect_2;
    int width;
    int height;
} Paralllax;

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

SDL_Surface *load_image(char *path, SDL_Surface *surface)
{
    SDL_Surface *optimized_surface = NULL;
    SDL_Surface *loaded_surface = IMG_Load(path);
    
    if (loaded_surface == NULL)
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
    }
    else
    {
        optimized_surface = SDL_ConvertSurface(loaded_surface, surface->format, NULL);
        if( optimized_surface == NULL )
        {
            printf( "Unable to optimize image %s! SDL Error: %s\n", path, SDL_GetError());
        }
        
        SDL_FreeSurface(loaded_surface);
    }

    return optimized_surface;
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
    SDL_Surface *windowSurface;
    
    if((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        windowSurface = SDL_GetWindowSurface(window);
    
    }
    else
    {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
    }
    
    SDL_Surface *background_1 = load_image("parallax_1.png", windowSurface);
    SDL_Surface *background_2 = load_image("parallax_2.png", windowSurface);
    SDL_Surface *background_3 = load_image("parallax_3.png", windowSurface);

    Parallax background = {};
    init_parallax(&background, SCREEN_WIDTH, SCREEN_HEIGHT);
    Parallax foreground = {};
    init_parallax(&foreground, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    running = true;
    
    int foreground_pos = 0;
    int background_pos = 0;
    SDL_bool down = SDL_FALSE;
    
    while (running)
    {
    
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                {
                    running = false;
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
            
            printf("foreground: %d background: %d\n", foreground_pos, background_pos);
            
            clip_horizontal_parallax(background_pos, &background);
            clip_horizontal_parallax(foreground_pos, &foreground);
        }
        
        SDL_BlitSurface(background_1, NULL, windowSurface, NULL);
        
        SDL_BlitSurface(background_2, &background.src_rect_1, windowSurface, &background.dst_rect_1);
        SDL_BlitSurface(background_2, &background.src_rect_2, windowSurface, &background.dst_rect_2);
        
        SDL_BlitSurface(background_3, &foreground.src_rect_1, windowSurface, &foreground.dst_rect_1);
        SDL_BlitSurface(background_3, &foreground.src_rect_2, windowSurface, &foreground.dst_rect_2);

        SDL_UpdateWindowSurface(window);
    }
    
    SDL_FreeSurface(background_1);
    SDL_FreeSurface(background_2);
    SDL_FreeSurface(background_3);
    
    IMG_Quit();
    SDL_Quit();
    
    return 0;
}
