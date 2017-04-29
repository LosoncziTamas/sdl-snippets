#include <SDL2/SDL.h>

#include "platform.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define TARGET_UPDATE_HZ 30

static SDL_bool SDL_eval(bool expr)
{
    return expr ? SDL_TRUE : SDL_FALSE;
}

struct Game_Controller_Input
{
    union
    {
        SDL_bool buttons[4];
        struct
        {
            SDL_bool up;
            SDL_bool down;
            SDL_bool left;
            SDL_bool right;
        };
    };
};

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

#define VELOCITY 100

static SDL_Rect rectangle = {0, 0, 50, 50};

void game_update(float delta, Game_Controller_Input *input, SDL_Renderer *renderer)
{
    SDL_Rect test = rectangle;
    
    if (input->up) {
        test.y -= delta * VELOCITY;
    }
    if (input->down) {
        test.y += delta * VELOCITY;
    }
    if (input->left) {
        test.x -= delta * VELOCITY;
    }
    if (input->right) {
        test.x += delta * VELOCITY;
    }
    
    if (test.x < 0 || test.x + test.w > SCREEN_WIDTH ||
        test.y < 0 || test.y + test.h > SCREEN_HEIGHT) {
        printf("Collision\n");
    } else {
        rectangle = test;
    }
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rectangle);
    
    SDL_RenderPresent(renderer);
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
        
        //TODO: measure SDL_delay
        while (seconds_elapsed(start_counter, SDL_GetPerformanceCounter()) < target_seconds_per_frame);
        Uint64 end_counter = SDL_GetPerformanceCounter();
        
        //pass renderer?
        game_update(target_seconds_per_frame, &new_input, renderer);
        
        double delta = ((double)(end_counter - start_counter) / (double)perf_count_freq);
        start_counter = end_counter;
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
