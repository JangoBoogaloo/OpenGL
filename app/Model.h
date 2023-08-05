#pragma once
#include <glm.hpp>
#include "IKeyListener.h"

class Model : public IKeyListener
{
public:
	glm::vec3 ScaleXYZ;
	glm::vec3 RotateYPR;
	glm::vec3 TranslateXYZ;
	const glm::mat4 GetModelMatrix();
	void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods);
	Model();

private:
	glm::mat4 scaleM;
	glm::mat4 rotationM;
	glm::mat4 translationM;
};

