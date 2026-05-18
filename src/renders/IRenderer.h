//
// Created by kuzni on 17.05.2026.
//

#ifndef FTESTER_IRENDERER_H
#define FTESTER_IRENDERER_H

#include "../fractal/commonMath.h"
#include "../fractal/fractalParams.h"

class IRenderer {
public:
    IRenderer(int width, int height)
        : width(width), height(height) {
        pixels = new glm::vec3[width * height];
    }

    virtual void render() = 0;
    virtual void display() = 0;

    virtual ~IRenderer() {
        if (pixels != nullptr) {
            delete[] pixels;
            pixels = nullptr;
        }
    }

    const glm::vec3* getPixelData() const { return pixels; }

protected:
    int width, height;
    glm::vec3* pixels = nullptr;

    FractalParams fractal;
    Camera cam;
    ShadeParams shade;
};

#endif //FTESTER_IRENDERER_H
