#include "Level1.h"
#define LEVEL1_WIDTH 15
#define LEVEL1_HEIGHT 8
#define LEVEL1_ENEMY_COUNT 3
unsigned int level1_data[] =
{
 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0,
 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2,
 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 1, 1, 1,
 1, 0, 0, 0, 0, 0, 2, 2, 2, 1, 1, 1, 1, 1, 1,
 1, 0, 0, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

void Level1::Initialize() {
    state.nextScene = -1;
    state.over = false;                                                 
    //Player Tings
    state.player = new Entity();
    state.player->entityType = EntityType::PLAYER;
    state.player->width = 1.0f;
    state.player->height = 0.9f;
    state.player->jumpPower = 5.0f;
    state.player->textureID = Util::LoadTexture("player_spritesheet.png");
    state.player->animCols = 7;
    state.player->animRows = 3;
    state.player->animIndices = new int[2]{ 0, 6 };
    state.player->animFrames = 2;
    state.player->animIndex = 0;
    state.player->position = glm::vec3(1.0f, -3.0f, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81, 0);
    state.player->velocity = glm::vec3(0);
    state.player->speed = 2.0f;

    //Enemy Tings
    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
  
    //Mouse Tingz
    state.enemies[0].entityType = EntityType::ENEMY;
    state.enemies[0].aiType = MOUSE;
    state.enemies[0].aiState = PATROLLING;
    state.enemies[0].width = 0.9f;
    state.enemies[0].height = 0.9f;
    state.enemies[0].textureID = Util::LoadTexture("mouse_sheet.png");
    state.enemies[0].animCols = 3;
    state.enemies[0].animRows = 1;
    state.enemies[0].animIndices = new int[2]{ 0, 2 };
    state.enemies[0].animFrames = 2;
    state.enemies[0].animIndex = 0;
    state.enemies[0].position = glm::vec3(7.5f, 0.0f, 0);
    state.enemies[0].movement = glm::vec3(0);
    state.enemies[0].acceleration = glm::vec3(0, -900.81, 0);
    state.enemies[0].velocity = glm::vec3(0);
    state.enemies[0].speed = 1.0f;

    //Mouse Tingz
    state.enemies[1].entityType = EntityType::ENEMY;
    state.enemies[1].aiType = MOUSE;
    state.enemies[1].aiState = PATROLLING;
    state.enemies[1].width = 0.9f;
    state.enemies[1].height = 0.9f;
    state.enemies[1].textureID = Util::LoadTexture("mouse_sheet.png");
    state.enemies[1].animCols = 3;
    state.enemies[1].animRows = 1;
    state.enemies[1].animIndices = new int[2]{ 0, 2 };
    state.enemies[1].animFrames = 2;
    state.enemies[1].animIndex = 0;
    state.enemies[1].position = glm::vec3(10.25f, 0.0f, 0);
    state.enemies[1].movement = glm::vec3(0);
    state.enemies[1].acceleration = glm::vec3(0, -900.81, 0);
    state.enemies[1].velocity = glm::vec3(0);
    state.enemies[1].speed = 1.0f;

    //Mouse Tingz
    state.enemies[2].entityType = EntityType::ENEMY;
    state.enemies[2].aiType = MOUSE;
    state.enemies[2].aiState = PATROLLING;
    state.enemies[2].width = 0.9f;
    state.enemies[2].height = 0.9f;
    state.enemies[2].textureID = Util::LoadTexture("mouse_sheet.png");
    state.enemies[2].animCols = 3;
    state.enemies[2].animRows = 1;
    state.enemies[2].animIndices = new int[2]{ 0, 2 };
    state.enemies[2].animFrames = 2;
    state.enemies[2].animIndex = 0;
    state.enemies[2].position = glm::vec3(12.75f, 0.0f, 0);
    state.enemies[2].movement = glm::vec3(0);
    state.enemies[2].acceleration = glm::vec3(0, -900.81, 0);
    state.enemies[2].velocity = glm::vec3(0);
    state.enemies[2].speed = 1.0f;


    //Map  Tingz
    GLuint mapTextureID = Util::LoadTexture("tileset2.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);
}
glm::mat4 Level1::Update(float deltaTime, glm::mat4 viewMatrix) {
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.map, state.enemies, LEVEL1_ENEMY_COUNT);
    }
	state.player->Update(deltaTime, state.player, state.map, state.enemies, LEVEL1_ENEMY_COUNT);

    viewMatrix = glm::mat4(1.0f);
    if (state.player->position.x > 9) {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-9, 3.75, 0));
    }
    else if (state.player->position.x > 5) {
        viewMatrix = glm::translate(viewMatrix,
            glm::vec3(-state.player->position.x, 3.75, 0));
    }
    else {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.75, 0));
    }

    if (state.player->position.x > 12.5) state.nextScene = 2;
    return viewMatrix;
}
void Level1::Render(ShaderProgram* program) {
	state.map->Render(program);
	state.player->Render(program);

    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }
}