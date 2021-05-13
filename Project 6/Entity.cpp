#include "Entity.h"
#include <cmath>

Entity::Entity()
{
    position = glm::vec3(0);
    movement = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    speed = 0;
    modelMatrix = glm::mat4(1.0f);
}

void Entity::AI(Entity* player, float deltaTime) {
    switch (aiType) {
        case DAD:
            //Animate(deltaTime);
            AIDad(player);
            break;
    }
}

void Entity::AIDad(Entity* player) {
    switch (aiState){
        case IDLE:
            movement = glm::vec3(0, 0, 0);
            animIndices = new int[1]{ 4 };
            animFrames = 1;
            if (glm::distance(position, player->position) < 3.5f) {
                aiState = ATTACKING;
                player->wasSeen = true;
            }
            break;
        case ATTACKING:
            movement = glm::vec3(0, 0, 0);
            if (player->position.x > position.x) {
                movement.x = 1.0;
                animIndices = new int[3]{ 1, 6, 11 };
                animFrames = 3;
            }
            else{
                movement.x = -1.0;
                animIndices = new int[3]{ 3, 8, 13 };
                animFrames = 3;
            }
            if (player->position.y < position.y) {
                movement.y = -1.0;
                animIndices = new int[3]{ 2, 7, 12 };
                animFrames = 3;
            }
            else {
                movement.y = 1.0;
                animIndices = new int[3]{ 0, 5, 10 };
                animFrames = 3;
            }
            player->wasSeen = false;
            if (glm::length(movement) > 1.0f) movement = glm::normalize(movement);  
            break;
        case PATROLLINGX:
            movement = glm::vec3(0, 0, 0);
            if (position.x - startingPosition.x > 2 || collidedRight) {
                forward = false;
            }
            else if (position.x - startingPosition.x < -2 || collidedLeft) {
                forward = true;
            }
            if (forward == true) {
                movement = glm::vec3(1.0, 0.0f, 0);
                animIndices = new int[3]{ 1, 6, 11 };
                animFrames = 3;
            }
            else {
                movement = glm::vec3(-1.0, 0.0f, 0);
                animIndices = new int[3]{ 3, 8, 13 };
                animFrames = 3;
            }
            if (glm::length(movement) > 1.0f) movement = glm::normalize(movement);
            break;
        case PATROLLINGY:
            movement = glm::vec3(0, 0, 0);
            if (position.y - startingPosition.y > 2 || collidedTop) {
                forward = false;
            }
            else if (position.y - startingPosition.y < -2 || collidedBottom) {
                forward = true;
            }
            if (forward == true) {
                movement = glm::vec3(0.0, 1.0f, 0);
                animIndices = new int[3]{ 0, 5, 10 };
                animFrames = 3;
            }
            else {
                movement = glm::vec3(0.0, -1.0f, 0);
                animIndices = new int[3]{ 2, 7, 12 };
                animFrames = 3;
            }
            if (glm::length(movement) > 1.0f) movement = glm::normalize(movement);
            break;
    }
    
}

void Entity::Animate(float deltaTime) {
    animTime += deltaTime;
    if (animTime >= 0.25f)
    {
        animTime = 0.0f;
        animIndex++;
        if (animIndex >= animFrames)
        {
            animIndex = 0;
        }
    }
}

void Entity::Update(float deltaTime,Entity* player, Map* map, Entity* enemies, int enemyCount, Entity* can, Entity* opener)
{
    if (!isActive) return;
    isHit = false;

    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;

    //Update Enemy
    if (entityType == ENEMY) {
        velocity.x = movement.x * speed;
        velocity.y = movement.y * speed;

        position.y += velocity.y * deltaTime; 
        CheckCollisionsY(map);

        position.x += velocity.x * deltaTime;     
        CheckCollisionsX(map);
        Animate(deltaTime);
        AI(player, deltaTime);
        
    }

    //Update Player
    if (entityType == PLAYER && !isHit && !isWon) {
        Animate(deltaTime);
        //Check Opener collisions
        if (CheckCollision(opener) && !hasOpener) {
            hasOpener = true;
            opener->isActive = false; 
        }
        //Check Can collisions
        if (CheckCollision(can) && hasOpener){
            isWon = true;
        }
        //Movement Things
        velocity.x = movement.x * speed;
        velocity.y = movement.y * speed;
        velocity += acceleration * deltaTime;

        position.y += velocity.y * deltaTime;
        CheckCollisionsY(map);
        if (!isInvincible) {
            //SDL_Log("Checking Cols Y");
            CheckCollisionsY(enemies, enemyCount);
        }

        position.x += velocity.x * deltaTime; 
        CheckCollisionsX(map);
        if (!isInvincible) {
            //SDL_Log("Checking Cols X");
            CheckCollisionsX(enemies, enemyCount);
        }

        //if hit, go back to starting position
        if (isHit) {
            isInvincible = true;
            //SDL_Log("Invincible now: %d",isInvincible);
        }

        SDL_Log("Position is %f , %f", position.x, position.y);
    }

    //Update Cab

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
}


