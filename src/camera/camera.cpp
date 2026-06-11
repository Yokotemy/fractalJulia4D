#include "camera.h"
#include <cmath>
#include <algorithm>

Camera::Camera(glm::vec3 startPosition, float startAspectRatio)
    : position(startPosition),
      front(glm::vec3(0.0f, 0.0f, -1.0f)),
      up(glm::vec3(0.0f, 1.0f, 0.0f)),
      yaw(-90.0f),
      pitch(0.0f),
      movementSpeed(5.0f),
      mouseSensitivity(0.2f),
      aspectRatio(startAspectRatio)
{
    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    glm::vec3 newFront;
    newFront.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    newFront.y = std::sin(glm::radians(pitch));
    newFront.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));

    front = glm::normalize(newFront);
    right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
    up    = glm::normalize(glm::cross(right, front));
}

void Camera::processKeyboard(const std::string& direction, float deltaTime) {
    float velocity = movementSpeed * deltaTime;

    if (direction == "FORWARD")  position += front * velocity;
    if (direction == "BACKWARD") position -= front * velocity;
    if (direction == "LEFT")     position -= right * velocity;
    if (direction == "RIGHT")    position += right * velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset) {
    // Uwaga: w Qt (yoffset) może być dodatni/ujemny w zależności od MouseArea
    yaw   += xoffset * mouseSensitivity;
    pitch += yoffset * mouseSensitivity;

    // Ograniczenie pola widzenia, aby nie "przekręcić" kamery
    if (pitch > 89.0f)  pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    updateCameraVectors();
}