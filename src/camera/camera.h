#ifndef FTESTER_CAMERA_H
#define FTESTER_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

class Camera {
public:
    // Wektory stanu
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;

    // Parametry kamery
    float yaw;
    float pitch;
    float movementSpeed;
    float mouseSensitivity;
    float aspectRatio;

    Camera(glm::vec3 startPosition = glm::vec3(0.0f, 0.0f, 4.0f), float startAspectRatio = 16.0f / 9.0f);

    // Nowa metoda obsługi wejścia (niezależna od GLFW)
    void processKeyboard(const std::string& direction, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset);

private:
    void updateCameraVectors();
};

#endif // FTESTER_CAMERA_H