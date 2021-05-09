#pragma once
#include "Scene.h"
class Level3 : public Scene {

public:
	void Initialize() override;
	glm::mat4 Update(float deltaTime, glm::mat4 viewMatrix);
	void Render(ShaderProgram* program) override;
};