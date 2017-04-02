#include "atlas.h"

#define TOKEN_BUFFER_SIZE 512

int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
    if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
        strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
        return 0;
    }
    return -1;
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

int convert_to_int(jsmntok_t* token, char* json_str)
{
    char *tmp = strndup(json_str + token->start, token->end - token->start);
    int ret = atoi(tmp);
    free(tmp);
    
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
    SDL_Rect *frames = (SDL_Rect*) malloc(sizeof(SDL_Rect) * num_of_frames);
    
    for (int i = 1, j = 0; i < ret; i++)
    {
        if (jsoneq(json, &tokens[i], "filename") == 0)
        {
            if(jsoneq(json, &tokens[i+2], "frame") == 0)
            {
                int x = convert_to_int(&tokens[i+5], json);
                int y = convert_to_int(&tokens[i+7], json);
                int w = convert_to_int(&tokens[i+9], json);
                int h = convert_to_int(&tokens[i+11], json);
                
                SDL_Rect src_rect = {x, y, w, h};
                frames[j] = src_rect;
                
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
    atlas->num_of_frames = num_of_frames;
    
    return atlas;
}

void destroy_atlas(Atlas* atlas)
{
    free(atlas->frames);
    free(atlas->file_name);
    free(atlas);
}
