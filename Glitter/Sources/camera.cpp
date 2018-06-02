#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>
#include "camera.hpp"


Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch)
    : cameraPosition(position)
	, cameraFront(glm::vec3(0.0f, 0.0f, -1.0f))
    , worldUp (up)
    , cameraYaw (yaw)
    , cameraPitch (pitch)
	, cameraSpeed(defaultSpeed)
	, cameraSensitivity(defaultSensitivity)
	, cameraFOV(defaultFOV)
{
    updateCameraVectors();
}



glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
}


glm::vec3 Camera::getPosition() const
{
	return cameraPosition;
}


GLfloat Camera::getFOV() const
{
	return cameraFOV;
}


void Camera::handleKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
    GLfloat cameraVelocity = cameraSpeed * deltaTime;

    if (direction == FORWARD)
        cameraPosition += cameraFront * cameraVelocity;
    if (direction == BACKWARD)
        cameraPosition -= cameraFront * cameraVelocity;
    if (direction == LEFT)
        cameraPosition -= cameraRight * cameraVelocity;
    if (direction == RIGHT)
        cameraPosition += cameraRight * cameraVelocity;
}


void Camera::handleMouseMove(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
    xoffset *= cameraSensitivity;
    yoffset *= cameraSensitivity;
    cameraYaw += xoffset;
    cameraPitch += yoffset;

    if (constrainPitch)
    {
        if (cameraPitch > 89.0f)
            cameraPitch = 89.0f;
        if (cameraPitch < -89.0f)
            cameraPitch = -89.0f;
    }

    updateCameraVectors();
}


void Camera::handleScroll(GLfloat yoffset)
{
    if (cameraFOV >= glm::radians(1.0f) && cameraFOV <= glm::radians(45.0f))
        cameraFOV -= glm::radians(yoffset);

    if (cameraFOV <= glm::radians(1.0f))
        cameraFOV = glm::radians(1.0f);

    if (cameraFOV >= glm::radians(45.0f))
        cameraFOV = glm::radians(45.0f);
}


void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    front.y = sin(glm::radians(cameraPitch));
    front.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    cameraFront = glm::normalize(front);
    cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
    cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}
