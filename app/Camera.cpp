#include "Camera.h"

Camera::Camera()
{
	OrthoGraphicProjection(1, -1, 1, -1, 1, -1);
}

void Camera::PerspectiveProjection(float fovyDegree, float aspect, float zNear, float zFar)
{
	float t = zNear * glm::tan(glm::radians(fovyDegree) / 2);
	float b = -t;
	float r = aspect * t;
	float l = -r;
	float projectionArray[] =
	{
	  2 * zNear / (r - l),  0,                   (r + l) / (r - l),                 0,
	  0,                    2 * zNear / (t - b), (t + b) / (t - b),                 0,
	  0,                    0,                   -(zFar + zNear) / (zFar - zNear), -2 * zFar * zNear / (zFar - zNear),
	  0,                    0,                   -1,                                0
	};

	projection = glm::make_mat4(projectionArray);
}

void Camera::OrthoGraphicProjection(float right, float left, float top, float bottom, float far, float near)
{
	float centering[] =
	{
	  1, 0, 0, -(left + right) / 2,
	  0, 1, 0, -(top + bottom) / 2,
	  0, 0, 1, -(far + near) / 2,
	  0, 0, 0, 1
	};

	float scale[] =
	{
	  2 / (right - left), 0,                  0,                0,
	  0,                  2 / (top - bottom), 0,                0,
	  0,                  0,                  2 / (far - near), 0,
	  0,                  0,                  0,                1,
	};

	projection = glm::make_mat4(scale) * glm::make_mat4(centering);

	//flip z axis
	projection[3][3] *= -1;
}

void Camera::LookAt(glm::vec3 camPosition, glm::vec3 center, glm::vec3 upDirection)
{
	cameraPosition = camPosition;
	float translationArray[] =
	{
	  1, 0, 0, -cameraPosition.x,
	  0, 1, 0, -cameraPosition.y,
	  0, 0, 1, -cameraPosition.z,
	  0, 0, 0, 1
	};
	glm::mat4 translation = glm::make_mat4(translationArray);

	//Not the actual up from camera's perspective
	glm::vec3 up = glm::normalize(upDirection);

	//actual forward, right, up, need to be normalized before set as rotation matrix
	glm::vec3 f = glm::normalize(glm::vec3(center - cameraPosition));
	glm::vec3 r = glm::normalize(glm::cross(f, up));
	glm::vec3 u = glm::normalize(glm::cross(r, f));

	float rotationArray[] =
	{
	  r[0],  r[1],  r[2], 0,
	  u[0],  u[1],  u[2], 0,
	 -f[0], -f[1], -f[2], 0,
	  0,        0,     0, 1
	};
	glm::mat4 rotation = glm::make_mat4(rotationArray);
	worldToCameraMatrix = translation * rotation;
}

void Camera::AdvanceProjection(glm::mat4 projectionMatrix) {
	projection = projectionMatrix;
}