#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, rockMatrix, monsterMatrix, projectionMatrix;

GLuint rockID, monsterID;

GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);
    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    stbi_image_free(image);
    return textureID;
}


void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Fun!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    rockMatrix = glm::mat4(1.0f);
    monsterMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    glUseProgram(program.programID);

    glEnable(GL_BLEND);
    // Good setting for transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    rockID = LoadTexture("Rock2.png");
    monsterID = LoadTexture("Pink_Monster.png");
}

void ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            gameIsRunning = false;
        }
    }
}


float monster_x = 0.0f;
float monster_y = -3.25f;
float monster_rotate = 0.0f;

float rock_x = 0.0f;
float rock_y = 0.0f;
float rock_rotate = 0.0f;

float lastTicks = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    rock_rotate += 70.0f * deltaTime;

    if (monster_x > 5.0f) {
        monster_x = -5.0f;
    }
    else {
        monster_x += 2.0f * deltaTime;
    }
 

    if (rock_x > 6.0f || rock_x < -6.0f || rock_y > 4.75f || rock_x < -4.75f) {
        rock_x = monster_x + 0.3f;
        rock_y = monster_y + 0.25f;
       /* rock_rotate = monster_rotate;*/
    }
    else {
        rock_x += 6.0f * deltaTime;
        rock_y += 2.0f * deltaTime;
    }



    monsterMatrix = glm::mat4(1.0f);
    rockMatrix = glm::mat4(1.0f);

    rockMatrix = glm::translate(rockMatrix, glm::vec3(rock_x, rock_y, 1.0f));
    rockMatrix = glm::rotate(rockMatrix, glm::radians(rock_rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    
    rockMatrix = glm::scale(rockMatrix, glm::vec3(0.5f, 0.5f, 1.0f));

    monsterMatrix = glm::translate(monsterMatrix, glm::vec3(monster_x, monster_y, 1.0f));
    /*monsterMatrix = glm::rotate(monsterMatrix, glm::radians(monster_rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    monsterMatrix = glm::scale(monsterMatrix, glm::vec3(2.0f, 2.0f, 1.0f));*/
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);

    program.SetModelMatrix(rockMatrix);
    glBindTexture(GL_TEXTURE_2D, rockID);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    program.SetModelMatrix(monsterMatrix);
    glBindTexture(GL_TEXTURE_2D, monsterID);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
    
    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }
    
    Shutdown();
    return 0;
}
