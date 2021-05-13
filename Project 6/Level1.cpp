#include "Level1.h"
#define LEVEL1_WIDTH 15
#define LEVEL1_HEIGHT 15
#define LEVEL1_ENEMY_COUNT 3
unsigned int level1_map_data[] =
{
    3, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 3,
    3,369,371, 95, 95,132,131, 95, 95, 95, 95, 95, 95, 95, 3,
    3,396,398, 95, 95, 95, 95, 95, 95, 95, 95,345,347, 95, 3,
    3,396,398, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 3,
    3,423,425, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 3,
    3, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 3,
    3, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 3,
    3, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 3,
    3, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 3,
    3, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 3,
    3, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 3,
    3, 95,369,370,371, 95, 95, 95, 95, 95, 95, 95, 95, 95, 3,
    3, 95,396,397,398, 95, 95, 95, 95, 95, 95, 95, 95, 95, 3,
    3, 95,423,424,425, 95, 95, 95, 95, 95, 95, 95, 95, 95, 3,
    3, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 3

};

unsigned int level1_object_data[] =
{
    466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466,
    439,  26,  26, 444, 133,  26,  26, 133, 444, 320, 321, 322, 323,  26, 438,
    437,  26,  26, 444,  26,  26,  26,  26, 444,  26,  26,  26,  26,  26, 438,
    439,  26,  26, 444,  26,  26,  26,  26, 444,  26,  26,  26,  26,  26, 438,
    439,  26,  26, 444,  26,  26,  26,  26, 444,  26, 429, 430, 431,  26, 438,
    439,  26,  26,  26,  26,  26,  26,  26, 444,  26, 529,  26, 529,  26, 438,
    491, 469, 469, 469,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26, 438,
    439,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26, 438,
    439,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26, 438,
    439,  26,  26,  26,  26,  26,  26,  26, 469, 469, 469, 469, 469, 469, 438,
    439,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26, 438,
    491,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  48,  49, 438,
    439,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  75,  76, 438,
    439,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,  26, 102, 103, 438,
    465, 465, 465, 465, 465, 465, 465, 465, 465, 465, 465, 465, 465, 465, 465
};

