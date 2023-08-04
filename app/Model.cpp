#include "Model.h"

#include <gtc/type_ptr.hpp>

void Model::Scale(float x, float y, float z)
{
	float scaleMat[] =
	{
	  x, 0, 0, 0,
	  0, y, 0, 0,
	  0, 0, z, 0,
	  0, 0, 0, 1
	};
	scale = glm::make_mat4(scaleMat);
}

void Model::Translate(float x, float y, float z)
{
	float translateMat[]
	{
	  1, 0, 0, x,
	  0, 1, 0, y,
	  0, 0, 1, z,
	  0, 0, 0, 1
	};
	translation = glm::make_mat4(translateMat);
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

void Model::Rotate(const float yawDegree, const float pitchDegree, const float rollDegree)
{
	rotation = RotateYaw(yawDegree) * RotatePitch(pitchDegree) * RotateRoll(rollDegree);
}

const glm::mat4 Model::GetModelMatrix()
{
	//return translation * rotation * scale;
	//transpose(T) * transpose(R) * transpose(S) = transpose(S * R * T)
	auto result = scale * rotation * translation;
	return result;
}

Model::Model()
{
	scale = glm::mat4(1);
	rotation = glm::mat4(1);
	translation = glm::mat4(1);
}