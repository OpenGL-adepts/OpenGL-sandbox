#ifndef CAMERA_H
#define CAMERA_H

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>


enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};


class Camera
{
public:
	static constexpr GLfloat defaultYaw			= -90.0f;
	static constexpr GLfloat defaultPitch		= 0.0f;
	static constexpr GLfloat defaultSpeed		= 1.0f;
	static constexpr GLfloat defaultSensitivity	= 0.10f;
	static constexpr GLfloat defaultFOV			= glm::radians(45.0f);

	Camera( glm::vec3 position  = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3 up		= glm::vec3(0.0f, 1.0f, 0.0f),
			GLfloat yaw			= defaultYaw,
			GLfloat pitch		= defaultPitch);

	glm::mat4 getViewMatrix() const;
	GLfloat getFOV() const;
	void handleKeyboard(Camera_Movement direction, GLfloat deltaTime);
	void handleMouseMove(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);
	void handleScroll(GLfloat yoffset);

private:
	void updateCameraVectors();

	glm::vec3 cameraPosition;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::vec3 cameraRight;
	glm::vec3 worldUp;
	GLfloat cameraYaw;
	GLfloat cameraPitch;
	GLfloat cameraSpeed;
	GLfloat cameraSensitivity;
	GLfloat cameraFOV;
};

#endif
