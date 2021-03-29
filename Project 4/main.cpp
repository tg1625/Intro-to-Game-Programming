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
#include "Entity.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "vector"
#include "string"

#define PLATFORM_COUNT 25
#define ENEMY_COUNT 3

struct GameState {
    Entity* player;
    Entity* platforms;
    Entity* enemies;
    bool over;
};

GameState state;

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
//Matrices
glm::mat4 viewMatrix, projectionMatrix;

bool over = false;

//Font tings
GLuint fontTextureID;

void GameOver() {
    over = true;
    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].aiState = IDLE;
        state.enemies[i].movement = glm::vec3(0);
    }
}

void DrawText(ShaderProgram* program, GLuint fontTextureID, std::string text, float size, float spacing, glm::vec3 position){
    float width = 1.0f / 16.0f;
    float height = 1.0f / 16.0f;

    std::vector<float> vertices;
    std::vector<float> texCoords;

    for (int i = 0; i < text.size(); i++) {
        int index = (int)text[i];
        float offset = (size + spacing) * i;

        float u = (float)(index % 16) / 16.0f;
        float v = (float)(index / 16) / 16.0f;
        vertices.insert(vertices.end(), {
          offset + (-0.5f * size), 0.5f * size,
          offset + (-0.5f * size), -0.5f * size,
          offset + (0.5f * size), 0.5f * size,
          offset + (0.5f * size), -0.5f * size,
          offset + (0.5f * size), 0.5f * size,
          offset + (-0.5f * size), -0.5f * size,
            });
        texCoords.insert(texCoords.end(), {
            u, v,
            u, v + height,
            u + width, v,
            u + width, v + height,
            u + width, v,
            u, v + height,
            });
    } 

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    program->SetModelMatrix(modelMatrix);

    glUseProgram(program->programID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
    glEnableVertexAttribArray(program->texCoordAttribute);

    glBindTexture(GL_TEXTURE_2D, fontTextureID);
    glDrawArrays(GL_TRIANGLES, 0, (int)(text.size() * 6));

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}


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

    //Player Tings
    state.player = new Entity();
    state.player->entityType = EntityType::PLAYER;
    state.player->width = 0.8f;
    state.player->height = 0.9f;
    state.player->jumpPower = 7.0f;
    state.player->textureID = LoadTexture("player_spritesheet.png");
    state.player->animCols = 7;
    state.player->animRows = 3;
    state.player->animIndices = new int[2]{ 0, 6 };
    state.player->animFrames = 2;
    state.player->animIndex = 0;
    state.player->position = glm::vec3(-4.0, 0.0f, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81, 0);
    state.player->velocity = glm::vec3(0);
    state.player->speed = 2.0f;
    state.player->Update(0,NULL, NULL, 0, NULL, 0);

    //Enemy Tings
    state.enemies = new Entity[ENEMY_COUNT];
    //Bat Tings
    state.enemies[0].entityType = EntityType::ENEMY;
    state.enemies[0].aiType = BAT;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].width = 1.0f;
    state.enemies[0].height = 1.0f;
    state.enemies[0].textureID = LoadTexture("bat_sheet.png");
    state.enemies[0].animCols = 4;
    state.enemies[0].animRows = 1;
    state.enemies[0].animIndices = new int[2]{ 0, 2 };
    state.enemies[0].animFrames = 2;
    state.enemies[0].animIndex = 0;
    state.enemies[0].position = glm::vec3(1.5f, 0.0f, 0);
    state.enemies[0].movement = glm::vec3(0);
    state.enemies[0].acceleration = glm::vec3(0, 0, 0);
    state.enemies[0].velocity = glm::vec3(0);
    state.enemies[0].speed = 1.0f;
    //state.enemies[0].Update(0,NULL,  NULL, 0, NULL, 0);

    //Blocky Tingz
    state.enemies[1].entityType = EntityType::ENEMY;
    state.enemies[1].aiType = BLOCKY;
    state.enemies[1].aiState = IDLE;
    state.enemies[1].width = 1.0f;
    state.enemies[1].height = 1.0f;
    state.enemies[1].textureID = LoadTexture("blocky_sheet.png");
    state.enemies[1].animCols = 2;
    state.enemies[1].animRows = 1;
    state.enemies[1].animIndices = new int[2]{ 0, 1 };
    state.enemies[1].animFrames = 2;
    state.enemies[1].animIndex = 1;
    state.enemies[1].position = glm::vec3(-2.0f, 0.0f, 0);
    state.enemies[1].movement = glm::vec3(0);
    state.enemies[1].acceleration = glm::vec3(0, 0, 0);
    state.enemies[1].velocity = glm::vec3(0);
    state.enemies[1].speed = 3.0f;
    //state.enemies[1].Update(0,NULL, NULL, 0, NULL, 0);

    //Mouse Tingz
    state.enemies[2].entityType = EntityType::ENEMY;
    state.enemies[2].aiType = MOUSE;
    state.enemies[2].aiState = PATROLLING;
    state.enemies[2].width = 0.9f;
    state.enemies[2].height = 0.9f;
    state.enemies[2].textureID = LoadTexture("mouse_sheet.png");
    state.enemies[2].animCols = 3;
    state.enemies[2].animRows = 1;
    state.enemies[2].animIndices = new int[2]{ 0, 2 };
    state.enemies[2].animFrames = 2;
    state.enemies[2].animIndex = 0;
    state.enemies[2].position = glm::vec3(4.0f, -1.25f, 0);
    state.enemies[2].movement = glm::vec3(0);
    state.enemies[2].acceleration = glm::vec3(0, -900.81, 0);
    state.enemies[2].velocity = glm::vec3(0);
    state.enemies[2].speed = 1.0f;
    //state.enemies[2].Update(0,NULL, NULL, 0, NULL, 0);

    /*state.enemies = [&state.bat, &state.blocky, &state.mouse];*/


    //Platform Tingz
    state.platforms = new Entity[PLATFORM_COUNT];
    //horizontals
    GLuint groundTextureID = LoadTexture("grassMid.png");
    for (int i = 0; i < 5; i++) {
        state.platforms[i].textureID = groundTextureID;
        state.platforms[i].position = glm::vec3(i - 4.5, -3.25f, 0);
    }
    for (int i = 5; i < 10; i++) {
        state.platforms[i].textureID = groundTextureID;
        state.platforms[i].position = glm::vec3(i - 4.5, -2.25f, 0);
    }
    groundTextureID = LoadTexture("grassCenter.png");
    for (int i = 10; i < 15; i++) {
        state.platforms[i].textureID = groundTextureID;
        state.platforms[i].position = glm::vec3(i - 9.5, -3.25f, 0);
    }

    //"Invisible" Walls
    GLuint wallTextureID = LoadTexture("grass.png");
    for (int i = 15; i < 20; i++) {
        state.platforms[i].textureID = wallTextureID;
        state.platforms[i].position = glm::vec3(5.5f, i - 16.25, 0);
    }
    for (int i = 20; i < 25; i++) {
        state.platforms[i].textureID = wallTextureID;
        state.platforms[i].position = glm::vec3(-5.5f, i - 22.25, 0);
    }
    //state.platforms[16].textureID = platformTextureID; 
    //state.platforms[16].position = glm::vec3(-1.5f, -1.0f, 0); //floating left
 
 
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Update(0,NULL, NULL, 0, NULL, 0);
        state.platforms[i].entityType = EntityType::PLATFORM;
    }
 
    //Font Tings
    fontTextureID = LoadTexture("font1.png");
}

