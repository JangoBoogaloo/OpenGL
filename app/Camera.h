#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "IMouseScrollListener.h"

class Camera : public IMouseScrollListener
{
private:
	glm::vec3 eye;
	glm::vec3 target;
	glm::vec3 up;
	glm::mat4 projection;
	glm::mat4 worldToCameraMatrix;

public:
	Camera();
	void PerspectiveProjection(float fovyDegree, float aspect, float zNear, float zFar);
	void OrthoGraphicProjection(float left, float right, float top, float bottom, float far, float near);
	void AdvanceProjection(glm::mat4 projectionMatrix);
	void LookAt(glm::vec3 eye, glm::vec3 target, glm::vec3 up);
	const glm::mat4 GetProjection() { return projection; }
	const glm::mat4 GetView();
	void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset);
};

