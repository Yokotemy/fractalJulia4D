#pragma once

#include <QObject>
#include <memory>
#include "camera/camera.h"
#include "renders/IRenderer.h"

class App : public QObject {
    Q_OBJECT
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(bool useGPU READ useGPU WRITE setUseGPU NOTIFY useGPUChanged)

public:
    explicit App(QObject* parent = nullptr);
    ~App();

    int width() const { return m_width; }
    int height() const { return m_height; }
    bool useGPU() const { return m_useGPU; }

    void setWidth(int w);
    void setHeight(int h);
    void setUseGPU(bool gpu);

    Q_INVOKABLE void renderFrame();

    signals:
        void widthChanged(int);
    void heightChanged(int);
    void useGPUChanged(bool);

private:
    void updateRenderer();

    int m_width, m_height;
    bool m_useGPU;
    std::unique_ptr<Camera> m_camera;
    std::unique_ptr<IRenderer> m_renderer;
};