void ProcessInput() {
    SDL_Event event;

    state.player->movement = glm::vec3(0);
    state.player->acceleration.x = 0.0f;

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
                if (state.player->collidedBottom) {
                    state.player->jump = true;
                }
                /*state.player->jump = true;*/
                break;
            }
            break; // SDL_KEYDOWN
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (!over) { //only let player move if the game is going
        if (keys[SDL_SCANCODE_LEFT]) {
            if (!state.player->collidedLeft || !state.player->collidedRight) {
                state.player->movement.x = -1.0f;
            }
        }
        if (keys[SDL_SCANCODE_RIGHT]) {
            if (!state.player->collidedLeft || !state.player->collidedRight) {
                state.player->movement.x = 1.0f;
            }
        }
        if (glm::length(state.player->movement) > 1.0f) {
           state.player->movement = glm::normalize(state.player->movement);
        }
        
    }
    else {
        if (keys[SDL_SCANCODE_RETURN]) { //hit enter to restart
            state.player->animIndex = 0;
            state.player->position = glm::vec3(-4.25, 0.0f, 0);
            state.player->movement = glm::vec3(0);
            state.player->acceleration = glm::vec3(0, -9.81, 0);
            state.player->velocity = glm::vec3(0);
            state.player->isLost = false;
            state.player->isWon = false;
            for (int i = 0; i < ENEMY_COUNT; i++) {
                state.enemies[i].isActive = true;
                state.enemies[i].forward = true;
            }
            //Bat reset
            state.enemies[0].aiState = IDLE;
            state.enemies[0].animIndex = 0;
            state.enemies[0].position = glm::vec3(1.5f, 0.0f, 0);
            state.enemies[0].movement = glm::vec3(0);
            state.enemies[0].acceleration = glm::vec3(0, 0, 0);
            state.enemies[0].velocity = glm::vec3(0);
            //Blocky reset
            state.enemies[1].aiState = IDLE;
            state.enemies[1].animIndex = 1;
            state.enemies[1].position = glm::vec3(-2.0f, 0.0f, 0);
            state.enemies[1].movement = glm::vec3(0);
            state.enemies[1].acceleration = glm::vec3(0, 0, 0);
            state.enemies[1].velocity = glm::vec3(0);
            //Mouse reset
            state.enemies[2].animIndex = 0;
            state.enemies[2].position = glm::vec3(4.0f, -1.25f, 0);
            state.enemies[2].movement = glm::vec3(0);
            state.enemies[2].acceleration = glm::vec3(0, -90.81, 0);
            state.enemies[2].velocity = glm::vec3(0);
            state.enemies[2].aiState = PATROLLING;
            over = false;
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
    if (!over) {
        float ticks = (float)SDL_GetTicks() / 1000.0f;
        float deltaTime = ticks - lastTicks;
        lastTicks = ticks;

        deltaTime += accumulator;
        if (deltaTime < FIXED_TIMESTEP) {
            accumulator = deltaTime;
            return;
        }
        while (deltaTime >= FIXED_TIMESTEP) {
            // Update. Notice it's FIXED_TIMESTEP. Not deltaTime
            state.player->Update(FIXED_TIMESTEP, state.player, state.platforms, PLATFORM_COUNT, state.enemies, ENEMY_COUNT);
            for (int i = 0; i < ENEMY_COUNT; i++) {
                state.enemies[i].Update(FIXED_TIMESTEP, state.player, state.platforms, PLATFORM_COUNT, state.enemies, ENEMY_COUNT);
            }
            deltaTime -= FIXED_TIMESTEP;
        }
        accumulator = deltaTime;

        int activeCount = 0;
        for (int i = 0; i < ENEMY_COUNT; i++) {
            if (state.enemies[i].isActive) activeCount++;
        }

        if (activeCount == 0) state.player->isWon = true;
        //If we lose or win, the game is over
        if (state.player->isLost || state.player->isWon) GameOver();
    }
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Render(&program);
    }

    state.player->Render(&program);
    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].Render(&program);
    }

    if (state.player->isWon) {
        DrawText(&program, fontTextureID, "You Win!", 0.5f, -0.25f,
            glm::vec3(-2.75f, 2.75, 0));
    }
    else if (state.player->isLost) {
        DrawText(&program, fontTextureID, "You Lose :(", 0.5f, -0.25f,
            glm::vec3(-2.75f, 2.75, 0));
    }
    if (over) {
        DrawText(&program, fontTextureID, "Hit Enter to Play Again", 0.5f, -0.25f, glm::vec3(-2.75f, 2.0, 0));
    }
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
