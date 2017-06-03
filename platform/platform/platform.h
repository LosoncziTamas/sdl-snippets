#ifndef platform_h
#define platform_h

#include "desktop.h"

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

struct Game_State
{
    World world;
    Player player;
    SDL_bool inited;
};

void game_update(float delta, Game_Controller_Input *input, SDL_Renderer *renderer, Game_State *game_state);

#endif
