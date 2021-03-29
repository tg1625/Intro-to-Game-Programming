#pragma once
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

enum EntityType { PLAYER, PLATFORM, ENEMY };

enum AIType { BAT, BLOCKY, MOUSE };
enum AIState { ATTACKING, PATROLLING, FLYING, IDLE };

class Entity {
public:
    
    enum EntityType entityType;

    AIType aiType;
    AIState aiState;

    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 acceleration;
    glm::vec3 velocity;
    float speed;

    float width = 1;
    float height = 1;

    bool jump = false;
    float jumpPower = 0;

    GLuint textureID;

    glm::mat4 modelMatrix;

    int* animRight = NULL;
    int* animLeft = NULL;
    int* animUp = NULL;
    int* animDown = NULL;

    int* animIndices = NULL;
    int animFrames = 0;
    int animIndex = 0;
    float animTime = 0;
    int animCols = 0;
    int animRows = 0;

    bool isActive = true;
    bool isLost = false;
    bool isWon = false;

    bool collidedTop = false;
    bool collidedBottom = false;
    bool collidedLeft = false;
    bool collidedRight = false;

    bool forward = true;

    Entity();

    bool CheckCollision(Entity* other);

    void CheckCollisionsY(Entity* objects, int objectCount);
    void CheckCollisionsX(Entity* objects, int objectCount);

    void AI(Entity* player, float deltaTime);
    void AIBat(Entity* player);
    void AIBlocky(Entity* player);
    void AIMouse(Entity* player);

    void Animate(float deltaTime);

    void Update(float deltaTime, Entity* player, Entity* platforms, int platformCount, Entity* enemies, int enemycount);
    void Render(ShaderProgram* program);
    void DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index);
};
