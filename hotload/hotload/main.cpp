#include <iostream>
#include <dlfcn.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "game.h"

#define SCREEN_WIDTH 640    
#define SCREEN_HEIGHT 480

typedef void (*game_update_fn)(offscreen_buffer* memory);

struct game_code
{
    void *dll;
    time_t last_load_time;
    game_update_fn update;
};

inline time_t get_last_write_time(const char* file_name)
{
    struct stat file_attrs;
    stat(file_name, &file_attrs);
    
    return file_attrs.st_mtime;
}

bool load_app_dll(game_code* app_dll)
{
    const char* source_dll_path = "libhotload.so";
    time_t last_write_time = get_last_write_time(source_dll_path);
    
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
    void *lib = dlopen(source_dll_path, RTLD_LOCAL | RTLD_LAZY);
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
    bool quit = false;
    time_t last_write_time = 0;

#if 0
    game_code app_dll;
    memset(&app_dll, 0, sizeof(game_code));
    bool loading_dll = true;
#endif
    
    offscreen_buffer buffer = {};
    buffer.texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_ARGB8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                SCREEN_WIDTH,
                                SCREEN_HEIGHT);
    
    buffer.width = SCREEN_WIDTH;
    buffer.height = SCREEN_HEIGHT;
    buffer.bytes_per_pixel = 4;
    buffer.bitmap_memory = malloc(buffer.width * buffer.height * buffer.bytes_per_pixel);
    
    while(!quit)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                {
                    quit = true;
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
        
        game_update(&buffer);
        
#if 0        
        time_t write_time = get_last_write_time("libhotload.so");
        if (write_time > last_write_time)
        {
            printf("Changed\n");
            last_write_time = write_time;
            loading_dll = true;
        }
        
        if (loading_dll)
        {
           if(load_app_dll(&app_dll))
           {
               loading_dll = false;
           }
        }
        else if(app_dll.dll)
        {
            app_dll.update(&game_memory);
        }
#endif
        
    }
    
    SDL_DestroyWindow(window);
    SDL_Quit();
}
