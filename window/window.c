#include <SDL2/SDL.h>
#include <stdbool.h>

int main(void)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
    
    if (window)
    {
        for (bool running = true; running;)
        {   
            for(SDL_Event event; SDL_PollEvent(&event);)
            {
                switch(event.type)
                {
                    case SDL_QUIT:
                    {
                        running = false;
                    }
                }
            }
        }
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
    return 0;
}