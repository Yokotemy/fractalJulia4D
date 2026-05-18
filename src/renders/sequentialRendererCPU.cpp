//
// Created by kuzni on 17.05.2026.
//

#include "sequentialRendererCPU.h"
#include <GL/glew.h>
#include "../fractal/commonMath.h"

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

void CPURenderer::display() { // <- Poprawione przypisanie do klasy
    if (!pixels) return;

    if (textureID == 0) {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB32F,
        width,
        height,
        0,
        GL_RGB,
        GL_FLOAT,
        pixels
    );

    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 1.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}