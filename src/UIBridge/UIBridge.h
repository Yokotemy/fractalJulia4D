#pragma once

#include <QObject>

class App;

class UIBridge : public QObject {
    Q_OBJECT

public:
    explicit UIBridge(App& app, QObject* parent = nullptr);

    // Wywołaj renderowanie według zadanych parametrów (możesz wywołać z QML/Q_INVOKABLE)
    Q_INVOKABLE void startRender(int width, int height, bool useGPU);

    // Przykładowa metoda do wywołania pojedynczej ramki na CPU (opcjonalnie)
    Q_INVOKABLE void renderSingleFrame();

private:
    App& m_app;
};