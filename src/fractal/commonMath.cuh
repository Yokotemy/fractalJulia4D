#ifndef FTESTER_COMMONMATH_CUH
#define FTESTER_COMMONMATH_CUH

#include <cuda_runtime.h>

#include "../fractal/fractalParams.h"
#include <glm/glm.hpp>
#include <glm/geometric.hpp>
#include <glm/vec4.hpp>
#include <cmath>
#include <algorithm>

#if defined(__CUDACC__)
    #define CUDA_HOST_DEVICE __host__ __device__
#else
    #define CUDA_HOST_DEVICE
#endif

// q(x, y, z, w) kwadrat kwaternionu
CUDA_HOST_DEVICE inline glm::vec4 qSquare(const glm::vec4& q) {
    return glm::vec4(
        2.0f * q.w * q.x, // x
        2.0f * q.w * q.y, // y
        2.0f * q.w * q.z, // z
        q.w * q.w - q.x * q.x - q.y * q.y - q.z * q.z // w
    );
}

// Estymator odległości punktu od fraktala Julii (SDF)
// https://www.scitepress.org/Papers/2024/123805/123805.pdf
CUDA_HOST_DEVICE inline float Julia_distanceEstimator(glm::vec3 point, // pozycja punktu na promieniu
    glm::vec4 C, // stała Julii
    float w, // 4 wymiar
    float escapeRadius2 = 4.0f, // promień ucieczki
    int maxIters = 30) // maksymalna liczba iteracji fraktala
{
    glm::vec4 z(point.y, point.z, w, point.x);
    float md2 = 1.0f;
    float mz2 = glm::dot(z, z);

    if (mz2 < 1e-10f) return 0.0f;

    for (int i = 0; i < maxIters; i++) {
        md2 *= 4.0f * mz2;
        z = qSquare(z) + C;
        mz2 = glm::dot(z, z);

        if (mz2 > escapeRadius2) break;
    }

    if (mz2 > escapeRadius2) {
        return 0.25f * std::sqrt(mz2 / md2) * std::log(mz2);
    }
    return 0.0f;
}

// Wyznaczenie wektora kierunku promienia
// https://www.cl.cam.ac.uk/teaching/1718/AdvGraph/5.%20GPU%20Ray%20Marching.pdf
CUDA_HOST_DEVICE inline glm::vec3 getRayDir(glm::vec3 camDir, // kierunek kamery
    glm::vec3 camUp, // orientacja kamery
    glm::vec2 texCoord, // współrzędne pikselu po normalizacji [0, 1]
    float aspectRatio) // stosunek szerokości/wysokości
{
    glm::vec3 camSide = glm::normalize(glm::cross(camDir, camUp));
    float x = (texCoord.x * 2.0f - 1.0f) * aspectRatio;
    float y = (texCoord.y * 2.0f - 1.0f);

    return glm::normalize(x * camSide + y * camUp + camDir * 2.0f);
}

CUDA_HOST_DEVICE inline float raymarch(glm::vec3 rayOrgin, // pozycja startowa promienia
    glm::vec3 rayDir, // kierunek promienia
    glm::vec4 C, // stała Julii
    float w) // 4 wymiar
{
    float t = 0.0f;
    constexpr float maxT = 20.0f;

    for (int i = 0; i < 256; i++) { // Zwiększamy liczbę kroków
        glm::vec3 p = rayOrgin + rayDir * t;
        float d = Julia_distanceEstimator(p, C, w);

        if (d < 0.001f) return t;

        // TO JEST KLUCZOWE: Mnożnik zapobiega przelatywaniu przez detale i usuwa te "paski"
        t += d * 0.4f;

        if (t > maxT) break;
    }

    return -1.0f;
}

// Shading
// Gradient SDF
CUDA_HOST_DEVICE inline glm::vec3 gradientSDF(glm::vec3 point, glm::vec4 C, float w) {
    float e = 0.001f;
    return glm::normalize(glm::vec3(
        Julia_distanceEstimator(point + glm::vec3(e, 0, 0), C, w) - Julia_distanceEstimator(point - glm::vec3(e, 0, 0), C, w),
        Julia_distanceEstimator(point + glm::vec3(0, e, 0), C, w) - Julia_distanceEstimator(point - glm::vec3(0, e, 0), C, w),
        Julia_distanceEstimator(point + glm::vec3(0, 0, e), C, w) - Julia_distanceEstimator(point - glm::vec3(0, 0, e), C, w)
    ));
}

// Ambient occlusion
CUDA_HOST_DEVICE inline float ambientOclusion(glm::vec3 point,
    glm::vec3 n, // normalna do powierzchni w punkcie trafienia (gradientSDF)
    glm::vec4 C,
    float w) {
    float occ = 0.0f;
    float weight = 1.0f;
    for (int i = 1; i <= 5; i++) {
        float d = Julia_distanceEstimator(point + n * (i * 0.1f), C, w);
        occ += weight * (i * 0.1f - d);
        weight *= 0.5f;
    }
    return 1.0f - glm::clamp(occ, 0.0f, 1.0f);
}

// Soft shadow
CUDA_HOST_DEVICE inline float softShadow(glm::vec3 pt, glm::vec3 lightPos, glm::vec4 C, float w, int maxSteps = 64) {
    glm::vec3 lightDir = glm::normalize(lightPos - pt);
    float lightDist = glm::length(lightPos - pt);
    float kd = 1.0f;
    int step = 0;

    for (float t = 0.1f; t < lightDist && step < maxSteps && kd > 0.001f;) {
        float d = std::abs(Julia_distanceEstimator(pt + t * lightDir, C, w));
        if (d < 0.001f) {
            kd = 0.0f;
        }
        else {
            kd = std::min(kd, 16.0f * d / t);
        }
        t += d;
        step++;
    }

    return kd;
}

CUDA_HOST_DEVICE inline glm::vec3 shade(glm::vec3 rayOrigin,
    glm::vec3 rayDir,
    float t,
    glm::vec4 C,
    float w,
    glm::vec3 ambient, // kolor tła
    glm::vec3 baseColor, // podstawowy kolor fraktala
    glm::vec3 lightPos) // pozycja źródła światła
{
    glm::vec3 point = rayOrigin + t * rayDir;
    glm::vec3 n = gradientSDF(point, C, w);
    glm::vec3 lightDir = glm::normalize(lightPos - point);

    float ao_val = ambientOclusion(point, n, C, w);
    float shadow_val = softShadow(point + n * 0.01f, lightPos, C, w);
    float diffuse = glm::max(0.0f, glm::dot(n, lightDir));

    return ambient * ao_val + diffuse * shadow_val * baseColor;
}

#endif //FTESTER_COMMONMATH_CUH