#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQuickControls2/QQuickStyle>

#include "core/GameEngine.h"
#include "core/GameState.h"
#include "core/Snake.h"

using namespace Qt::StringLiterals;

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);
    app.setApplicationName("SnakeMaze");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("SnakeMaze");

    // Use Material style for modern look
    QQuickStyle::setStyle("Material");

    // Register C++ types for use in QML
    qmlRegisterType<GameEngine>("SnakeMaze", 1, 0, "GameEngine");
    qmlRegisterUncreatableType<GameState>(
        "SnakeMaze", 1, 0, "GameState",
        "GameState is created by GameEngine");
    qmlRegisterUncreatableType<Snake>(
        "SnakeMaze", 1, 0, "Snake",
        "Snake is created by GameEngine");

    QQmlApplicationEngine engine;

    const QUrl url(u"qrc:/qt/qml/SnakeMaze/SnakeMaze/src/ui/Main.qml"_s);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        [](const QUrl& url) {
            qWarning() << "Failed to load QML:" << url;
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    engine.load(url);
    return app.exec();
}
