//
// Created by kuzni on 17.05.2026.
//

#include "app.h"

/*
 #include "dynamicRendererGLSL.h"
#include <iostream>

// --- KOD SHADERA KARTY GRAFICZNEJ ---
// Ten kod zostanie wysłany do karty graficznej i skompilowany w locie
const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

// Zmienne odbierane z C++ (zamiast konstruktora)
uniform vec2 u_resolution;
uniform vec3 u_camOrigin;
uniform vec3 u_camDir;
uniform vec3 u_camUp;
uniform vec4 u_C;
uniform float u_w;

// Oświetlenie z C++
uniform vec3 u_ambient;
uniform vec3 u_baseColor;
uniform vec3 u_lightPos;

vec4 qSquare(vec4 q) {
    return vec4(
        2.0 * q.w * q.x,
        2.0 * q.w * q.y,
        2.0 * q.w * q.z,
        q.w * q.w - q.x * q.x - q.y * q.y - q.z * q.z
    );
}

float Julia_distanceEstimator(vec3 p, vec4 C, float w) {
    // Pamiętasz nasz fix na zapadanie wymiarów? Tutaj też jest!
    vec4 z = vec4(p.y, p.z, w, p.x);
    float md2 = 1.0;
    float mz2 = dot(z, z);

    if (mz2 < 1e-10) return 0.0;

    for (int i = 0; i < 80; i++) {
        md2 *= 4.0 * mz2;
        z = qSquare(z) + C;
        mz2 = dot(z, z);
        if (mz2 > 10.0) break;
    }

    if (mz2 > 10.0) {
        return 0.25 * sqrt(mz2 / md2) * log(mz2);
    }
    return 0.0;
}

vec3 getRayDir(vec2 fragCoord) {
    vec2 uv = fragCoord / u_resolution; // od 0.0 do 1.0
    vec3 camSide = normalize(cross(u_camDir, u_camUp));
    float aspectRatio = u_resolution.x / u_resolution.y;

    float x = (uv.x * 2.0 - 1.0) * aspectRatio;
    float y = (uv.y * 2.0 - 1.0);

    return normalize(x * camSide + y * u_camUp + u_camDir * 2.0);
}

vec3 gradientSDF(vec3 p) {
    float e = 0.001;
    return normalize(vec3(
        Julia_distanceEstimator(p + vec3(e,0,0), u_C, u_w) - Julia_distanceEstimator(p - vec3(e,0,0), u_C, u_w),
        Julia_distanceEstimator(p + vec3(0,e,0), u_C, u_w) - Julia_distanceEstimator(p - vec3(0,e,0), u_C, u_w),
        Julia_distanceEstimator(p + vec3(0,0,e), u_C, u_w) - Julia_distanceEstimator(p - vec3(0,0,e), u_C, u_w)
    ));
}

float softShadow(vec3 pt, vec3 lightPos) {
    vec3 lightDir = normalize(lightPos - pt);
    float lightDist = length(lightPos - pt);
    float kd = 1.0;
    int step = 0;

    for (float t = 0.1; t < lightDist && step < 64 && kd > 0.001;) {
        float d = abs(Julia_distanceEstimator(pt + t * lightDir, u_C, u_w));
        if (d < 0.001) kd = 0.0;
        else kd = min(kd, 16.0 * d / t);
        t += d;
        step++;
    }
    return kd;
}

void main() {
    vec3 rayOrigin = u_camOrigin;
    vec3 rayDir = getRayDir(gl_FragCoord.xy);

    float t = 0.0;
    float maxT = 20.0;
    bool hit = false;

    // Główna pętla Ray Marchingu
    for (int i = 0; i < 256; i++) {
        vec3 p = rayOrigin + rayDir * t;
        float d = Julia_distanceEstimator(p, u_C, u_w);

        if (d < 0.001) { hit = true; break; }
        t += d * 0.4;
        if (t > maxT) break;
    }

    if (!hit) {
        FragColor = vec4(u_ambient, 1.0);
    } else {
        vec3 point = rayOrigin + rayDir * t;
        vec3 n = gradientSDF(point);
        vec3 lightDir = normalize(u_lightPos - point);

        float shadow = softShadow(point + n * 0.01, u_lightPos);
        float diffuse = max(0.0, dot(n, lightDir));

        vec3 finalColor = u_ambient + diffuse * shadow * u_baseColor;
        FragColor = vec4(finalColor, 1.0);
    }
}
)";

// Prosty Vertex Shader rysujący kwadrat na cały ekran
const char* vertexShaderSource = R"(
#version 330 core
const vec2 vertices[4] = vec2[4](
    vec2(-1.0, -1.0), vec2(1.0, -1.0),
    vec2(-1.0,  1.0), vec2(1.0,  1.0)
);
void main() {
    gl_Position = vec4(vertices[gl_VertexID], 0.0, 1.0);
}
)";

// Kompilacja w C++
void DynamicRendererGLSL::initShaders() {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void DynamicRendererGLSL::render() {
    // Na GPU renderowanie i wyświetlanie to to samo! Puste z premedytacją.
}

void DynamicRendererGLSL::display() {
    if (shaderProgram == 0) initShaders();

    glUseProgram(shaderProgram);

    // Wysyłanie zmiennych z C++ (np. klawiatury) do karty graficznej
    glUniform2f(glGetUniformLocation(shaderProgram, "u_resolution"), width, height);
    glUniform3f(glGetUniformLocation(shaderProgram, "u_camOrigin"), cam.origin.x, cam.origin.y, cam.origin.z);
    glUniform3f(glGetUniformLocation(shaderProgram, "u_camDir"), cam.dir.x, cam.dir.y, cam.dir.z);
    glUniform3f(glGetUniformLocation(shaderProgram, "u_camUp"), cam.up.x, cam.up.y, cam.up.z);
    glUniform4f(glGetUniformLocation(shaderProgram, "u_C"), fractal.C.x, fractal.C.y, fractal.C.z, fractal.C.w);
    glUniform1f(glGetUniformLocation(shaderProgram, "u_w"), fractal.w);

    glUniform3f(glGetUniformLocation(shaderProgram, "u_ambient"), shade.ambient.x, shade.ambient.y, shade.ambient.z);
    glUniform3f(glGetUniformLocation(shaderProgram, "u_baseColor"), shade.baseColor.x, shade.baseColor.y, shade.baseColor.z);
    glUniform3f(glGetUniformLocation(shaderProgram, "u_lightPos"), shade.lightPos.x, shade.lightPos.y, shade.lightPos.z);

    // Rysowanie 2 trójkątów na cały ekran
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

// Dodana funkcja, by w mainie aktualizować kamerę!
void DynamicRendererGLSL::updateCamera(glm::vec3 newOrigin, glm::vec3 newDir, glm::vec3 newUp) {
    cam.origin = newOrigin;
    cam.dir = newDir;
    cam.up = newUp;
}
 */

