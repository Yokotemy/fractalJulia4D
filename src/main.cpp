#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "app.h"
#include "UIBridge/UIBridge.h"
#include "UIBridge/FractalItem.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    qmlRegisterType<FractalItem>("FractalApp", 1, 0, "FractalItem");

    App appBackend;
    UIBridge uiBridge(appBackend);

    QQmlApplicationEngine engine;
    // Udostępniamy oba obiekty do QML
    engine.rootContext()->setContextProperty("appBackend", &appBackend);
    engine.rootContext()->setContextProperty("uiBridge", &uiBridge);

    const QUrl url = QUrl::fromLocalFile(
        QCoreApplication::applicationDirPath() + "/qml/qml/Main.qml"
    );

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl) QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);

    engine.load(url);
    return app.exec();
}