#pragma once

#include <glm.hpp>

class Model
{
public:
	void Scale(float x, float y, float z);
	void Rotate(float yawDegree, float pitchDegree, float rollDegree);
	void Translate(float x, float y, float z);
	const glm::mat4 GetModelMatrix();
	Model();

private:
	glm::mat4 scale;
	glm::mat4 rotation;
	glm::mat4 translation;
};

