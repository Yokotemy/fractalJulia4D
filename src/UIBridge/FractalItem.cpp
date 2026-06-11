#include "FractalItem.h"
#include <QPainter>
#include <QDebug>
#include <cmath>

static glm::vec4 defaultC(-0.2f, 0.8f, 0.0f, 0.0f);
static float defaultW = 0.0f;
static float defaultEscape = 4.0f;
static int defaultMaxIters = 30;
static glm::vec3 defaultOrigin(0.0f, 0.0f, 4.0f);
static glm::vec3 defaultDir(0.0f, 0.0f, -1.0f);
static glm::vec3 defaultUp(0.0f, 1.0f, 0.0f);
static glm::vec3 defaultAmbient(0.2f);
static glm::vec3 defaultBaseColor(0.8f, 0.5f, 1.0f);
static glm::vec3 defaultLightPos(4.0f, 2.0f, -2.0f);

FractalItem::FractalItem(QQuickItem *parent)
    : QQuickPaintedItem(parent),
      m_parallelRenderer(int(width()), int(height()), defaultC, defaultW, defaultEscape, defaultMaxIters, defaultOrigin, defaultDir, defaultUp, defaultAmbient, defaultBaseColor, defaultLightPos),
      m_sequentialRenderer(int(width()), int(height()), defaultC, defaultW, defaultEscape, defaultMaxIters, defaultOrigin, defaultDir, defaultUp, defaultAmbient, defaultBaseColor, defaultLightPos)
{
    setRenderTarget(QQuickPaintedItem::Image);

    connect(&m_timer, &QTimer::timeout, this, &FractalItem::updateFrame);
    m_timer.setInterval(16);

    connect(&m_animTimer, &QTimer::timeout, this, [this]() {
        m_animW += 0.02f;
        if (m_animW > 3.14159f) m_animW = -3.14159f;
        float val = std::sin(m_animW) * 0.5f;
        switch (m_animAxis) {
            case 0: m_fractal.C.x = val; break;
            case 1: m_fractal.C.y = val; break;
            case 2: m_fractal.C.z = val; break;
            case 3: m_fractal.C.w = val; break;
        }
    });
    m_animTimer.setInterval(16);

    connect(this, &FractalItem::cpuFrameReady, this, &FractalItem::onCpuFrameReady,
            Qt::QueuedConnection);

    m_lastTime = std::chrono::high_resolution_clock::now();

    m_fractal.C = defaultC;
    m_fractal.w = defaultW;
    m_fractal.escapeRadius2 = defaultEscape;
    m_fractal.maxIters = defaultMaxIters;
}

FractalItem::~FractalItem() {
    stopCpuThread();
    if (m_cudaDeviceBuffer)
        cudaFree(m_cudaDeviceBuffer);
}

void FractalItem::startCpuThread(int w, int h) {
    stopCpuThread();
    m_cpuStopFlag = false;
    m_cpuFrameDone = false;

    RenderCamera camSnapshot;
    camSnapshot.origin      = m_camera.position;
    camSnapshot.dir         = m_camera.front;
    camSnapshot.up          = m_camera.up;
    camSnapshot.aspectRatio = float(w) / float(h);

    FractalParams fractalSnapshot = m_fractal;

    m_cpuThread = std::thread([this, w, h, camSnapshot, fractalSnapshot]() {
        auto start = std::chrono::high_resolution_clock::now();

        for (int y = 0; y < h && !m_cpuStopFlag; ++y) {
            for (int x = 0; x < w && !m_cpuStopFlag; ++x) {
                glm::vec2 texCoord(float(x) / w, float(y) / h);
                glm::vec3 rayDir = getRayDir(
                    camSnapshot.dir, camSnapshot.up, texCoord, camSnapshot.aspectRatio);
                float t = raymarch(camSnapshot.origin, rayDir,
                                   fractalSnapshot.C, fractalSnapshot.w);
                glm::vec3 col = (t < 0.0f)
                    ? defaultAmbient
                    : shade(camSnapshot.origin, rayDir, t,
                            fractalSnapshot.C, fractalSnapshot.w,
                            defaultAmbient, defaultBaseColor, defaultLightPos);

                uchar4 px{
                    (unsigned char)(glm::clamp(col.r * 255.0f, 0.0f, 255.0f)),
                    (unsigned char)(glm::clamp(col.g * 255.0f, 0.0f, 255.0f)),
                    (unsigned char)(glm::clamp(col.b * 255.0f, 0.0f, 255.0f)),
                    255
                };

                {
                    std::lock_guard<std::mutex> lock(m_cpuBufferMutex);
                    if (y * w + x < (int)m_cpuThreadBuffer.size())
                        m_cpuThreadBuffer[y * w + x] = px;
                }
            }
            emit cpuFrameReady();
        }

        // Czas tylko pierwszej (i jedynej) klatki CPU
        if (!m_cpuStopFlag) {
            auto end = std::chrono::high_resolution_clock::now();
            double ms = std::chrono::duration<double, std::milli>(end - start).count();
            m_renderTime = QString::number(ms, 'f', 2) + " ms";
            emit renderTimeChanged();
        }
    });
}

