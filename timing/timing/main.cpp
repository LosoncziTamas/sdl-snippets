#include <SDL2/SDL.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define TARGET_UPDATE_HZ 30

int get_window_refresh_rate(SDL_Window *window)
{
    SDL_DisplayMode mode;
    int display_index = SDL_GetWindowDisplayIndex(window);
    if (SDL_GetDesktopDisplayMode(display_index, &mode) != 0)
    {
        return 0;
    }
    return mode.refresh_rate;
}

float seconds_elapsed(Uint64 old, Uint64 current)
{
    return ((float)(current - old) / (float)(SDL_GetPerformanceFrequency()));
}

int main(void) {
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_Window *window = SDL_CreateWindow("Timing",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH,
                                          SCREEN_HEIGHT,
                                          SDL_WINDOW_RESIZABLE);
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window,
                                                -1,
                                                SDL_RENDERER_PRESENTVSYNC);
    
    SDL_bool running = SDL_TRUE;
    
    int refresh_rate = get_window_refresh_rate(window);
    float target_seconds_per_frame = 1.0f / (float) TARGET_UPDATE_HZ;
    Uint64 perf_count_freq = SDL_GetPerformanceFrequency();
    Uint64 start_counter = SDL_GetPerformanceCounter();
    
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
        
        while (seconds_elapsed(start_counter, SDL_GetPerformanceCounter()) < target_seconds_per_frame)
        {
            //busy waiting
        }
        
        Uint64 end_counter = SDL_GetPerformanceCounter();
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
        
        Uint64 counter_elapsed = end_counter - start_counter;
        double fps = (double)perf_count_freq / (double)counter_elapsed;
        double ms_per_frame = (((1000.0f * (double)counter_elapsed) / (double)perf_count_freq));
        printf("%.02ff/s, %.02fms/f monitor refresh rate: %d \n", fps, ms_per_frame, refresh_rate);

        start_counter = end_counter;
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
