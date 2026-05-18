#pragma once
#include <glm/glm.hpp>

// Parametry matematyczne fraktala 4D
struct FractalParams {
    glm::vec4 C;
    float w;
    float escapeRadius2;
    int maxIters;
};

// Ustawienia kamery (Ray Marching)
struct Camera {
    glm::vec3 origin;
    glm::vec3 dir;
    glm::vec3 up;
    float aspectRatio;
};

// Parametry oświetlenia i koloru
struct ShadeParams {
    glm::vec3 ambient;
    glm::vec3 baseColor;
    glm::vec3 lightPos;
};