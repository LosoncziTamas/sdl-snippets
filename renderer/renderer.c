#include <SDL2/SDL.h>
#include <stdbool.h>

char* ReadTextFile(const char* fileName) 
{
    SDL_RWops *io = SDL_RWFromFile(fileName, "rb");
    if (io) 
    {
        Sint64 size = SDL_RWsize(io);
        if (size > -1) 
        {
            char *strMem = SDL_malloc(size + 1);
            SDL_RWread(io, strMem, size, 1);
            // Null terminate allocated memory
            strMem[size] = '\0';
            SDL_RWclose(io);
            return strMem;
        }
    }
    return NULL;
}

int main(void)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window;
    SDL_Renderer* renderer;

    if (SDL_CreateWindowAndRenderer(640, 480, SDL_WINDOW_ALWAYS_ON_TOP, &window, &renderer) == 0)
    {
        bool running = true;
        SDL_Rect screenRect = {.x = 0, .y = 0, .w = 640, .h = 480};

        SDL_SetRenderDrawColor(renderer, 64, 0, 128, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        SDL_Texture* screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, 640, 480);
        /*SDL_SetRenderTarget(renderer, screen);

        unsigned char pixels[640 * 480 * 4] = {12};

        SDL_UpdateTexture(screen, &screenRect, pixels, 4 * 640);
        SDL_RenderCopy(renderer, screen, NULL, NULL);*/
        
        SDL_RenderPresent(renderer);
        while(running)
        {
            for(SDL_Event event; SDL_PollEvent(&event);)
            {
                switch (event.type)
                {
                    case SDL_QUIT:
                    {
                        running = false;
                    } break;
                }
            }
        }

        SDL_DestroyTexture(screen);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
