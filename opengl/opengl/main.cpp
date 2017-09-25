#include "glextension.hpp"

#include <vector>
#include <iostream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const char *vertexShader = "#version 330 core layout (location = 0) in vec3 aPos; void main() { gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); }";

const char *fragmentShader = "#version 330 core out vec4 FragColor; void main() { FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); } ";

int main(void) {
    
    if(SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        exit(1);
    }
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    int major, minor;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor);
    
    printf("OpenGl version %d, %d \n", major, minor);
    
    SDL_Window *window = SDL_CreateWindow("SDL OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    
    if(glContext == NULL) {
        printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
        exit(1);
    }
    
    SDL_GL_LoadExtensions();
    
    if (SDL_GL_SetSwapInterval(1) < 0) {
        printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
        exit(1);
    }

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };
    
    GLuint vbo, vao;
    
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexShader, NULL);
    glCompileShader(vertex);
    
    GLint result;
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        GLint length;
        glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> error(length);
        glGetShaderInfoLog(vertex, length, &length, &error[0]);
        std::cout << "Failed to compile vertex shader!" << std::endl << &error[0] << std::endl;
        glDeleteShader(vertex);
        return 0;
    }
    
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentShader, NULL);
    glCompileShader(fragment);
    
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        GLint length;
        glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> error(length);
        glGetShaderInfoLog(fragment, length, &length, &error[0]);
        std::cout << "Failed to compile fragment shader!" << std::endl << &error[0] << std::endl;
        glDeleteShader(fragment);
        return 0;
    }
    
    GLuint program = glCreateProgram();
    
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    glValidateProgram(program);
    glUseProgram(program);
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    
    bool quit = false;
    
    SDL_Event e;
    
    while(!quit) {
        
        while(SDL_PollEvent(&e) != 0) {
            if( e.type == SDL_QUIT) {
                quit = true;
            }
        }
        glClearColor(0.0f, 1.0f, 1.0f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        SDL_GL_SwapWindow(window);
    }
    
    SDL_DestroyWindow(window);
    SDL_Quit();
    
}