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

static SDL_bool jumping = SDL_FALSE;
static SDL_bool walking = SDL_FALSE;

static float gravity_x = 0;
static float gravity_y = 100;

struct Player
{
    float x;
    float y;
    float vel_x;
    float vel_y;
};

static Player player = {50, 430, 0, 0};

void game_update(float delta, Game_Controller_Input *input, SDL_Renderer *renderer)
{
    Player test = player;
    if (input->up && !jumping)
    {
        jumping = SDL_TRUE;
        test.vel_y = -100;
    }
    if (input->down)
    {
        walking = SDL_TRUE;
        test.vel_y += delta * VELOCITY;
    }
    if (input->left)
    {
        walking = SDL_TRUE;
        test.vel_x -= delta * VELOCITY;
    }
    if (input->right)
    {
        walking = SDL_TRUE;
        test.vel_x += delta * VELOCITY;
    }
    
    if (jumping)
    {
        test.vel_x += delta * gravity_x;
        test.vel_y += delta * gravity_y;
    }
    
    if (jumping || walking)
    {
        test.x += delta * test.vel_x;
        test.y += delta * test.vel_y;
    }
    
    if (test.x < 0 || test.x + 50 > SCREEN_WIDTH ||
        test.y < 0 || test.y + 50 > SCREEN_HEIGHT) {
        jumping = SDL_FALSE;
    } else {
        player = test;
    }
    
    SDL_Rect player_rect = {(int)player.x, (int)player.y, 50, 50};
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &player_rect);
    
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
