#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>

#include "FontManager.hpp"
#include "Renderer.hpp"

int main(void) {

    SDL_Init(SDL_INIT_VIDEO);
    
    FontManager::init();
    
    SDL_Window *window = SDL_CreateWindow("ttf",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          640,
                                          480,
                                          SDL_WINDOW_RESIZABLE);
    
    Renderer renderer(*window);
    
    SDL_bool running = SDL_TRUE;
    
    Font *font = FontManager::getFont();
    Texture texture = renderer.createTexture(*font, "Hello", {0, 0, 0, 0});
    
    renderer.clear();
    renderer.drawText(100, 100, texture);
    renderer.update();
    
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
    }
    
    SDL_DestroyWindow(window);
    FontManager::destroy();
    SDL_Quit();
    
    return 0;
}

