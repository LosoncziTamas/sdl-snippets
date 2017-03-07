#include <iostream>
#include <dlfcn.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "game.h"

#define SCREEN_WIDTH 640    
#define SCREEN_HEIGHT 480

typedef void (*game_update_fn)(Offscreen_Buffer* memory);

struct Game_Code
{
    void *dll;
    time_t last_load_time;
    game_update_fn update;
};

inline time_t get_last_write_time(const char *file_name)
{
    struct stat file_attrs;
    if (stat(file_name, &file_attrs) < 0) {
        return 0;
    }
    
    return file_attrs.st_mtime;
}

bool load_app_dll(Game_Code* app_dll, const char *src_dll_path)
{
    time_t last_write_time = get_last_write_time(src_dll_path);
    
    if (app_dll->last_load_time >= last_write_time)
    {
        return false;
    }
    else
    {
        if (app_dll->dll)
        {
            dlclose(app_dll->dll);
            app_dll->dll = 0;
        }
    }
    void *lib = dlopen(src_dll_path, RTLD_LOCAL | RTLD_LAZY);
    if (!lib)
    {
        return false;
    }
    
    app_dll->dll = lib;
    app_dll->update = (game_update_fn)dlsym(lib, "game_update");
    app_dll->last_load_time = last_write_time;
    return true;
}

int main(void)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_CreateWindowAndRenderer(SCREEN_WIDTH,
                                SCREEN_HEIGHT,
                                SDL_WINDOW_RESIZABLE,
                                &window,
                                &renderer);
    SDL_Event event;
    bool running = true;
    time_t last_write_time = 0;

    Game_Code app_dll = {};
    bool loading_dll = true;
    const char *dll_path = "game.so";
    
    Offscreen_Buffer buffer = {};
    buffer.texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_ARGB8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                SCREEN_WIDTH,
                                SCREEN_HEIGHT);
    
    buffer.width = SCREEN_WIDTH;
    buffer.height = SCREEN_HEIGHT;
    buffer.bytes_per_pixel = 4;
    buffer.bitmap_memory = malloc(buffer.width * buffer.height * buffer.bytes_per_pixel);
    
    while(running)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                {
                    running = false;
                } break;
            }
        }
        
        SDL_UpdateTexture(buffer.texture,
                          0,
                          buffer.bitmap_memory,
                          buffer.width * buffer.bytes_per_pixel);
        
        SDL_RenderCopy(renderer,
                       buffer.texture,
                       0,
                       0);
        
        SDL_RenderPresent(renderer);
     
        time_t write_time = get_last_write_time(dll_path);
        
        if (write_time > last_write_time)
        {
            last_write_time = write_time;
            loading_dll = true;
        }
        
        if (loading_dll)
        {
           if(load_app_dll(&app_dll, dll_path))
           {
               loading_dll = false;
           }
        }
        else if(app_dll.dll)
        {
            app_dll.update(&buffer);
        }
    }
    
    SDL_DestroyWindow(window);
    SDL_Quit();
}
