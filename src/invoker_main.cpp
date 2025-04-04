#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <boost/asio.hpp>
#include "TaskHandler.h"

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);

    // Create Boost.Asio's io_context
    boost::asio::io_context io_context;
    auto work_guard = boost::asio::make_work_guard(io_context);

    // Create the TaskHandler instance
    TaskHandler taskHandler(io_context);

    // Set up the QML engine
    QQmlApplicationEngine engine;

    // Expose TaskHandler to QML as "taskHandler"
    engine.rootContext()->setContextProperty("taskHandler", &taskHandler);

    // Load the QML file
    const QUrl url(QStringLiteral("qrc:/TaskHandlerModule/src/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject* obj, const QUrl& objUrl) {
                         if (!obj && url == objUrl)
                             QCoreApplication::exit(-1);
                     },
                     Qt::QueuedConnection);

    engine.load(url);

    // Run Boost.Asio's io_context in a separate thread
    std::thread asio_thread([&io_context]() {
        qDebug() << "Boost.Asio io_context is running...";
        io_context.run();
        qDebug() << "Boost.Asio io_context stopped.";
    });

    // Connect the QML application quit signal to gracefully stop io_context
    QObject::connect(&app, &QCoreApplication::aboutToQuit, [&]() {
        qDebug() << "Application is about to quit. Stopping io_context...";
        work_guard.reset();  // Allow io_context to exit
        io_context.stop();   // Stop the io_context
        asio_thread.join();  // Wait for the thread to finish
        qDebug() << "io_context stopped and thread joined.";
    });

    // Run the Qt event loop
    int result = app.exec();

    // Ensure io_context is stopped and thread is joined (in case of unexpected exit)
    if (asio_thread.joinable()) {
        work_guard.reset();
        io_context.stop();
        asio_thread.join();
    }

    return result;
}