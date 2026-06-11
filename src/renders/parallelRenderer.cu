#include "parallelRenderer.cuh"
#include "../fractal/commonMath.cuh"

__device__ inline glm::vec3 shade_gpu(
    glm::vec3 rayOrigin,
    glm::vec3 rayDir,
    float t,
    glm::vec4 C,
    float w,
    glm::vec3 ambient,
    glm::vec3 baseColor,
    glm::vec3 lightPos)
{
    glm::vec3 point = rayOrigin + t * rayDir;
    glm::vec3 n = gradientSDF(point, C, w);
    glm::vec3 lightDir = glm::normalize(lightPos - point);

    float ao_val = ambientOclusion(point, n, C, w);
    float shadow_val = softShadow(point + n * 0.01f, lightPos, C, w);
    float diffuse = glm::max(0.0f, glm::dot(n, lightDir));

    return ambient * ao_val + diffuse * shadow_val * baseColor;
}

__global__ void renderKernel(uchar4* d_output, int width, int height,
                             RenderCamera cam, FractalParams fractal, ShadeParams shade)
{
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if (x >= width || y >= height) return;

    glm::vec2 texCoord(static_cast<float>(x) / width, static_cast<float>(y) / height);
    glm::vec3 rayDir = getRayDir(cam.dir, cam.up, texCoord, cam.aspectRatio);
    glm::vec3 rayOrigin = cam.origin;

    float t = raymarch(rayOrigin, rayDir, fractal.C, fractal.w);

    glm::vec3 color = shade.ambient;
    if (t > 0.0f) {
        color = shade_gpu(rayOrigin, rayDir, t, fractal.C, fractal.w, shade.ambient, shade.baseColor, shade.lightPos);
    }

    unsigned char r = static_cast<unsigned char>(glm::clamp(color.r * 255.0f, 0.0f, 255.0f));
    unsigned char g = static_cast<unsigned char>(glm::clamp(color.g * 255.0f, 0.0f, 255.0f));
    unsigned char b = static_cast<unsigned char>(glm::clamp(color.b * 255.0f, 0.0f, 255.0f));

    d_output[y * width + x] = make_uchar4(r, g, b, 255);
}

// Pusta, dla polimorfizmu
void ParallelRenderer::render() {}

void ParallelRenderer::render(const Camera& camQml, const FractalParams& fractal, uchar4* d_output, int w, int h) {
    RenderCamera cam;
    cam.origin = camQml.position;
    cam.dir = camQml.front;
    cam.up = camQml.up;
    cam.aspectRatio = camQml.aspectRatio;

    dim3 blockSize(16, 16);
    dim3 gridSize((w + 15) / 16, (h + 15) / 16);

    renderKernel<<<gridSize, blockSize>>>(d_output, w, h, cam, fractal, shade);
    cudaDeviceSynchronize();
}