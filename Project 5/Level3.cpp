#include "Level3.h"
#define LEVEL3_WIDTH 15
#define LEVEL3_HEIGHT 8
#define LEVEL3_ENEMY_COUNT 1
unsigned int level3_data[] =
{
 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1,
 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 3, 1,
 1, 0, 0 ,0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2, 1,
 1, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 2, 2, 1, 1,
 1, 0, 0, 0, 2, 1, 1, 0, 0, 2, 2, 1, 1, 1, 1,
 1, 2, 2, 2, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1
};

void Level3::Initialize() {
    state.nextScene = -1;
    state.over = false;
    //Player Tings
    state.player = new Entity();
    state.player->entityType = EntityType::PLAYER;
    state.player->width = 0.9f;
    state.player->height = 0.9f;
    state.player->jumpPower = 5.0f;
    state.player->textureID = Util::LoadTexture("player_spritesheet.png");
    state.player->animCols = 7;
    state.player->animRows = 3;
    state.player->animIndices = new int[2]{ 0, 6 };
    state.player->animFrames = 2;
    state.player->animIndex = 0;
    state.player->position = glm::vec3(1.0f, 1.0f, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81, 0);
    state.player->velocity = glm::vec3(0);
    state.player->speed = 2.0f;

    //Enemy Tings
    state.enemies = new Entity[LEVEL3_ENEMY_COUNT];
    //Bat Tings
    state.enemies[0].entityType = EntityType::ENEMY;
    state.enemies[0].aiType = BAT;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].width = 1.0f;
    state.enemies[0].height = 1.0f;
    state.enemies[0].textureID = Util::LoadTexture("bat_sheet.png");
    state.enemies[0].animCols = 4;
    state.enemies[0].animRows = 1;
    state.enemies[0].animIndices = new int[2]{ 0, 2 };
    state.enemies[0].animFrames = 2;
    state.enemies[0].animIndex = 0;
    state.enemies[0].position = glm::vec3(-1.0f, -4.0f, 0);
    state.enemies[0].movement = glm::vec3(0);
    state.enemies[0].acceleration = glm::vec3(0, 0, 0);
    state.enemies[0].velocity = glm::vec3(0);
    state.enemies[0].speed = 1.0f;

    //Map  Tingz
    GLuint mapTextureID = Util::LoadTexture("tileset2.png");
    state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTextureID, 1.0f, 4, 1);

    //Font Tings
    GLuint fontTextureID = Util::LoadTexture("font1.png");
}
glm::mat4 Level3::Update(float deltaTime, glm::mat4 viewMatrix) {
    for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.map, state.enemies, LEVEL3_ENEMY_COUNT);
    }
    state.player->Update(deltaTime, state.player, state.map, state.enemies, LEVEL3_ENEMY_COUNT);

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

    if (state.player->position.x > 12) state.nextScene = 5;
    return viewMatrix;
}
void Level3::Render(ShaderProgram* program) {
    state.map->Render(program);
    state.player->Render(program);

    for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }
}