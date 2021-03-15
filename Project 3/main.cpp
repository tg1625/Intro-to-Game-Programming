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

struct GameState {
    Entity* player;
    Entity* platforms;
    Entity* grass;
};

GameState state;


SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
//Matrices
glm::mat4 viewMatrix, projectionMatrix;

//Check if someone lost
bool over = false;

//Font tings
GLuint fontTextureID;

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
    state.player->width = 0.8f;
    state.player->height = 0.9f;
    state.player->textureID = LoadTexture("player_spritesheet.png");
    state.player->animCols = 7;
    state.player->animRows = 3;
    state.player->animIndices = new int[4]{ 0,1,2, 6 };
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->position = glm::vec3(0, 2.0f, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -0.45f, 0);
    state.player->velocity = glm::vec3(0);
    state.player->speed = 2.0f;

    //Platform Tingz
    state.platforms = new Entity[PLATFORM_COUNT];
    //horizontals
    GLuint platformTextureID = LoadTexture("boxExplosiveAlt.png");
    for (int i = 0; i < 6; i++) {
        state.platforms[i].textureID = platformTextureID;
        state.platforms[i].position = glm::vec3(i - 3.5, -3.25f, 0);
    }

    state.platforms[6].textureID = platformTextureID;
    state.platforms[6].position = glm::vec3(3.5f, -3.25f, 0); //bottom right of flag
    state.platforms[7].textureID = platformTextureID;
    state.platforms[7].position = glm::vec3(1.7f, 0.75f, 0); //floating right
    state.platforms[8].textureID = platformTextureID; 
    state.platforms[8].position = glm::vec3(-1.5f, -1.0f, 0); //floating left
 
    //left
    for (int i = 0; i < 8; i++) {
        state.platforms[9 + i].textureID = platformTextureID;
        state.platforms[9 + i].position = glm::vec3(-4.5, i - 3.25, 0);
    }
    //right
    for (int i = 0; i < 8; i++) {
        state.platforms[17 + i].textureID = platformTextureID;
        state.platforms[17 + i].position = glm::vec3(4.5, i - 3.25, 0);
    }
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Update(0, NULL, 0, NULL);
        state.platforms[i].entityType = EntityType::WALL;
    }

    //Win Flags Tings
    state.grass = new Entity[1];
    GLuint vicTextureID = LoadTexture("grass.png");
    state.grass[0].textureID = vicTextureID;
    state.grass[0].position = glm::vec3(2.5, -3.25f, 0);
    state.grass[0].Update(0, NULL, 0, NULL);
    state.grass[0].entityType = EntityType::GRASS;
 
    //Font Tings
    fontTextureID = LoadTexture("font1.png");
}

void ProcessInput() {
    SDL_Event event;

    state.player->movement = glm::vec3(0);
    state.player->acceleration.x = 0.0f;

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (!over) { //only let player move if the game is going
        if (keys[SDL_SCANCODE_LEFT]) {
            if (!state.player->collidedBottom || !state.player->collidedLeft || !state.player->collidedRight) {
                state.player->acceleration.x = -1.0f;
            }
        }
        if (keys[SDL_SCANCODE_RIGHT]) {
            if (!state.player->collidedBottom || !state.player->collidedLeft || !state.player->collidedRight) {
                state.player->acceleration.x = 1.0f;
            }
        }
        if (glm::length(state.player->movement) > 1.0f) {
           state.player->movement = glm::normalize(state.player->movement);
        }
    }
    else {
        if (keys[SDL_SCANCODE_SPACE]) { //hit space to restart
            state.player->position = glm::vec3(0, 2.75f, 0);
            state.player->movement = glm::vec3(0);
            state.player->acceleration = glm::vec3(0, -0.2f, 0);
            state.player->velocity = glm::vec3(0);
            state.player->isLost = false;
            state.player->isWon = false;
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
        state.player->Update(FIXED_TIMESTEP, state.platforms, PLATFORM_COUNT, state.grass);
        deltaTime -= FIXED_TIMESTEP;
    }
    accumulator = deltaTime;

    //If we lose or win, the game is over
    if (state.player->isLost || state.player->isWon) over = true;
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    state.grass[0].Render(&program);

    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Render(&program);
    }

    state.player->Render(&program);

    if (state.player->isWon) {
        DrawText(&program, fontTextureID, "Mission Successful!", 0.5f, -0.25f,
            glm::vec3(-2.75f, 2.75, 0));
    }
    else if (state.player->isLost) {
        DrawText(&program, fontTextureID, "Mission Failed", 0.5f, -0.25f,
            glm::vec3(-2.75f, 2.75, 0));
    }
    if (over) {
        DrawText(&program, fontTextureID, "Hit Space to Play Again", 0.5f, -0.25f, glm::vec3(-2.75f, 2.0, 0));
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
