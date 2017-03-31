#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include "jsmn.h"

#define TOKEN_BUFFER_SIZE 512

struct Frame
{
    char* name;
    SDL_Rect src_rect;
};

struct Atlas
{
    int size;
    char *file_name;
    Frame *frames;
};

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
    if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
        strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
        return 0;
    }
    return -1;
}

int convert_to_int(jsmntok_t* token, char* json_str)
{
    char *tmp = strndup(json_str + token->start, token->end - token->start);
    int ret = atoi(tmp);
    free(tmp);
    
    return ret;
}

char *read_file(const char* file_name)
{
    char *file_content = NULL;
    SDL_RWops *file = SDL_RWFromFile(file_name, "r");
    if (file)
    {
        Sint64 file_size = SDL_RWsize(file);
        file_content = (char*) malloc(file_size + 1);
        if(SDL_RWread(file, file_content, sizeof(char), file_size))
        {
            file_content[file_size] = '\0';
        }
        else
        {
            printf( "Error: Unable to read file! SDL Error: %s\n", SDL_GetError());
            free(file_content);
            file_content = NULL;
        }
        SDL_RWclose(file);
    }
    else
    {
        printf( "Error: Unable to open file! SDL Error: %s\n", SDL_GetError());
    }
    return file_content;
}

int count_frame_objects(jsmntok_t *tokens, int num_of_tokens, char *json_str)
{
    int ret = 0;
    for (int i = 1; i < num_of_tokens; i++)
    {
        if (jsoneq(json_str, &tokens[i], "frame") == 0)
        {
            ++ret;
        }
    }
    return ret;
}

Atlas *create_atlas_from_json(char *json)
{
    unsigned int n = TOKEN_BUFFER_SIZE;
    jsmn_parser parser;
    jsmn_init(&parser);
    
    jsmntok_t *tokens = (jsmntok_t*) malloc(sizeof(jsmntok_t) * n);
    int ret = jsmn_parse(&parser,
                         json,
                         strlen(json),
                         tokens,
                         n);
    
    while (ret == JSMN_ERROR_NOMEM)
    {
        n = n * 2 + 1;
        printf("Expand token buffer to %d", n);
        
        tokens = (jsmntok_t*) realloc(tokens, sizeof(jsmntok_t) * n);
        
        ret = jsmn_parse(&parser,
                         json,
                         strlen(json),
                         tokens,
                         n);
    }
    
    if (ret == JSMN_ERROR_INVAL)
    {
        printf("Invalid JSON string.");
    }
    if (ret == JSMN_ERROR_PART)
    {
        printf("Truncated JSON string.");
    }
    
    SDL_assert(tokens[0].type == JSMN_OBJECT);
    
    int num_of_frames = count_frame_objects(tokens, ret, json);

    Atlas *atlas = (Atlas*) malloc(sizeof(Atlas));
    Frame *frames = (Frame*) malloc(sizeof(Frame) * num_of_frames);
    for (int i = 1, j = 0; i < ret; i++)
    {
        if (jsoneq(json, &tokens[i], "filename") == 0)
        {
            int name_len = tokens[i+1].end - tokens[i+1].start;
            char *file_name = strndup(json + tokens[i+1].start, name_len);
            
            if(jsoneq(json, &tokens[i+2], "frame") == 0)
            {
                int x = convert_to_int(&tokens[i+5], json);
                int y = convert_to_int(&tokens[i+7], json);
                int w = convert_to_int(&tokens[i+9], json);
                int h = convert_to_int(&tokens[i+11], json);
                
                SDL_Rect src_rect = {x, y, w, h};
                Frame frame = {file_name, src_rect};
                frames[j] = frame;
                
                i += 11;
                j++;
            }
            else
            {
                printf("Unexpected key.");
            }
        }
        if (jsoneq(json, &tokens[i], "image") == 0)
        {
            atlas->file_name = strndup(json + tokens[i+1].start, tokens[i+1].end - tokens[i+1].start);
        }
    }
    
    atlas->frames = frames;
    atlas->size = num_of_frames;
    
    return atlas;
}

void destroy_atlas(Atlas* atlas)
{
    for (int i = 0; i < atlas->size; ++i)
    {
        Frame* f = &atlas->frames[i];
        free(f->name);
    }
    free(atlas->frames);
    free(atlas->file_name);
    free(atlas);
}

int main(void)
{
    char *json = read_file("player.json");
    Atlas *atlas = create_atlas_from_json(json);
    
    destroy_atlas(atlas);
    free(json);
    return 0;
}
