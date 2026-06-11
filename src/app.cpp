#include "app.h"
#include "renders/parallelRenderer.cuh"
#include "renders/sequentialRenderer.h"
#include <QDebug>

App::App(QObject* parent)
    : QObject(parent),
      m_width(1280),
      m_height(720),
      m_useGPU(true)
{
    m_camera = std::make_unique<Camera>();
    updateRenderer();
}

App::~App() {}

void App::setWidth(int w) {
    if (m_width != w) {
        m_width = w;
        emit widthChanged(w);
        updateRenderer();
    }
}

void App::setHeight(int h) {
    if (m_height != h) {
        m_height = h;
        emit heightChanged(h);
        updateRenderer();
    }
}

void App::setUseGPU(bool gpu) {
    if (m_useGPU != gpu) {
        m_useGPU = gpu;
        emit useGPUChanged(gpu);
        updateRenderer();
    }
}

void App::updateRenderer() {
    qDebug() << "Nowy renderer: " << (m_useGPU ? "GPU (CUDA)" : "CPU");
    glm::vec4 defaultParam(0.0f);
    glm::vec3 defaultVec(0.0f);
    float defaultFloat = 0.0f;
    int defaultInt = 100;

    if (m_useGPU) {
        m_renderer = std::make_unique<ParallelRenderer>(
            m_width, m_height, defaultParam, defaultFloat, defaultFloat,
            defaultInt, defaultVec, defaultVec, defaultVec, defaultVec, defaultVec, defaultVec
        );
    } else {
        m_renderer = std::make_unique<CPURenderer>(
            m_width, m_height, defaultParam, defaultFloat, defaultFloat,
            defaultInt, defaultVec, defaultVec, defaultVec, defaultVec, defaultVec, defaultVec
        );
    }
}

void App::renderFrame() {
    if (!m_useGPU && m_renderer) {
        m_renderer->render();
        qDebug() << "Rendered single CPU frame";
    }
}