void Level1::Initialize() {
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
    state.player->position = glm::vec3(1.0f, -1.0f, 0);
    state.player->startingPosition = glm::vec3(1.0f, -1.0f, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, 0, 0);
    state.player->velocity = glm::vec3(0);
    state.player->speed = 2.0f;

    ////Enemy Tings
    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
  
    ////Dad
    state.enemies[0].entityType = EntityType::ENEMY;
    state.enemies[0].aiType = DAD;
    state.enemies[0].aiState = PATROLLINGX;
    state.enemies[0].width = 0.9f;
    state.enemies[0].height = 0.9f;
    state.enemies[0].textureID = Util::LoadTexture("father.png");
    state.enemies[0].animCols = 5;
    state.enemies[0].animRows = 3;
    state.enemies[0].animIndices = new int[2]{ 1, 3 };
    state.enemies[0].animFrames = 2;
    state.enemies[0].animIndex = 0;
    state.enemies[0].position = glm::vec3(2.0f, -7.0f, 0);
    state.enemies[0].startingPosition = glm::vec3(1.5f, -9.0f, 0);
    state.enemies[0].movement = glm::vec3(0);
    state.enemies[0].acceleration = glm::vec3(0, 0, 0);
    state.enemies[0].velocity = glm::vec3(0);
    state.enemies[0].speed = 1.0f;

    state.enemies[1].entityType = EntityType::ENEMY;
    state.enemies[1].aiType = DAD;
    state.enemies[1].aiState = IDLE;
    state.enemies[1].width = 0.9f;
    state.enemies[1].height = 0.9f;
    state.enemies[1].textureID = Util::LoadTexture("father.png");
    state.enemies[1].animCols = 5;
    state.enemies[1].animRows = 3;
    state.enemies[1].animIndices = new int[2]{ 1, 3 };
    state.enemies[1].animFrames = 2;
    state.enemies[1].animIndex = 0;
    state.enemies[1].position = glm::vec3(5.0f, -2.0f, 0);
    state.enemies[1].startingPosition = glm::vec3(5.0f, -2.0f, 0);
    state.enemies[1].movement = glm::vec3(0);
    state.enemies[1].acceleration = glm::vec3(0, 0, 0);
    state.enemies[1].velocity = glm::vec3(0);
    state.enemies[1].speed = 1.0f;

    state.enemies[2].entityType = EntityType::ENEMY;
    state.enemies[2].aiType = DAD;
    state.enemies[2].aiState = PATROLLINGY;
    state.enemies[2].width = 0.9f;
    state.enemies[2].height = 0.9f;
    state.enemies[2].textureID = Util::LoadTexture("father.png");
    state.enemies[2].animCols = 5;
    state.enemies[2].animRows = 3;
    state.enemies[2].animIndices = new int[2]{ 1, 3 };
    state.enemies[2].animFrames = 2;
    state.enemies[2].animIndex = 0;
    state.enemies[2].position = glm::vec3(8.0f, -8.0f, 0);
    state.enemies[2].startingPosition = glm::vec3(8.0f, -8.0f, 0);
    state.enemies[2].movement = glm::vec3(0);
    state.enemies[2].acceleration = glm::vec3(0, 0, 0);
    state.enemies[2].velocity = glm::vec3(0);
    state.enemies[2].speed = 1.0f;
    

    //Can Tingz
    state.can = new Entity();
    state.can->entityType = EntityType::CAN;
    state.can->textureID = Util::LoadTexture("bean.png");
    state.can->position = glm::vec3(13.0f, -1.25f, 0);

    //Opener
    state.opener = new Entity();
    state.opener->entityType = EntityType::OPENER;
    state.opener->textureID = Util::LoadTexture("opener2.png");
    state.opener->position = glm::vec3(1.0f, -13.0, 0);

    //Map  Tingz
    GLuint mapTextureID = Util::LoadTexture("tilesheet_complete.png");
    state.tilemap = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_object_data, mapTextureID, 1.0f, 27, 20); //object map
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_map_data, mapTextureID, 1.0f, 27, 20); //background map

    //Timer things
    state.startTime = (float)SDL_GetTicks() / 1000.0f;
    state.hits = 0;
}
glm::mat4 Level1::Update(float deltaTime, glm::mat4 viewMatrix) {
    //Games over if time runs out
    if ((int)(60.0 - (state.timer - state.startTime) - (state.hits * 10)) <= 0) {
        state.player->isLost = true;
        state.nextScene = 4;
    }
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.tilemap, state.enemies, LEVEL1_ENEMY_COUNT, state.can, state.opener);
    }

    state.can->Update(deltaTime, state.player, state.tilemap, state.enemies, LEVEL1_ENEMY_COUNT, state.can, state.opener);
    state.opener->Update(deltaTime, state.player, state.tilemap, state.enemies, LEVEL1_ENEMY_COUNT, state.can, state.opener);
	state.player->Update(deltaTime, state.player, state.tilemap, state.enemies, LEVEL1_ENEMY_COUNT, state.can, state.opener);

    //Increase hits if player's been hit
    if (state.player->isHit) {
        state.hits++;
    }
    //Player wins if they have the can{
    if (state.player->isWon) {
        state.nextScene = 2;
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
void Level1::Render(ShaderProgram* program) {
    state.map->Render(program);
    state.tilemap->Render(program);
	state.player->Render(program);

    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }

    state.can->Render(program);
    state.opener->Render(program);

    
    
}

void Level1::RenderUI(ShaderProgram* program) {
    GLuint fontTextureID = Util::LoadTexture("font1.png");
    Util::DrawText(program, fontTextureID, "Time before starvation: " + std::to_string((int)(60.0 - (state.timer - state.startTime) - (state.hits*10))), 0.5f, -0.25f,
        glm::vec3(-3.0f, 3.5f, 0));
}