#include "Level1.h"
#define LEVEL1_WIDTH 15
#define LEVEL1_HEIGHT 8
#define LEVEL1_ENEMY_COUNT 3
unsigned int level1_data[] =
{
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 3,
 3, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3,
 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3
};

void Level1::Initialize() {
    state.over = false;                                                 
    //Player Tings
    state.player = new Entity();
    state.player->entityType = EntityType::PLAYER;
    state.player->width = 1.0f;
    state.player->height = 0.9f;
    state.player->jumpPower = 7.0f;
    state.player->textureID = Util::LoadTexture("player_spritesheet.png");
    state.player->animCols = 7;
    state.player->animRows = 3;
    state.player->animIndices = new int[2]{ 0, 6 };
    state.player->animFrames = 2;
    state.player->animIndex = 0;
    state.player->position = glm::vec3(2.0f, 0.0f, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81, 0);
    state.player->velocity = glm::vec3(0);
    state.player->speed = 2.0f;

    //Enemy Tings
    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
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
    state.enemies[0].position = glm::vec3(10.5f, -3.0f, 0);
    state.enemies[0].movement = glm::vec3(0);
    state.enemies[0].acceleration = glm::vec3(0, 0, 0);
    state.enemies[0].velocity = glm::vec3(0);
    state.enemies[0].speed = 1.0f;

    //Blocky Tingz
    state.enemies[1].entityType = EntityType::ENEMY;
    state.enemies[1].aiType = BLOCKY;
    state.enemies[1].aiState = IDLE;
    state.enemies[1].width = 1.0f;
    state.enemies[1].height = 1.0f;
    state.enemies[1].textureID = Util::LoadTexture("blocky_sheet.png");
    state.enemies[1].animCols = 2;
    state.enemies[1].animRows = 1;
    state.enemies[1].animIndices = new int[2]{ 0, 1 };
    state.enemies[1].animFrames = 2;
    state.enemies[1].animIndex = 1;
    state.enemies[1].position = glm::vec3(6.0f, -3.0f, 0);
    state.enemies[1].movement = glm::vec3(0);
    state.enemies[1].acceleration = glm::vec3(0, 0, 0);
    state.enemies[1].velocity = glm::vec3(0);
    state.enemies[1].speed = 3.0f;

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
    state.enemies[2].position = glm::vec3(3.0f, -1.25f, 0);
    state.enemies[2].movement = glm::vec3(0);
    state.enemies[2].acceleration = glm::vec3(0, -900.81, 0);
    state.enemies[2].velocity = glm::vec3(0);
    state.enemies[2].speed = 1.0f;


    //Map  Tingz
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);

    //Font Tings
    GLuint fontTextureID = Util::LoadTexture("font1.png");
}
void Level1::Update(float deltaTime) {
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.map, state.enemies, LEVEL1_ENEMY_COUNT);
    }
	state.player->Update(deltaTime, state.player, state.map, state.enemies, LEVEL1_ENEMY_COUNT);
}
void Level1::Render(ShaderProgram* program) {
	state.map->Render(program);
	state.player->Render(program);

    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }

    /*if (state.player->isWon) {
        Util::DrawText(&program, fontTextureID, "You Win!", 0.5f, -0.25f,
            glm::vec3(-2.75f, 2.75, 0));
    }
    else if (state.player->isLost) {
        Util::DrawText(&program, fontTextureID, "You Lose :(", 0.5f, -0.25f,
            glm::vec3(-2.75f, 2.75, 0));
    }
    if (over) {
        Util::DrawText(&program, fontTextureID, "Hit Enter to Play Again", 0.5f, -0.25f, glm::vec3(-2.75f, 2.0, 0));
    }*/
}