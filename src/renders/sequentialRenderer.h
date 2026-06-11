#ifndef FTESTER_SEQUENTIALRENDERERCPU_H
#define FTESTER_SEQUENTIALRENDERERCPU_H

#include "IRenderer.h"
#include "../fractal/fractalParams.h"

class CPURenderer : public IRenderer {
public:
    CPURenderer(int width, int height,
        glm::vec4 C,
        float w,
        float escapeRadius2,
        int maxIters,
        glm::vec3 origin,
        glm::vec3 dir,
        glm::vec3 up,
        glm::vec3 ambient,
        glm::vec3 baseColor,
        glm::vec3 lightPos)
        : IRenderer(width, height)
    {
        fractal.C = C;
        fractal.w = w;
        fractal.escapeRadius2 = escapeRadius2;
        fractal.maxIters = maxIters;

        cam.origin = origin;
        cam.dir = dir;
        cam.up = up;
        cam.aspectRatio = static_cast<float>(width) / height;

        shade.ambient = ambient;
        shade.baseColor = baseColor;
        shade.lightPos = lightPos;
    }

    void render() override;
    void display() override;
};

#endif //FTESTER_SEQUENTIALRENDERERCPU_H