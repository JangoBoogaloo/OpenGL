#include "ModelMatrix.h"

#include <gtc/type_ptr.hpp>

using namespace Matrix;

static inline glm::mat4 Scale(float x, float y, float z)
{
	float scaleMat[] =
	{
	  x, 0, 0, 0,
	  0, y, 0, 0,
	  0, 0, z, 0,
	  0, 0, 0, 1
	};
	return glm::make_mat4(scaleMat);
}

static inline glm::mat4 Translate(float x, float y, float z)
{
	float translateMat[]
	{
	  1, 0, 0, x,
	  0, 1, 0, y,
	  0, 0, 1, z,
	  0, 0, 0, 1
	};
	return glm::make_mat4(translateMat);
}

static inline glm::mat4 RotateYaw(const float yawDegree)
{
	float yawRadian = glm::radians(yawDegree);
	float rotYawArray[] =
	{
	  cos(yawRadian), -sin(yawRadian), 0, 0,
	  sin(yawRadian),  cos(yawRadian), 0, 0,
	  0,               0,              1, 0,
	  0,               0,              0, 1,
	};
	return glm::make_mat4(rotYawArray);
}

static inline glm::mat4 RotatePitch(const float pitchDegree)
{
	float pitchRadian = glm::radians(pitchDegree);
	float rotPitchArray[] =
	{
	  cos(pitchRadian),  0, sin(pitchRadian), 0,
	  0,                 1, 0,                0,
	  -sin(pitchRadian), 0, cos(pitchRadian), 0,
	  0,                 0, 0,                1
	};
	return glm::make_mat4(rotPitchArray);
}

static inline glm::mat4 RotateRoll(const float rollDegree)
{
	float rollRadian = glm::radians(rollDegree);
	float rotRollArray[] =
	{
	  1, 0,               0,                0,
	  0, cos(rollRadian), -sin(rollRadian), 0,
	  0, sin(rollRadian), cos(rollRadian),  0,
	  0, 0,               0,                1
	};
	return glm::make_mat4(rotRollArray);
}

const glm::mat4 ModelMatrix::GetModelMatrix()
{
	translationM = Translate(TranslateXYZ.x, TranslateXYZ.y, TranslateXYZ.z);
	rotationM = RotateYaw(RotateYPR.x) * RotatePitch(RotateYPR.y) * RotateRoll(RotateYPR.z);
	scaleM = Scale(ScaleXYZ.x, ScaleXYZ.y, ScaleXYZ.z);
	//return translation * rotation * scale;
	//transpose(T) * transpose(R) * transpose(S) = transpose(S * R * T)
	auto result = scaleM * rotationM * translationM;
	return result;
}

ModelMatrix::ModelMatrix()
{
	ScaleXYZ = glm::vec3(1, 1, 1);
	RotateYPR = glm::vec3();
	TranslateXYZ = glm::vec3();
	scaleM = glm::mat4(1);
	rotationM = glm::mat4(1);
	translationM = glm::mat4(1);
}

void ModelMatrix::OnKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_RELEASE) return;
	switch (key) {
	case GLFW_KEY_Y:
		RotateYPR.x += 1.0f;
		break;
	case GLFW_KEY_R:
		RotateYPR.y += 1.0f;
		break;
	case GLFW_KEY_P:
		RotateYPR.z += 1.0f;
		break;
	case GLFW_KEY_SPACE:
		RotateYPR = glm::vec3();
		break;
	}
}