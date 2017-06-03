#include "desktop.h"
#include "platform.h"

#define TARGET_UPDATE_HZ 30

SDL_bool SDL_eval(bool expr)
{
    return expr ? SDL_TRUE : SDL_FALSE;
}

static SDL_bool handle_event(SDL_Event *event)
{
    switch (event->type)
    {
        case SDL_QUIT:
        {
            return SDL_FALSE;
        }
        case SDL_WINDOWEVENT:
        {
            switch(event->window.event)
            {
                case SDL_WINDOWEVENT_EXPOSED:
                {
                    printf("Redraw/n");
                } break;
            }
        } break;
    }
    
    return SDL_TRUE;
}

static float seconds_elapsed(Uint64 old, Uint64 current)
{
    return ((float)(current - old) / (float)(SDL_GetPerformanceFrequency()));
}

int main(void)
{
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_Window *window = SDL_CreateWindow("Platform",
                                          SDL_WINDOWPOS_CENTERED_MASK,
                                          SDL_WINDOWPOS_CENTERED_MASK,
                                          SCREEN_WIDTH,
                                          SCREEN_HEIGHT,
                                          SDL_WINDOW_RESIZABLE);
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window,
                                                -1,
                                                SDL_RENDERER_PRESENTVSYNC);
    
    SDL_bool running = SDL_TRUE;
    
    float target_seconds_per_frame = 1.0f / (float) TARGET_UPDATE_HZ;
    Uint64 start_counter = SDL_GetPerformanceCounter();
    Uint64 perf_count_freq = SDL_GetPerformanceFrequency();
    
    Game_Controller_Input new_input;
    Game_State game_state = {};
    
    while(running)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            running = handle_event(&event);
        }
        
        new_input = {};
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        new_input.left = SDL_eval(state[SDL_SCANCODE_LEFT]);
        new_input.down = SDL_eval(state[SDL_SCANCODE_DOWN]);
        new_input.up = SDL_eval(state[SDL_SCANCODE_UP]);
        new_input.right = SDL_eval(state[SDL_SCANCODE_RIGHT]);
        
        SDL_GetMouseState(&new_input.mouse_x, &new_input.mouse_y);
        
        //TODO: measure SDL_delay
        while (seconds_elapsed(start_counter, SDL_GetPerformanceCounter()) < target_seconds_per_frame);
        Uint64 end_counter = SDL_GetPerformanceCounter();
        
        game_update(target_seconds_per_frame, &new_input, renderer, &game_state);
        
        double delta = ((double)(end_counter - start_counter) / (double)perf_count_freq);
        start_counter = end_counter;
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}