bool Entity::CheckCollision(Entity* other) {
    if (other == this) return false; 

    if (!isActive || !other->isActive) return false;

    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);

    if (xdist < 0 && ydist < 0) return true;

    return false;
}

void Entity::CheckCollisionsY(Entity* objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++){
        Entity* object = &objects[i];
        if (CheckCollision(object)){
            float ydist = fabs(position.y - object->position.y);
            float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
            if (velocity.y > 0) {
                position.y -= penetrationY;
                velocity.y = 0;
                collidedTop = true;
                if (!isWon && !isHit) {
                    if (object->entityType == EntityType::ENEMY) {
                        isHit = true;
                        animIndex = 1;
                    }
                }
            }
            else if (velocity.y < 0) {
                position.y += penetrationY;
                velocity.y = 0;
                collidedBottom = true;
                if (!isWon && !isHit) {
                    if (object->entityType == EntityType::ENEMY) {
                        isHit = true;
                        animIndex = 1;
                    }
                }
            }
        }
    }
}

void Entity::CheckCollisionsY(Map* map)
{
    // Probes for tiles
    glm::vec3 top = glm::vec3(position.x, position.y + (height / 2), position.z);
    glm::vec3 top_left = glm::vec3(position.x - (width / 2), position.y + (height / 2), position.z);
    glm::vec3 top_right = glm::vec3(position.x + (width / 2), position.y + (height / 2), position.z);

    glm::vec3 bottom = glm::vec3(position.x, position.y - (height / 2), position.z);
    glm::vec3 bottom_left = glm::vec3(position.x - (width / 2), position.y - (height / 2), position.z);
    glm::vec3 bottom_right = glm::vec3(position.x + (width / 2), position.y - (height / 2), position.z);

    float penetration_x = 0;
    float penetration_y = 0;
    if (map->IsSolid(top, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    else if (map->IsSolid(top_left, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    else if (map->IsSolid(top_right, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    if (map->IsSolid(bottom, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
    }
    else if (map->IsSolid(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
    }
    else if (map->IsSolid(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
    }
}

void Entity::CheckCollisionsX(Entity* objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++){
        Entity* object = &objects[i];
        if (CheckCollision(object)){
            if (!isWon && !isHit) {
                if (object->entityType == EntityType::ENEMY) {
                    isHit = true;
                }
            }
            float xdist = fabs(position.x - object->position.x);
            float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
            if (velocity.x > 0) {
                position.x -= penetrationX;
                velocity.x = 0;
                collidedRight = true;
            }
            else if (velocity.x < 0) {
                position.x += penetrationX;
                velocity.x = 0;
                collidedLeft = true;
            }
        }
    }
}

void Entity::CheckCollisionsX(Map* map)
{
    // Probes for tiles
    glm::vec3 left = glm::vec3(position.x - (width / 2), position.y, position.z);
    glm::vec3 right = glm::vec3(position.x + (width / 2), position.y, position.z);

    float penetration_x = 0;
    float penetration_y = 0;
    if (map->IsSolid(left, &penetration_x, &penetration_y) && velocity.x < 0) {
        position.x += penetration_x;
        velocity.x = 0;
        collidedLeft = true;
    }

    if (map->IsSolid(right, &penetration_x, &penetration_y) && velocity.x > 0) {
        position.x -= penetration_x;
        velocity.x = 0;
        collidedRight = true;
    }
}

void Entity::Render(ShaderProgram* program) {
    if (!isActive) return;

    program->SetModelMatrix(modelMatrix);
    
    if (animIndices != NULL) {
        DrawSpriteFromTextureAtlas(program, textureID, animIndices[animIndex]);
        return;
    }

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

    glBindTexture(GL_TEXTURE_2D, textureID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index)
{
    float u = (float)(index % animCols) / (float)animCols;
    float v = (float)(index / animCols) / (float)animRows;

    float width = 1.0f / (float)animCols;
    float height = 1.0f / (float)animRows;

    float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
        u, v + height, u + width, v, u, v };

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };

    glBindTexture(GL_TEXTURE_2D, textureID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

