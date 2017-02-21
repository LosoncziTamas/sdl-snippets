//
//  main.cpp
//  parallax
//
//  Created by Losonczi Tamás on 2/5/17.
//  Copyright © 2017 Losonczi Tamás. All rights reserved.
//

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <iostream>

static bool running;

SDL_Surface *Parallax_LoadImage(char *path, SDL_Surface *surface) {
    SDL_Surface *optimizedSurface = NULL;
    SDL_Surface *loadedSurface = IMG_Load(path);
    
    if (loadedSurface == NULL)
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
    }
    else
    {
        optimizedSurface = SDL_ConvertSurface(loadedSurface, surface->format, NULL);
        if( optimizedSurface == NULL )
        {
            printf( "Unable to optimize image %s! SDL Error: %s\n", path, SDL_GetError());
        }
        
        SDL_FreeSurface(loadedSurface);
    }

    return optimizedSurface;
}

int main(void)
{

    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_Window *window = SDL_CreateWindow("Parallax",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          640,
                                          480,
                                          SDL_WINDOW_RESIZABLE);
    

    SDL_Surface *windowSurface = NULL;
    int imgFlags = IMG_INIT_PNG;
    if((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        windowSurface = SDL_GetWindowSurface(window);
    
    }
    else
    {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
    }
    
    SDL_Surface *background_1 =Parallax_LoadImage("parallax_1.png", windowSurface);
    SDL_Surface *background_2 = Parallax_LoadImage("parallax_2.png", windowSurface);
    SDL_Surface *background_3 = Parallax_LoadImage("parallax_3.png", windowSurface);

    
    SDL_Rect dst_rect_1;
    dst_rect_1.x = 0;
    dst_rect_1.y = 0;
    dst_rect_1.w = 0;
    dst_rect_1.h = 480;
    
    SDL_Rect dst_rect_2;
    dst_rect_2.x = 0;
    dst_rect_2.y = 0;
    dst_rect_2.w = 640;
    dst_rect_2.h = 480;
    
    SDL_Rect src_rect_1;
    src_rect_1.x = 640;
    src_rect_1.y = 0;
    src_rect_1.w = 0;
    src_rect_1.h = 480;
    
    SDL_Rect src_rect_2;
    src_rect_2.x = 0;
    src_rect_2.y = 0;
    src_rect_2.w = 640;
    src_rect_2.h = 480;
    
    SDL_Rect foreground_dst_rect_1;
    foreground_dst_rect_1.x = 0;
    foreground_dst_rect_1.y = 0;
    foreground_dst_rect_1.w = 0;
    foreground_dst_rect_1.h = 480;
    
    SDL_Rect foreground_dst_rect_2;
    foreground_dst_rect_2.x = 0;
    foreground_dst_rect_2.y = 0;
    foreground_dst_rect_2.w = 640;
    foreground_dst_rect_2.h = 480;
    
    SDL_Rect foreground_src_rect_1;
    foreground_src_rect_1.x = 640;
    foreground_src_rect_1.y = 0;
    foreground_src_rect_1.w = 0;
    foreground_src_rect_1.h = 480;
    
    SDL_Rect foreground_src_rect_2;
    foreground_src_rect_2.x = 0;
    foreground_src_rect_2.y = 0;
    foreground_src_rect_2.w = 640;
    foreground_src_rect_2.h = 480;
    
    running = true;
    
    Sint32 focal = 0;
    SDL_bool down = SDL_FALSE;
    
    while (running)
    {
    
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
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
            focal += 1;
        }
        
        focal = focal % 640;
        
        //TODO: fix jumping
        Uint32 background_speed = (focal / 2 ) % 640;
        
        dst_rect_1.w = background_speed;
        
        dst_rect_2.x = background_speed;
        dst_rect_2.w = 640 - background_speed;

        src_rect_1.x = 640 - background_speed;
        src_rect_1.w = background_speed;
        
        src_rect_2.w = 640 - background_speed;
        
        foreground_dst_rect_1.w = focal;

        foreground_dst_rect_2.x = focal;
        foreground_dst_rect_2.w = 640 - focal;
        
        foreground_src_rect_1.x = 640 - focal;
        foreground_src_rect_1.w = focal;
        
        foreground_src_rect_2.w = 640 - focal;
        
        SDL_BlitSurface(background_1, NULL, windowSurface, NULL);
        
        SDL_BlitSurface(background_2, &src_rect_1, windowSurface, &dst_rect_1);
        SDL_BlitSurface(background_2, &src_rect_2, windowSurface, &dst_rect_2);
        
        SDL_BlitSurface(background_3, &foreground_src_rect_1, windowSurface, &foreground_dst_rect_1);
        SDL_BlitSurface(background_3, &foreground_src_rect_2, windowSurface, &foreground_dst_rect_2);

        SDL_UpdateWindowSurface(window);
        
        SDL_Delay(5);
    }
    
    IMG_Quit();
    SDL_Quit();
    return 0;
}
