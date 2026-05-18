#define GLM_ENABLE_EXPERIMENTAL
#include "fractal/commonMath.h"
#include "renders/sequentialRendererCPU.h" // Upewnij się, że używasz poprawnej ścieżki
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

FractalParams fractalParams {
    // x = 0.156 (część urojona 'i')
    // y = 0.0   (część urojona 'j' - zerujemy dla 2D)
    // z = 0.0   (część urojona 'k' - zerujemy dla 2D)
    // w = -0.8  (część rzeczywista)
    .C = glm::vec4(0.156f, 0.0f, 0.0f, -0.8f),
    .w = 0.0f,
    .escapeRadius2 = 10.0f,
    .maxIters = 100 // Spirale wymagają więcej iteracji
};

Camera camera {
    .origin = glm::vec3(0.0f, 0.0f, 2.5f),  // Patrzymy idealnie na płaszczyznę XY
    .dir    = glm::vec3(0.0f, 0.0f, -1.0f),
    .up     = glm::vec3(0.0f, 1.0f, 0.0f),
    .aspectRatio = 800.0f / 600.0f
};

ShadeParams shading {
    .ambient   = glm::vec3(0.05f, 0.1f, 0.15f), // Ciemne tło, by kontrastowało
    .baseColor = glm::vec3(0.2f, 0.9f, 0.6f),   // Neonowa zieleń wpadająca w morski
    .lightPos  = glm::vec3(1.0f, 2.0f, 2.0f)    // Oświetlenie lekko z góry
};

int main()
{
    const int WIDTH = 800;
    const int HEIGHT = 600;

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Fractal Renderer", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Utworzenie renderera
    CPURenderer renderer(
        WIDTH, HEIGHT,
        fractalParams.C, fractalParams.w, fractalParams.escapeRadius2, fractalParams.maxIters,
        camera.origin, camera.dir, camera.up,
        shading.ambient, shading.baseColor, shading.lightPos
    );

    std::cout << "Rozpoczecie wyznaczania mapy pikseli na CPU..." << std::endl;
    renderer.render();
    std::cout << "Renderowanie zakonczone. Wyswietlanie." << std::endl;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        renderer.display();

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}