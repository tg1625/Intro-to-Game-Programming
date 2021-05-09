#include "WinScreen.h"


void WinScreen::Initialize() {
    state.nextScene = -1;
    state.over = false;


}
glm::mat4 WinScreen::Update(float deltaTime, glm::mat4 viewMatrix) {
    viewMatrix = glm::mat4(1.0f);
    return viewMatrix;
}
void WinScreen::Render(ShaderProgram* program) {
    GLuint fontTextureID = Util::LoadTexture("font1.png");

    Util::DrawText(program, fontTextureID, "You win!", 0.5f, -0.25f,
        glm::vec3(-3.0f, 0.0, 0));
    Util::DrawText(program, fontTextureID, "[press enter to replay]", 0.5f, -0.25f,
        glm::vec3(-3.0f, -0.75f, 0));

}

void WinScreen::RenderUI(ShaderProgram* program) {
    return;
}