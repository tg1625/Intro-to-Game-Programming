#include "Level2.h"
#define LEVEL2_WIDTH 15
#define LEVEL2_HEIGHT 15
#define LEVEL2_ENEMY_COUNT 7
unsigned int level2_map_data[] =
{
    3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3, 3,
    3, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 3,
    3, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 3,
    3, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 3,
    3, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 3,
    3, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 3,
    3, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 3,
    3, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 3,
    3, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 3,
    3, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 3,
    3, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 3,
    3, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 3,
    3, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 3,
    3, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 3,
    3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3, 3,
};

unsigned int level2_object_data[] =
{
    520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520,
    493,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26, 492,
    493,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26, 492,
    493, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523,  26,  26,  26, 492,
    493,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26, 492,
    493,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26, 492,
    493,  26,  26,  26,  26, 524, 524, 524,  26,  26,  26,  26, 524, 524, 492,
    493,  26,  26,  26,  26, 496,  26, 497,  26,  26,  26,  26,  26,  26, 492,
    493,  26,  26,  26,  26, 496,  26, 497,  26,  26,  26,  26,  26,  26, 492,
    493,  26,  26,  26,  26, 496,  26, 497, 524, 524, 524,  26,  26,  26, 492,
    493,  26,  26,  26,  26, 496,  26, 497,  26,  26,  26,  26,  26,  26, 492,
    493,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26, 492,
    493,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26, 492,
    493,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26, 492,
    519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519
};

