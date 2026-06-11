#ifndef FRACTALITEM_H
#define FRACTALITEM_H

#include <QQuickPaintedItem>
#include <QImage>
#include <QTimer>
#include <chrono>
#include <vector>
#include <atomic>
#include <thread>
#include <mutex>
#include <cuda_runtime.h>
#include "../camera/camera.h"
#include "../utils/timer.h"
#include "../fractal/fractalParams.h"
#include "../renders/parallelRenderer.cuh"
#include "../renders/sequentialRenderer.h"

class FractalItem : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(QString renderTime READ renderTime NOTIFY renderTimeChanged)
    Q_PROPERTY(bool useGPU READ useGPU WRITE setUseGPU NOTIFY useGPUChanged)
    Q_PROPERTY(bool isRunning READ isRunning WRITE setIsRunning NOTIFY isRunningChanged)
    Q_PROPERTY(float renderScale READ renderScale WRITE setRenderScale NOTIFY renderScaleChanged)
    Q_PROPERTY(int dimension READ dimension WRITE setDimension NOTIFY dimensionChanged)
    Q_PROPERTY(int animAxis READ animAxis WRITE setAnimAxis NOTIFY animAxisChanged)

public:
    FractalItem(QQuickItem *parent = nullptr);
    ~FractalItem();

    void paint(QPainter *painter) override;

    int dimension() const { return m_dimension; }
    void setDimension(int d);

    QString renderTime() const { return m_renderTime; }
    bool useGPU() const { return m_useGPU; }
    void setUseGPU(bool val) {
        if (m_useGPU == val) return;
        m_useGPU = val;
        m_allocWidth = 0;
        m_allocHeight = 0;
        emit useGPUChanged();
    }
    bool isRunning() const { return m_isRunning; }
    void setIsRunning(bool val);

    int animAxis() const { return m_animAxis; }
    void setAnimAxis(int axis) {
        m_animAxis = qBound(0, axis, 3);
        emit animAxisChanged();
    }

    float renderScale() const { return m_renderScale; }
    void setRenderScale(float val) {
        m_renderScale = qBound(0.333f, val, 1.0f);
        emit renderScaleChanged();
    }

    Q_INVOKABLE void setKeyState(const QString& key, bool isPressed);
    Q_INVOKABLE void rotateCamera(float dx, float dy);
    Q_INVOKABLE void resetBuffers();
    Q_INVOKABLE void setCx(float v) { m_fractal.C.x = v; }
    Q_INVOKABLE void setCy(float v) { m_fractal.C.y = v; }
    Q_INVOKABLE void setCz(float v) { m_fractal.C.z = v; }
    Q_INVOKABLE void setCw(float v) { m_fractal.C.w = v; }

signals:
    void renderTimeChanged();
    void renderScaleChanged();
    void dimensionChanged();
    void useGPUChanged();
    void isRunningChanged();
    void animAxisChanged();
    void cpuFrameReady();

private slots:
    void updateFrame();
    void onCpuFrameReady();

private:
    int m_animAxis = 2;
    int m_dimension = 3;
    float m_animW = 0.0f;
    QTimer m_animTimer;
    float m_renderScale = 1.0f;
    bool m_isRunning = false;
    bool m_useGPU = true;
    QString m_renderTime = "0.0 ms";

    QTimer m_timer;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_lastTime;
    bool m_keys[4] = {false, false, false, false};

    Camera m_camera;
    Timer m_renderTimer;
    FractalParams m_fractal;
    ParallelRenderer m_parallelRenderer;
    CPURenderer m_sequentialRenderer;

    std::vector<uchar4> m_cpuHostBuffer;
    uchar4* m_cudaDeviceBuffer = nullptr;
    int m_allocWidth = 0;
    int m_allocHeight = 0;

    std::thread m_cpuThread;
    std::atomic<bool> m_cpuStopFlag{false};
    std::atomic<bool> m_cpuFrameDone{false};
    std::atomic<bool> m_firstFrameDone{false};
    std::mutex m_cpuBufferMutex;
    std::vector<uchar4> m_cpuThreadBuffer;
    std::vector<uchar4> m_cpuDisplayBuffer;

    void reallocateBuffers(int w, int h);
    void startCpuThread(int w, int h);
    void stopCpuThread();
};

#endif // FRACTALITEM_H