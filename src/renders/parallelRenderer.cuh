#ifndef PARALLEL_RENDERER_CUH
#define PARALLEL_RENDERER_CUH

#include <cuda_runtime.h>
#include "IRenderer.h"
#include "../camera/camera.h"
#include "../fractal/fractalParams.h"
#include <vector>

// Struktura uchar4 dla CUDA/Qt
struct uchar4;

class ParallelRenderer : public IRenderer {
public:
    ParallelRenderer(int width, int height,
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
    void display() override {} // niepotrzebne

    // CUDA wersja do QML z buforem (to masz wywoływane z FractalItem!)
    void render(const Camera& camQml, const FractalParams& fractal, uchar4* d_output, int w, int h);
};

#endif