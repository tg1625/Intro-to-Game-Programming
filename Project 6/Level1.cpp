#include "Level1.h"
#define LEVEL1_WIDTH 15
#define LEVEL1_HEIGHT 15
#define LEVEL1_ENEMY_COUNT 1
unsigned int level1_map_data[] =
{
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5,
    5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5,
    5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5,
    5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5,
    5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5,
    5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5,
    5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5,
    5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5,
    5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5,
    5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5,
    5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5,
    5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5,
    5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
};

unsigned int level1_object_data[] =
{
    26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 
    26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
    26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
    26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
    26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
    26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
    26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
    26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
    26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
    26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
    26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
    26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
    26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
    26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
    26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
};

float timer, startTime;
int hits;

void Level1::Initialize() {
    state.nextScene = -1;
    state.over = false;                                                 
    //Player Tings
    state.player = new Entity();
    state.player->entityType = EntityType::PLAYER;
    state.player->width = 1.0f;
    state.player->height = 1.0f;
    state.player->jumpPower = 5.0f;
    state.player->textureID = Util::LoadTexture("sara.png");
    state.player->animCols = 5;
    state.player->animRows = 3;
    state.player->animIndices = new int[2]{ 1, 3 };
    state.player->animFrames = 2;
    state.player->animIndex = 0;
    state.player->position = glm::vec3(1.0f, -3.0f, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, 0, 0);
    state.player->velocity = glm::vec3(0);
    state.player->speed = 2.0f;

    ////Enemy Tings
    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
  
    ////Mouse Tingz
    state.enemies[0].entityType = EntityType::ENEMY;
    state.enemies[0].aiType = DAD;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].width = 0.9f;
    state.enemies[0].height = 0.9f;
    state.enemies[0].textureID = Util::LoadTexture("father.png");
    state.enemies[0].animCols = 5;
    state.enemies[0].animRows = 3;
    state.enemies[0].animIndices = new int[2]{ 1, 3 };
    state.enemies[0].animFrames = 2;
    state.enemies[0].animIndex = 0;
    state.enemies[0].position = glm::vec3(1.5f, -6.0f, 0);
    state.enemies[0].movement = glm::vec3(0);
    state.enemies[0].acceleration = glm::vec3(0, 0, 0);
    state.enemies[0].velocity = glm::vec3(0);
    state.enemies[0].speed = 1.0f;

    //Can Tingz
    state.can = new Entity();
    state.can->entityType = EntityType::CAN;
    state.can->textureID = Util::LoadTexture("can.png");
    state.can->position = glm::vec3(10.5f, -12.0f, 0);

    //Map  Tingz
    GLuint mapTextureID = Util::LoadTexture("tilesheet_complete.png");
    state.tilemap = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_object_data, mapTextureID, 1.0f, 27, 20);

    //mapTextureID = Util::LoadTexture("tileset2.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_map_data, mapTextureID, 1.0f, 27, 20);

    //Timer things
    startTime = (float)SDL_GetTicks() / 1000.0f;
    hits = 0;
}
glm::mat4 Level1::Update(float deltaTime, glm::mat4 viewMatrix) {
    //Games over if time runs out
    if (60.0 - (timer - startTime) - (hits * 10) <= 0) {
        state.player->isLost = true;
        state.nextScene = 2;
    }
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.map, state.enemies, LEVEL1_ENEMY_COUNT);
    }

    state.can->Update(deltaTime, state.player, state.map, state.enemies, LEVEL1_ENEMY_COUNT);
    
	state.player->Update(deltaTime, state.player, state.map, state.enemies, LEVEL1_ENEMY_COUNT);

    if (state.player->isHit) {
        hits++;
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
    timer = (float)SDL_GetTicks() / 1000.0f;
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

    
    
}

void Level1::RenderUI(ShaderProgram* program) {
    GLuint fontTextureID = Util::LoadTexture("font1.png");
    Util::DrawText(program, fontTextureID, "Time before starvation: " + std::to_string((int)(60.0 - (timer - startTime) - (hits*10))), 0.5f, -0.25f,
        glm::vec3(-3.0f, 3.5f, 0));
}