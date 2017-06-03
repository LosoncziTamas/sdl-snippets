#include <SDL2/SDL.h>

#include "platform.h"

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 540
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
#define TILES_PER_WIDTH 10
#define TILES_PER_HEIGHT 10

struct World
{
    int *tiles;
    int count_x;
    int count_y;
    int tiles_per_width;
    int tiles_per_height;
};

struct Player
{
    int tile_index_x;
    int tile_index_y;
    float tile_rel_x;
    float tile_rel_y;
    float vel_x;
    float vel_y;
};

static int level[5][20] =
{
    
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0,
};

struct Game_State
{
    World world;
    Player player;
    SDL_bool inited;
};

void flip_rect(SDL_Rect *rect)
{
    rect->y = SCREEN_HEIGHT - rect->y - rect->h;
}

void debug_player_pos(Player *player)
{
    printf("tile_x %d tile_y %d x %f y %f \n", player->tile_index_x, player->tile_index_y, player->tile_rel_x, player->tile_rel_y);
}

int get_tile_value(int column, int row, World *world)
{
    if(column < 0 || column >= world->count_x || row < 0 || row >= world->count_y)
    {
        return -1;
    }
    return world->tiles[row * world->count_x + column];
}

void game_update(float delta, Game_Controller_Input *input, SDL_Renderer *renderer, Game_State *game_state)
{
    World *world = &game_state->world;
    Player *player = &game_state->player;
    
    int tile_width_in_pixel = 40;
    int tile_height_in_pixel = 40;
    
    if (!game_state->inited)
    {
        world->count_x = 20;
        world->count_y = 5;
        world->tiles = (int*)level;
        world->tiles_per_width = 10;
        world->tiles_per_height = 10;
        player->tile_rel_x = 0;
        player->tile_rel_y = 0;
        player->tile_index_x = 1;
        player->tile_index_y = 4;
        game_state->inited = SDL_TRUE;
        
    }
    
    Player test = *player;
    
    if (input->left)
    {
        test.tile_rel_x -= delta * VELOCITY;
    }
    if (input->right)
    {
        test.tile_rel_x += delta * VELOCITY;
    }
    if (input->up)
    {
        test.tile_rel_y += delta * VELOCITY;
    }
    if (input->down)
    {
        test.tile_rel_y -= delta * VELOCITY;
    }

    if (test.tile_rel_y < 0) {
        test.tile_index_y -= (int)(test.tile_rel_y / tile_height_in_pixel) - 1;
        test.tile_rel_y = tile_height_in_pixel - fmod(fabs(test.tile_rel_y), tile_height_in_pixel);
    } else if(test.tile_rel_y >= tile_height_in_pixel) {
        test.tile_index_y -= (int) test.tile_rel_y / tile_height_in_pixel;
        test.tile_rel_y = fmod(fabs(test.tile_rel_y), tile_height_in_pixel);
    }
    
    if (test.tile_rel_x <= 0) {
        test.tile_index_x += (int)(test.tile_rel_x / tile_width_in_pixel) - 1;
        test.tile_rel_x = tile_width_in_pixel - fmod(fabs(test.tile_rel_x), tile_width_in_pixel);
    } else if(test.tile_rel_x > tile_width_in_pixel) {
        test.tile_index_x += (int)(test.tile_rel_x / tile_width_in_pixel);
        test.tile_rel_x = fmod(fabs(test.tile_rel_x), tile_width_in_pixel);
    }
    
    debug_player_pos(player);
    
    *player = test;
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    int center_tile_x = player->tile_index_x;
    int center_tile_y = player->tile_index_y;
    
    float screen_center_x = world->tiles_per_width * (float)tile_width_in_pixel * 0.5f;
    float screen_center_y = world->tiles_per_height * (float)tile_height_in_pixel * 0.5f;
    
    int tile_range_x = world->tiles_per_width;
    int tile_range_y = world->tiles_per_height;

    for (int x = -tile_range_x; x < tile_range_x; x++)
    {
        for (int y = -tile_range_y; y < tile_range_y; y++)
        {
            SDL_Rect tile1 = {
                (int)(screen_center_x - player->tile_rel_x + x * tile_width_in_pixel),
                (int)(screen_center_y - player->tile_rel_y + y * tile_height_in_pixel),
                tile_width_in_pixel,
                tile_height_in_pixel
            };
            flip_rect(&tile1);
            int column = player->tile_index_x + x;
            int row = player->tile_index_y - y;
            int value = get_tile_value(column, row, world);
            if (value == 1)
            {
                SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
            }
            else if(value == -1)
            {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }
            
            if (row == center_tile_y && column == center_tile_x)
            {
                SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
            }
            
            SDL_RenderFillRect(renderer, &tile1);

        }
    }
    
    SDL_Rect player_rect = {
        (int)(screen_center_x),
        (int)(screen_center_y),
        10,
        10};
    flip_rect(&player_rect);
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
        
        //TODO: measure SDL_delay
        while (seconds_elapsed(start_counter, SDL_GetPerformanceCounter()) < target_seconds_per_frame);
        Uint64 end_counter = SDL_GetPerformanceCounter();
        
        //pass renderer?
        game_update(target_seconds_per_frame, &new_input, renderer, &game_state);
        
        double delta = ((double)(end_counter - start_counter) / (double)perf_count_freq);
        start_counter = end_counter;
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
