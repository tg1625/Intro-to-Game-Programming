#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_opengl.h>  
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Util.h"
#include "Entity.h"
#include "Map.h"
#include "Scene.h"
#include "MainMenu.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "LoseScreen.h"
#include "WinScreen.h"

#include "vector"
#include "string"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
//Matrices
glm::mat4 viewMatrix, projectionMatrix;

//Music
Mix_Music* music;
Mix_Chunk* bounce;

//Lives
int lives = 3;

Scene* currentScene;
Scene* sceneList[6];

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

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("background.mp3");
    bounce = Mix_LoadWAV("bounce.wav");
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 5);

    viewMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);

    glEnable(GL_BLEND);
    // Good setting for transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(219.0f / 255.0f, 240.0f / 255.0f, 255.0f / 255.0f, 1.0f);

    sceneList[0] = new MainMenu();
    sceneList[1] = new Level1();
    sceneList[2] = new Level2();
    sceneList[3] = new Level3();
    sceneList[4] = new LoseScreen();
    sceneList[5] = new WinScreen();
    SwitchToScene(sceneList[0]);
}

void ProcessInput() {
    SDL_Event event;

    if (currentScene == sceneList[0] || currentScene == sceneList[4] || currentScene == sceneList[5]) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_RETURN:
                    SwitchToScene(sceneList[1]);
                    break;
                }
                break; // SDL_KEYDOWN
            }
        }
    }
    else {
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
                        Mix_PlayChannel(1, bounce, 0);
                        Mix_VolumeChunk(bounce, MIX_MAX_VOLUME / 3);
                    }
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
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }
    while (deltaTime >= FIXED_TIMESTEP) {
        viewMatrix = currentScene->Update(FIXED_TIMESTEP, viewMatrix);
        // Update. Notice it's FIXED_TIMESTEP. Not deltaTime
            
        deltaTime -= FIXED_TIMESTEP;
    }
    accumulator = deltaTime;

    ////If we got hit, we lose a life
    if (currentScene->state.player && currentScene->state.player->isHit) lives -= 1;

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

        //Scene Switching Tingz
        if(currentScene->state.nextScene >= 0) SwitchToScene(sceneList[currentScene->state.nextScene]);
        if (currentScene->state.player && currentScene->state.player->isHit) SwitchToScene(currentScene);
        if (lives == 0) {
            SwitchToScene(sceneList[4]);
            lives = 3;
        };

        Render();
    }

    Shutdown();
    return 0;
}
