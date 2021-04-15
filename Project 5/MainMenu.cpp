#include "MainMenu.h"


void MainMenu::Initialize() {
    state.nextScene = -1;
    state.over = false;
  
    
}
glm::mat4 MainMenu::Update(float deltaTime, glm::mat4 viewMatrix) {
    return viewMatrix;
}
void MainMenu::Render(ShaderProgram* program) {
    GLuint fontTextureID = Util::LoadTexture("font1.png");
    
    Util::DrawText(program, fontTextureID, "Aliens in Arkansas", 0.5f, -0.25f,
        glm::vec3(-3.0f, 1.0, 0));
    Util::DrawText(program, fontTextureID, "[press enter to start]", 0.5f, -0.25f,
        glm::vec3(-3.0f, 0, 0));

}