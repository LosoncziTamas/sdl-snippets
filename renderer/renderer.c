#include <SDL2/SDL.h>
#include <stdbool.h>

int main(void)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window;
    SDL_Renderer* renderer;

    if (SDL_CreateWindowAndRenderer(640, 480, SDL_WINDOW_ALWAYS_ON_TOP, &window, &renderer) == 0)
    {
        bool running = true;
        SDL_Rect rect = {.x = 10, .y = 10, .w = 20, .h = 20};

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        while(running)
        {
            for(SDL_Event event; SDL_PollEvent(&event);)
            {
                switch (event.type)
                {
                    case SDL_KEYUP:
                    {
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
                        SDL_RenderDrawRect(renderer, &rect);
                    } break;
                    case SDL_QUIT:
                    {
                        running = false;
                    } break;
                }
            }
            SDL_RenderPresent(renderer);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
