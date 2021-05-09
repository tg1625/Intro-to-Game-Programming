#include "LoseScreen.h"


void LoseScreen::Initialize() {
    state.nextScene = -1;
    state.over = false;

    viewMatrix = glm::mat4(1.0f);
}
glm::mat4 LoseScreen::Update(float deltaTime, glm::mat4 viewMatrix) {
    viewMatrix = glm::mat4(1.0f);
    //viewMatrix = glm::translate(viewMatrix, glm::vec3(-9, 3.75, 0));
    return viewMatrix;
}

void LoseScreen::Render(ShaderProgram* program) {
    GLuint fontTextureID = Util::LoadTexture("font1.png");

    /*Util::DrawText(program, fontTextureID, "You lose!", 0.5f, -0.25f,
        glm::vec3(-11.0f, 3.75f , 0));
    Util::DrawText(program, fontTextureID, "[press enter to restart]", 0.5f, -0.25f,
        glm::vec3(-11.0f, 2.75f , 0));*/
    Util::DrawText(program, fontTextureID, "You lose :(", 0.5f, -0.25f,
        glm::vec3(-3.0f, 0.0, 0));
    Util::DrawText(program, fontTextureID, "[press enter to replay]", 0.5f, -0.25f,
        glm::vec3(-3.0f, -0.75f, 0));

}

void LoseScreen::RenderUI(ShaderProgram* program) {
    return;
}
