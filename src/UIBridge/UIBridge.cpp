#include "UIBridge.h"
#include "app.h"
#include <QDebug>

UIBridge::UIBridge(App& app, QObject* parent)
    : QObject(parent), m_app(app)
{}

void UIBridge::startRender(int width, int height, bool useGPU) {
    // Ustawiamy rozmiar oraz urządzenie
    m_app.setWidth(width);
    m_app.setHeight(height);
    m_app.setUseGPU(useGPU);

    // CPU: render pojedyncza klatka, GPU – pętla ogarnia się w renderze
    if (!useGPU) {
        m_app.renderFrame();
        qDebug() << "[UIBridge] CPU: render pojedynczej klatki";
    } else {
        qDebug() << "[UIBridge] GPU: pętla renderowania obsłużona w rendererze (QML/property)";
    }
}

void UIBridge::renderSingleFrame() {
    // CPU: pozwala QML wywołać renderowanie przy każdej zmianie layoutu czy interakcji
    if (!m_app.useGPU()) {
        m_app.renderFrame();
        qDebug() << "[UIBridge] CPU: renderSingleFrame!";
    }
}