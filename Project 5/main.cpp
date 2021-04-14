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
#include "Util.h"
#include "Entity.h"
#include "Map.h"
#include "Scene.h"
#include "Level1.h"

#include "vector"
#include "string"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
//Matrices
glm::mat4 viewMatrix, projectionMatrix;

// Add some variables and SwitchToScene function
Scene* currentScene;
Level1* level1;
void SwitchToScene(Scene* scene) {
    currentScene = scene;
    currentScene->Initialize();
}




void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Lander!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

    #ifdef _WINDOWS
        glewInit();
    #endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    viewMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);

    glEnable(GL_BLEND);
    // Good setting for transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(246.0f / 255.0f, 227.0f / 255.0f, 208.0f / 255.0f, 1.0f);

    // Clear stuff out of initialize and add this to the bottom:
    level1 = new Level1();
    SwitchToScene(level1);
}

void ProcessInput() {
    SDL_Event event;

    currentScene->state.player->movement = glm::vec3(0);
    currentScene->state.player->acceleration.x = 0.0f;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                // Move the player left
                break;

            case SDLK_RIGHT:
                // Move the player right
                break;

            case SDLK_SPACE:
                // Some sort of action
                if (currentScene->state.player->collidedBottom) {
                    currentScene->state.player->jump = true;
                }
                /*state.player->jump = true;*/
                break;
            }
            break; // SDL_KEYDOWN
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (!currentScene->state.over) { //only let player move if the game is going
        if (keys[SDL_SCANCODE_LEFT]) {
            if (!currentScene->state.player->collidedLeft || !currentScene->state.player->collidedRight) {
                currentScene->state.player->movement.x = -1.0f;
            }
        }
        if (keys[SDL_SCANCODE_RIGHT]) {
            if (!currentScene->state.player->collidedLeft || !currentScene->state.player->collidedRight) {
                currentScene->state.player->movement.x = 1.0f;
            }
        }
        if (glm::length(currentScene->state.player->movement) > 1.0f) {
            currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);
        }
        
    }
    else {
        if (keys[SDL_SCANCODE_RETURN]) { //hit enter to restart
            currentScene->state.player->animIndex = 0;
            currentScene->state.player->position = glm::vec3(-4.25, 0.0f, 0);
            currentScene->state.player->movement = glm::vec3(0);
            currentScene->state.player->acceleration = glm::vec3(0, -9.81, 0);
            currentScene->state.player->velocity = glm::vec3(0);
            currentScene->state.player->isLost = false;
            currentScene->state.player->isWon = false;
            //for (int i = 0; i < ENEMY_COUNT; i++) {
            //    state.enemies[i].isActive = true;
            //    state.enemies[i].forward = true;
            //}
            ////Bat reset
            //state.enemies[0].aiState = IDLE;
            //state.enemies[0].animIndex = 0;
            //state.enemies[0].position = glm::vec3(1.5f, 0.0f, 0);
            //state.enemies[0].movement = glm::vec3(0);
            //state.enemies[0].acceleration = glm::vec3(0, 0, 0);
            //state.enemies[0].velocity = glm::vec3(0);
            ////Blocky reset
            //state.enemies[1].aiState = IDLE;
            //state.enemies[1].animIndex = 1;
            //state.enemies[1].position = glm::vec3(-2.0f, 0.0f, 0);
            //state.enemies[1].movement = glm::vec3(0);
            //state.enemies[1].acceleration = glm::vec3(0, 0, 0);
            //state.enemies[1].velocity = glm::vec3(0);
            ////Mouse reset
            //state.enemies[2].animIndex = 0;
            //state.enemies[2].position = glm::vec3(4.0f, -1.25f, 0);
            //state.enemies[2].movement = glm::vec3(0);
            //state.enemies[2].acceleration = glm::vec3(0, -90.81, 0);
            //state.enemies[2].velocity = glm::vec3(0);
            //state.enemies[2].aiState = PATROLLING;
            currentScene->state.over = false;
        }
    }

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            gameIsRunning = false;
        }
    }
}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {
    if (!currentScene->state.over) {
        float ticks = (float)SDL_GetTicks() / 1000.0f;
        float deltaTime = ticks - lastTicks;
        lastTicks = ticks;

        deltaTime += accumulator;
        if (deltaTime < FIXED_TIMESTEP) {
            accumulator = deltaTime;
            return;
        }
        while (deltaTime >= FIXED_TIMESTEP) {
            currentScene->Update(FIXED_TIMESTEP);
            // Update. Notice it's FIXED_TIMESTEP. Not deltaTime
            
            deltaTime -= FIXED_TIMESTEP;
        }
        accumulator = deltaTime;

        int activeCount = 0;
        //for (int i = 0; i < ENEMY_COUNT; i++) {
        //    if (state.enemies[i].isActive) activeCount++;
        //}

        //if (activeCount == 0) state.player->isWon = true;
        ////If we lose or win, the game is over
        //if (state.player->isLost || state.player->isWon) GameOver();

        viewMatrix = glm::mat4(1.0f);
        if (currentScene->state.player->position.x > 9) {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-9, 3.75, 0));
        }
        else if (currentScene->state.player->position.x > 5) {
            viewMatrix = glm::translate(viewMatrix,
                glm::vec3(-currentScene->state.player->position.x, 3.75, 0));
        }
        else {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.75, 0));
        }

    }
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    program.SetViewMatrix(viewMatrix);

    currentScene->Render(&program);

    
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