void Level2::Initialize() {
    state.nextScene = -1;
    state.over = false;
    //Player Tings
    state.player = new Entity();
    state.player->entityType = EntityType::PLAYER;
    state.player->width = 0.7f;
    state.player->height = 0.7f;
    state.player->jumpPower = 5.0f;
    state.player->textureID = Util::LoadTexture("sara.png");
    state.player->animCols = 5;
    state.player->animRows = 3;
    //state.player->animIndices = new int[2]{ 1, 3 };
    //state.player->animFrames = 2;
    //state.player->animIndex = 0;
    state.player->position = glm::vec3(10.0f, -1.5f, 0);
    state.player->startingPosition = glm::vec3(3.0f, -1.5f, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, 0, 0);
    state.player->velocity = glm::vec3(0);
    state.player->speed = 2.0f;

    ////Enemy Tings
    state.enemies = new Entity[LEVEL2_ENEMY_COUNT];

    ////Dad
    state.enemies[0].entityType = EntityType::ENEMY;
    state.enemies[0].aiType = DAD;
    state.enemies[0].aiState = PATROLLINGX;
    state.enemies[0].width = 0.9f;
    state.enemies[0].height = 0.9f;
    state.enemies[0].textureID = Util::LoadTexture("father.png");
    state.enemies[0].animCols = 5;
    state.enemies[0].animRows = 3;
    state.enemies[0].position = glm::vec3(13.0f, -2.5f, 0);
    state.enemies[0].startingPosition = glm::vec3(13.0f, -2.5f, 0);
    state.enemies[0].movement = glm::vec3(0);
    state.enemies[0].acceleration = glm::vec3(0, 0, 0);
    state.enemies[0].velocity = glm::vec3(0);
    state.enemies[0].speed = 1.0f;

    state.enemies[1].entityType = EntityType::ENEMY;
    state.enemies[1].aiType = DAD;
    state.enemies[1].aiState = PATROLLINGX;
    state.enemies[1].width = 0.9f;
    state.enemies[1].height = 0.9f;
    state.enemies[1].textureID = Util::LoadTexture("father.png");
    state.enemies[1].animCols = 5;
    state.enemies[1].animRows = 3;
    state.enemies[1].position = glm::vec3(9.0f, -6.5f, 0);
    state.enemies[1].startingPosition = glm::vec3(9.0f, -6.5f, 0);
    state.enemies[1].movement = glm::vec3(0);
    state.enemies[1].acceleration = glm::vec3(0, 0, 0);
    state.enemies[1].velocity = glm::vec3(0);
    state.enemies[1].speed = 1.2f;

    state.enemies[2].entityType = EntityType::ENEMY;
    state.enemies[2].aiType = DAD;
    state.enemies[2].aiState = PATROLLINGX;
    state.enemies[2].width = 0.9f;
    state.enemies[2].height = 0.9f;
    state.enemies[2].textureID = Util::LoadTexture("father.png");
    state.enemies[2].animCols = 5;
    state.enemies[2].animRows = 3;
    state.enemies[2].position = glm::vec3(13.0f, -9.5f, 0);
    state.enemies[2].startingPosition = glm::vec3(13.0f, -9.5f, 0);
    state.enemies[2].movement = glm::vec3(0);
    state.enemies[2].acceleration = glm::vec3(0, 0, 0);
    state.enemies[2].velocity = glm::vec3(0);
    state.enemies[2].speed = 1.0f;

    state.enemies[3].entityType = EntityType::ENEMY;
    state.enemies[3].aiType = DAD;
    state.enemies[3].aiState = PATROLLINGX;
    state.enemies[3].width = 0.9f;
    state.enemies[3].height = 0.9f;
    state.enemies[3].textureID = Util::LoadTexture("father.png");
    state.enemies[3].animCols = 5;
    state.enemies[3].animRows = 3;
    state.enemies[3].position = glm::vec3(1.0f, -6.5f, 0);
    state.enemies[3].startingPosition = glm::vec3(1.0f, -6.5f, 0);
    state.enemies[3].movement = glm::vec3(0);
    state.enemies[3].acceleration = glm::vec3(0, 0, 0);
    state.enemies[3].velocity = glm::vec3(0);
    state.enemies[3].speed = 1.0f;

    state.enemies[4].entityType = EntityType::ENEMY;
    state.enemies[4].aiType = DAD;
    state.enemies[4].aiState = PATROLLINGX;
    state.enemies[4].width = 0.9f;
    state.enemies[4].height = 0.9f;
    state.enemies[4].textureID = Util::LoadTexture("father.png");
    state.enemies[4].animCols = 5;
    state.enemies[4].animRows = 3;
    state.enemies[4].position = glm::vec3(4.0f, -9.5f, 0);
    state.enemies[4].startingPosition = glm::vec3(4.0f, -9.5f, 0);
    state.enemies[4].movement = glm::vec3(0);
    state.enemies[4].acceleration = glm::vec3(0, 0, 0);
    state.enemies[4].velocity = glm::vec3(0);
    state.enemies[4].speed = 1.2f;


    state.enemies[5].entityType = EntityType::ENEMY;
    state.enemies[5].aiType = DAD;
    state.enemies[5].aiState = PATROLLINGY;
    state.enemies[5].width = 0.9f;
    state.enemies[5].height = 0.9f;
    state.enemies[5].textureID = Util::LoadTexture("father.png");
    state.enemies[5].animCols = 5;
    state.enemies[5].animRows = 3;
    state.enemies[5].position = glm::vec3(7.5f, -13.0f, 0);
    state.enemies[5].startingPosition = glm::vec3(7.5f, -13.0f, 0);
    state.enemies[5].movement = glm::vec3(0);
    state.enemies[5].acceleration = glm::vec3(0, 0, 0);
    state.enemies[5].velocity = glm::vec3(0);
    state.enemies[5].speed = 1.2f;

    state.enemies[6].entityType = EntityType::ENEMY;
    state.enemies[6].aiType = DAD;
    state.enemies[6].aiState = PATROLLINGY;
    state.enemies[6].width = 0.9f;
    state.enemies[6].height = 0.9f;
    state.enemies[6].textureID = Util::LoadTexture("father.png");
    state.enemies[6].animCols = 5;
    state.enemies[6].animRows = 3;
    state.enemies[6].position = glm::vec3(4.5f, -11.0f, 0);
    state.enemies[6].startingPosition = glm::vec3(4.5f, -11.0f, 0);
    state.enemies[6].movement = glm::vec3(0);
    state.enemies[6].acceleration = glm::vec3(0, 0, 0);
    state.enemies[6].velocity = glm::vec3(0);
    state.enemies[6].speed = 1.2f;

    //Can Tingz
    state.can = new Entity();
    state.can->entityType = EntityType::CAN;
    state.can->textureID = Util::LoadTexture("bean.png");
    state.can->position = glm::vec3(1.5f, -1.5f, 0);

    //Opener
    state.opener = new Entity();
    state.opener->entityType = EntityType::OPENER;
    state.opener->textureID = Util::LoadTexture("opener2.png");
    state.opener->position = glm::vec3(6.0f, -8.0, 0);

    //Map  Tingz
    GLuint mapTextureID = Util::LoadTexture("tilesheet_complete.png");
    state.tilemap = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_object_data, mapTextureID, 1.0f, 27, 20); //object map
    state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_map_data, mapTextureID, 1.0f, 27, 20); //background map

    //Timer things
    state.startTime = (float)SDL_GetTicks() / 1000.0f;
    state.hits = 0;
}
glm::mat4 Level2::Update(float deltaTime, glm::mat4 viewMatrix) {
    //Games over if time runs out
    if ((int)(75.0 - (state.timer - state.startTime) - (state.hits * 10)) <= 0) {
        state.player->isLost = true;
        state.nextScene = 4;
    }
    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.tilemap, state.enemies, LEVEL2_ENEMY_COUNT, state.can, state.opener);
    }

    state.can->Update(deltaTime, state.player, state.tilemap, state.enemies, LEVEL2_ENEMY_COUNT, state.can, state.opener);
    state.opener->Update(deltaTime, state.player, state.tilemap, state.enemies, LEVEL2_ENEMY_COUNT, state.can, state.opener);
    state.player->Update(deltaTime, state.player, state.tilemap, state.enemies, LEVEL2_ENEMY_COUNT, state.can, state.opener);

    //Increase hits if player's been hit
    if (state.player->isHit) {
        state.hits++;
    }
    //Player wins if they have the can{
    if (state.player->isWon) {
        state.nextScene = 3;
    }

    //Camera Things
    viewMatrix = glm::mat4(1.0f);
    //X camera 
    if (state.player->position.x > 9) {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-9, 0, 0));
    }
    else if (state.player->position.x > 5) {
        viewMatrix = glm::translate(viewMatrix,
            glm::vec3(-state.player->position.x, 0, 0));
    }
    else {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 0, 0));
    }
    //Y camera
    if (state.player->position.y < -10.0) {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(0, 10, 0));
    }
    else if (state.player->position.y < -3.75) {
        viewMatrix = glm::translate(viewMatrix,
            glm::vec3(0, -state.player->position.y, 0));
    }
    else {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(0, 3.75, 0));
    }

    //Update timer
    state.timer = (float)SDL_GetTicks() / 1000.0f;
    return viewMatrix;
}
void Level2::Render(ShaderProgram* program) {
    state.map->Render(program);
    state.tilemap->Render(program);
    state.player->Render(program);

    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }

    state.can->Render(program);
    state.opener->Render(program);

}

void Level2::RenderUI(ShaderProgram* program) {
    GLuint fontTextureID = Util::LoadTexture("font1.png");
    Util::DrawText(program, fontTextureID, "Time before starvation: " + std::to_string((int)(75.0 - (state.timer - state.startTime) - (state.hits * 10))), 0.5f, -0.25f,
        glm::vec3(-3.0f, 3.5f, 0));
}