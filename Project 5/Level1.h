#pragma once
#include "Scene.h"
class Level1 : public Scene {

public:
	void Initialize() override;
	glm::mat4 Update(float deltaTime, glm::mat4 viewMatrix) override;
	void Render(ShaderProgram* program) override;
};