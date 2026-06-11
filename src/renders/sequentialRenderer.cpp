#include "sequentialRenderer.h"
#include <algorithm>
#include "../fractal/commonMath.cuh"

void CPURenderer::render() {
    if (!pixels) return;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            glm::vec2 texCoord(
                static_cast<float>(x) / width,
                static_cast<float>(y) / height
            );

            glm::vec3 rayDir = getRayDir(cam.dir, cam.up, texCoord, cam.aspectRatio);
            glm::vec3 rayOrigin = cam.origin;

            float t = raymarch(rayOrigin, rayDir, fractal.C, fractal.w);

            glm::vec3 finalColor;
            if (t < 0.0f) {
                finalColor = shade.ambient;
            }
            else {
                finalColor = ::shade(
                    rayOrigin,
                    rayDir,
                    t,
                    fractal.C,
                    fractal.w,
                    shade.ambient,
                    shade.baseColor,
                    shade.lightPos
                );
            }

            pixels[y * width + x] = finalColor;
        }
    }
}

void CPURenderer::display() {
    // Niepotrzebne w integracji z QML, więc zostaw pusto
}