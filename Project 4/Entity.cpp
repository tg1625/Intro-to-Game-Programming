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
        case BAT:
            Animate(deltaTime);
            AIBat(player);
            break;
        case BLOCKY:
            AIBlocky(player);
            break;
        case MOUSE:
            Animate(deltaTime);
            AIMouse(player);
            break;
    }
}

void Entity::AIBat(Entity* player) {
    switch (aiState){
        case IDLE:
            if (glm::distance(position, player->position) < 3.0) aiState = FLYING;
            break;
        case FLYING:
            if (player->position.x < position.x) {
                movement.x = -1.5;
                forward = true;
            }
            else {
                movement.x = 1.5;
                forward = false;
            }
            if (player->position.y - 0.5< position.y) {
                movement.y = -2.0;
            }
            else {
                movement.y = 1.0;
            }
            
    }
    
}

void Entity::AIBlocky(Entity* player) {
    switch (aiState) {
        case IDLE:
            animIndex = 1;
            if (!collidedBottom && std::abs(position.x - player->position.x) < 1.5f) {
                aiState = ATTACKING;
            }
            break;
        case ATTACKING:
            animIndex = 0;
            movement = glm::vec3(0, -1.0f, 0);
            break;
    }

}

void Entity::AIMouse(Entity *player) {
    switch (aiState) {
    case IDLE:
        break;
    case PATROLLING:
        if (forward == true) {
            movement = glm::vec3(-1.0, 0.0f, 0);
        }
        else {
            movement = glm::vec3(1.0, 0.0f, 0);
        }

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

void Entity::Update(float deltaTime,Entity* player, Entity* platforms, int platformCount, Entity* enemies, int enemyCount)
{
    if (!isActive) return;

    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;

    if (entityType == ENEMY) AI(player, deltaTime);


    //Movement tings
    if (!isLost && !isWon) {
        if (jump) {
            jump = false;
            velocity.y += jumpPower;
        }
        velocity.x = movement.x * speed;
        if(entityType != PLAYER) velocity.y = movement.y * speed;

        velocity += acceleration * deltaTime;

        position.y += velocity.y * deltaTime;
        //CheckCollisionsY(flag, 1);
        CheckCollisionsY(platforms, platformCount);
        if(entityType == PLAYER) CheckCollisionsY(enemies, enemyCount);

        position.x += velocity.x * deltaTime; 
        CheckCollisionsX(platforms, platformCount);
        if (entityType == PLAYER) CheckCollisionsX(enemies, enemyCount);
    }

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    if (forward || entityType == PLAYER) {
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 1, 1));
    }
    else {
        modelMatrix = glm::scale(modelMatrix, glm::vec3(-1, 1, 1));
    }
    
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
                if (!isWon && !isLost) {
                    if (object->entityType == EntityType::ENEMY) {
                        isLost = true;
                        animIndex = 1;
                    }
                }
            }
            else if (velocity.y < 0) {
                position.y += penetrationY;
                velocity.y = 0;
                collidedBottom = true;
                if (!isWon && !isLost) {
                    if (object->entityType == EntityType::ENEMY) object->isActive = false;
                }
            }
        }
    }
}

void Entity::CheckCollisionsX(Entity* objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++){
        Entity* object = &objects[i];
        if (CheckCollision(object)){
            if (!isWon && !isLost) {
                if (object->entityType == EntityType::ENEMY) {
                    isLost = true;
                    animIndex = 1;
                }
            }
            float xdist = fabs(position.x - object->position.x);
            float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
            if (velocity.x > 0) {
                position.x -= penetrationX;
                velocity.x = 0;
                collidedRight = true;
                forward = true;
            }
            else if (velocity.x < 0) {
                position.x += penetrationX;
                velocity.x = 0;
                collidedLeft = true;
                forward = false;
            }
        }
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

