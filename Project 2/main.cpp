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

#include "vector"
#include "string"


SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
//Matrixes
glm::mat4 viewMatrix, ballMatrix, paddleLMatrix, paddleRMatrix, projectionMatrix;
//Texture IDs
GLuint ballID, paddleLID, paddleRID;
//Movement Vectors
glm::vec3 paddleLMovement, paddleRMovement;
glm::vec3 ballMovement = normalize(glm::vec3(1, 1, 0)); 
//Position Vectors
glm::vec3 paddleLPosition, paddleRPosition, ballPosition;
//Speeds
float ballSpeed = 3.0f;
float paddleLSpeed = 4.0f;
float paddleRSpeed = 4.0f;

//Hitboxes
float paddleH = 2.0f;
float paddleW = 0.20f;
float ballH = 0.5f;
float ballW = 0.5f;

//Score variables
int player1Score = 0;
int player2Score = 0;

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
    displayWindow = SDL_CreateWindow("Pong!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    viewMatrix = glm::mat4(1.0f);
    ballMatrix = glm::mat4(1.0f);
    paddleLMatrix = glm::mat4(1.0f);
    paddleRMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);

    glEnable(GL_BLEND);
    // Good setting for transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(224.0f / 255.0f, 203.0f / 255.0f, 194.0f / 255.0f, 1.0f);

    ballID = LoadTexture("ball.png");
    paddleLID = LoadTexture("paddle.png");
    paddleRID = LoadTexture("paddle.png");
    fontTextureID = LoadTexture("font1.png");

    paddleRPosition.x = 4.75f;
    paddleLPosition.x = -4.75f;

}

void ProcessInput() {
    SDL_Event event;

    paddleLMovement = glm::vec3(0);
    paddleRMovement = glm::vec3(0);

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (!over) { //only let player move if the game is going
        if (keys[SDL_SCANCODE_W]) {
            if (paddleLPosition.y < 5.0f) {
                paddleLMovement.y = 1.0f;
            } else {
                paddleLMovement.y = 0.0f;
            }
        }
        if (keys[SDL_SCANCODE_S]) {
            paddleLMovement.y = -1.0f;
        }
        if (keys[SDL_SCANCODE_UP]) {
            paddleRMovement.y = 1.0f;
        }
        if (keys[SDL_SCANCODE_DOWN]) {
            paddleRMovement.y = -1.0f;
        }
        if (glm::length(paddleLMovement) > 1.0f) {
            paddleLMovement = glm::normalize(paddleLMovement);
        }
        if (glm::length(paddleRMovement) > 1.0f) {
            paddleRMovement = glm::normalize(paddleRMovement);
        }
    }
    else {
        if (keys[SDL_SCANCODE_SPACE]) { //hit space to restart
            over = false;
            paddleRPosition.y = 0;
            paddleLPosition.y = 0;
            ballPosition.x = 0.0f;
            ballPosition.y = 0.0f;
            ballMovement = normalize(glm::vec3(1, 1, 0)); 
        }
    }

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            gameIsRunning = false;
        }
    }
}

void gameOver(int winner) {
    if (!over && winner == 1) {
        player1Score += 1;
    }
    else if (!over && winner == 2) {
        player2Score += 1;
    }
    over = true;
    ballMovement = glm::vec3(0);
}


void moveBall() {
    //checking for left paddle
    float xdistL = fabs((float)ballPosition.x - (float)paddleLPosition.x) - ((ballW + paddleW) / 2.0f);
    float ydistL = fabs((float)ballPosition.y - (float)paddleLPosition.y) - ((ballH + paddleH) / 2.0f);

    //checking for right paddle
    float xdistR = fabs((float)ballPosition.x - (float)paddleRPosition.x) - ((ballW + paddleW) / 2.0f);
    float ydistR = fabs((float)ballPosition.y - (float)paddleRPosition.y) - ((ballH + paddleH) / 2.0f);


    if (xdistL < 0 && ydistL < 0) {
        ballMovement.x = 1;
    } else if (xdistR < 0 && ydistR < 0) {
        ballMovement.x = -1;
    }

    //end if hits the wall
    if (ballPosition.x + ballW / 2.0f <= -4.5) {
        gameOver(2);
    }
    else if (ballPosition.x - ballW / 2.0f >= 4.5) {
        gameOver(1);
    }

    //bounce of top and bottom
    if (ballPosition.y + ballH / 2.0f >= 3.75f) {
        ballMovement.y = -1;
    }
    if (ballPosition.y - ballH / 2.0f <= -3.75f) {
        ballMovement.y = 1;
    }
}

void movePaddle() {
    //left paddle stay on screen
    if (paddleLPosition.y + paddleH / 2.0f >= 4.0f) {
        paddleLPosition.y = 4.0f - paddleH / 2.0f;
    }
    else if (paddleLPosition.y - paddleH / 2.0f <= -4.0f) {
        paddleLPosition.y = -4.0f + paddleH / 2.0f;
    }
    //right paddle stay on screen
    if (paddleRPosition.y + paddleH / 2.0f >= 4.0f) {
        paddleRPosition.y = 4.0f - paddleH / 2.0f;
    }
    else if (paddleRPosition.y - paddleH / 2.0f <= -4.0f) {
        paddleRPosition.y = -4.0f + paddleH / 2.0f;
    }
}


float lastTicks = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    moveBall();
    movePaddle();

    ballPosition += ballMovement * ballSpeed * deltaTime;
    paddleLPosition += paddleLMovement * paddleLSpeed * deltaTime;
    paddleRPosition += paddleRMovement * paddleRSpeed * deltaTime;

    paddleLMatrix = glm::mat4(1.0f);
    paddleRMatrix = glm::mat4(1.0f);
    ballMatrix = glm::mat4(1.0f);

    ballMatrix = glm::translate(ballMatrix, ballPosition);
    ballMatrix = glm::scale(ballMatrix, glm::vec3(0.5f, 0.5f, 1.0f)); 

    paddleLMatrix = glm::translate(paddleLMatrix, paddleLPosition);
    paddleLMatrix = glm::scale(paddleLMatrix, glm::vec3(1.5f, 1.5f, 1.0f));

    paddleRMatrix = glm::translate(paddleRMatrix, paddleRPosition);
    paddleRMatrix = glm::scale(paddleRMatrix, glm::vec3(1.5f, 1.5f, 1.0f));
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);

    program.SetModelMatrix(ballMatrix);
    glBindTexture(GL_TEXTURE_2D, ballID);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    program.SetModelMatrix(paddleLMatrix);
    glBindTexture(GL_TEXTURE_2D, paddleLID);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    program.SetModelMatrix(paddleRMatrix);
    glBindTexture(GL_TEXTURE_2D, paddleRID);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);

  
    DrawText(&program, fontTextureID, "Player 1: " + std::to_string(player1Score), 0.5f, -0.3f, glm::vec3(-4.75f, 3.5f, 0));
    DrawText(&program, fontTextureID, "Player 2: " + std::to_string(player2Score), 0.5f, -0.3f, glm::vec3(2.5f, 3.5f, 0));
    if(over){
        DrawText(&program, fontTextureID, "Game Over!", 0.5f, -0.25f, glm::vec3(-1.0f, 0.25f, 0));
        DrawText(&program, fontTextureID, "Hit Space to Play Again", 0.5f, -0.25f, glm::vec3(-2.4f, -0.25f, 0));
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
