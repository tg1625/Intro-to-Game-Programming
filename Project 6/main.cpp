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

float invincibleTime = 0;

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
//Matrices
glm::mat4 viewMatrix, projectionMatrix;
glm::mat4 uiViewMatrix, uiProjectionMatrix;
GLuint fontTextureID = Util::LoadTexture("font1.png");;

//Music
Mix_Music* music;
Mix_Chunk* hit;
Mix_Chunk* win;
Mix_Chunk* collect;
Mix_Chunk* seen;
Mix_Chunk* lose;

//Sound Triggers
bool noOpener = true;

Scene* currentScene;
Scene* sceneList[6];

void SwitchToScene(Scene* scene) {
    currentScene = scene;
    currentScene->Initialize();
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Bean Dad", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

    #ifdef _WINDOWS
        glewInit();
    #endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("background.mp3");
    hit = Mix_LoadWAV("hit2.wav");
    win = Mix_LoadWAV("win.wav");
    collect = Mix_LoadWAV("collect2.wav");
    seen = Mix_LoadWAV("seen.wav");
    lose = Mix_LoadWAV("lose2.wav");
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 5);

    viewMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    //UI
    uiViewMatrix = glm::mat4(1.0);
    uiProjectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

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
        currentScene->state.player->animIndices = new int[1]{ 4 };
        currentScene->state.player->animFrames = 1;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
            }
        }

        const Uint8* keys = SDL_GetKeyboardState(NULL);

        if (!currentScene->state.over) { //only let player move if the game is going
            if (keys[SDL_SCANCODE_LEFT]) {
                currentScene->state.player->animIndices = new int[3]{ 3, 8, 13 };
                currentScene->state.player->animFrames = 3;
                if (!currentScene->state.player->collidedLeft || !currentScene->state.player->collidedRight) {
                    currentScene->state.player->movement.x = -1.0f;
                }
            }
            if (keys[SDL_SCANCODE_RIGHT]) {
                currentScene->state.player->animIndices = new int[3]{ 1, 6, 11 };
                currentScene->state.player->animFrames = 3;
                if (!currentScene->state.player->collidedLeft || !currentScene->state.player->collidedRight) {
                    currentScene->state.player->movement.x = 1.0f;
                }
            }
            if (keys[SDL_SCANCODE_UP]) {
                currentScene->state.player->animIndices = new int[3]{ 0, 5, 10 };
                currentScene->state.player->animFrames = 3;
                if (!currentScene->state.player->collidedTop || !currentScene->state.player->collidedBottom) {
                    currentScene->state.player->movement.y = 1.0f;
                }
            }
            if (keys[SDL_SCANCODE_DOWN]) {
                currentScene->state.player->animIndices = new int[3]{ 2, 7, 12};
                currentScene->state.player->animFrames = 3;
                if (!currentScene->state.player->collidedTop || !currentScene->state.player->collidedBottom) {
                    currentScene->state.player->movement.y = -1.0f;
                }
            }
            if (glm::length(currentScene->state.player->movement) > 1.0f) {
                currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);
            }
            if(currentScene->state.player->animFrames == 1) currentScene->state.player->animIndex = 0;

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

    //Sound Effects
    if (currentScene == sceneList[1] || currentScene == sceneList[2] || currentScene == sceneList[3]) {
        if (noOpener && currentScene->state.player->hasOpener) {
            Mix_PlayChannel(1, collect, 0);
            Mix_VolumeChunk(collect, MIX_MAX_VOLUME / 3);
            noOpener = false;
        }
        if (currentScene->state.player->isWon) {
            Mix_PlayChannel(1, win, 0);
            Mix_VolumeChunk(win, MIX_MAX_VOLUME / 3);
        }
        if (currentScene->state.player->isLost) {
            Mix_PlayChannel(1, lose, 0);
            Mix_VolumeChunk(lose, MIX_MAX_VOLUME / 3);
        }
        if (currentScene->state.player->isHit) {
            Mix_PlayChannel(1, hit, 0);
            Mix_VolumeChunk(hit, MIX_MAX_VOLUME / 3);
            invincibleTime = 0.75;
        }
        if (currentScene->state.player->wasSeen) {
            Mix_PlayChannel(1, seen, 0);
            Mix_VolumeChunk(seen, MIX_MAX_VOLUME / 3);
        }
        invincibleTime -= deltaTime;
        //SDL_Log("Invincible time: %f", invincibleTime);
        if (invincibleTime <= 0) {
            currentScene->state.player->isInvincible = false;
        }
    }
}

void Render() {    
    glClear(GL_COLOR_BUFFER_BIT);
    program.SetViewMatrix(viewMatrix);
    currentScene->Render(&program);

    //UI
    program.SetProjectionMatrix(uiProjectionMatrix);
    program.SetViewMatrix(uiViewMatrix);
    currentScene->RenderUI(&program);
    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    Mix_FreeChunk(collect);
    Mix_FreeChunk(win);
    Mix_FreeChunk(hit);
    Mix_FreeChunk(seen);
    Mix_FreeMusic(music);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();

        //Scene Switching Tingz
        if (currentScene->state.nextScene >= 0) {
            noOpener = true;
            SwitchToScene(sceneList[currentScene->state.nextScene]);
        }

        Render();
    }

    Shutdown();
    return 0;
}
