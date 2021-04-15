#include "WinScreen.h"


void WinScreen::Initialize() {
    state.nextScene = -1;
    state.over = false;


}
glm::mat4 WinScreen::Update(float deltaTime, glm::mat4 viewMatrix) {
    return viewMatrix;
}
void WinScreen::Render(ShaderProgram* program) {
    GLuint fontTextureID = Util::LoadTexture("font1.png");

    Util::DrawText(program, fontTextureID, "You Win!", 0.5f, -0.25f,
        glm::vec3(7.5f, -3.0f, 0));
    Util::DrawText(program, fontTextureID, "[press enter to restart]", 0.5f, -0.25f,
        glm::vec3(6.5f, -4, 0));

}