void FractalItem::stopCpuThread() {
    m_cpuStopFlag = true;
    if (m_cpuThread.joinable())
        m_cpuThread.join();
}

void FractalItem::onCpuFrameReady() {
    {
        std::lock_guard<std::mutex> lock(m_cpuBufferMutex);
        m_cpuDisplayBuffer = m_cpuThreadBuffer;
    }
    update();
}

void FractalItem::setIsRunning(bool val) {
    if (m_isRunning == val) return;
    m_isRunning = val;
    m_firstFrameDone = false;
    emit isRunningChanged();

    if (m_isRunning) {
        if (m_useGPU) {
            m_timer.start();
        } else {
            int w = int(width()  * m_renderScale);
            int h = int(height() * m_renderScale);
            m_cpuThreadBuffer.resize(w * h);
            m_cpuDisplayBuffer.resize(w * h);
            startCpuThread(w, h);
        }
        if (m_dimension == 4) m_animTimer.start();
        m_lastTime = std::chrono::high_resolution_clock::now();
    } else {
        m_timer.stop();
        m_animTimer.stop();
        stopCpuThread();
    }
}

void FractalItem::setDimension(int d) {
    if (m_dimension == d) return;
    m_dimension = d;
    m_camera = Camera();
    resetBuffers();

    if (m_isRunning) {
        if (d == 4) m_animTimer.start();
        else {
            m_animTimer.stop();
            m_fractal.w = 0.0f;
        }
    }

    emit dimensionChanged();
}

void FractalItem::setKeyState(const QString& key, bool isPressed) {
    if (key == "W") m_keys[0] = isPressed;
    if (key == "S") m_keys[1] = isPressed;
    if (key == "A") m_keys[2] = isPressed;
    if (key == "D") m_keys[3] = isPressed;
}

void FractalItem::rotateCamera(float dx, float dy) {
    m_camera.processMouseMovement(dx, dy);
    update();
}

void FractalItem::resetBuffers() {
    stopCpuThread();
    m_allocWidth = 0;
    m_allocHeight = 0;
    m_sequentialRenderer.width = 0;
    m_sequentialRenderer.height = 0;
    if (m_sequentialRenderer.pixels) {
        delete[] m_sequentialRenderer.pixels;
        m_sequentialRenderer.pixels = nullptr;
    }
    if (m_cudaDeviceBuffer) {
        cudaFree(m_cudaDeviceBuffer);
        m_cudaDeviceBuffer = nullptr;
    }
    m_cpuThreadBuffer.clear();
    m_cpuDisplayBuffer.clear();
}

void FractalItem::updateFrame() {
    auto now = std::chrono::high_resolution_clock::now();
    float dt = std::chrono::duration<float>(now - m_lastTime).count();
    m_lastTime = now;
    if (m_keys[0]) m_camera.processKeyboard("FORWARD", dt);
    if (m_keys[1]) m_camera.processKeyboard("BACKWARD", dt);
    if (m_keys[2]) m_camera.processKeyboard("LEFT", dt);
    if (m_keys[3]) m_camera.processKeyboard("RIGHT", dt);

    m_renderTimer.start();
    update();
    double ms = m_renderTimer.stopGPU();
    if (!m_firstFrameDone) {
        m_firstFrameDone = true;
        m_renderTime = QString::number(ms, 'f', 2) + " ms";
        emit renderTimeChanged();
    }
}

void FractalItem::reallocateBuffers(int w, int h) {
    if (w == m_allocWidth && h == m_allocHeight)
        return;
    m_allocWidth = w;
    m_allocHeight = h;
    m_cpuHostBuffer.resize(w * h);
    if (m_cudaDeviceBuffer)
        cudaFree(m_cudaDeviceBuffer);
    cudaMalloc(&m_cudaDeviceBuffer, w * h * sizeof(uchar4));
}

void FractalItem::paint(QPainter *painter) {
    if (!m_isRunning) return;

    int w = int(width()  * m_renderScale);
    int h = int(height() * m_renderScale);

    if (m_useGPU) {
        reallocateBuffers(w, h);
        m_camera.aspectRatio = float(w) / float(h);

        if (m_dimension == 3) m_fractal.w = 0.0f;

        m_parallelRenderer.render(m_camera, m_fractal, m_cudaDeviceBuffer, w, h);
        cudaMemcpy(m_cpuHostBuffer.data(), m_cudaDeviceBuffer,
                   w * h * sizeof(uchar4), cudaMemcpyDeviceToHost);

        QImage img(reinterpret_cast<uchar*>(m_cpuHostBuffer.data()),
                   w, h, QImage::Format_RGBA8888);
        painter->drawImage(QRect(0, 0, int(width()), int(height())), img);

    } else {
        std::lock_guard<std::mutex> lock(m_cpuBufferMutex);
        if ((int)m_cpuDisplayBuffer.size() == w * h) {
            QImage img(reinterpret_cast<uchar*>(m_cpuDisplayBuffer.data()),
                       w, h, QImage::Format_RGBA8888);
            painter->drawImage(QRect(0, 0, int(width()), int(height())), img);
        }
    }
}