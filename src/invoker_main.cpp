// Copyright (c) 2025, Ibrahim Erturk <me@erturk.me>, ErturkMe
// Licensed under the BSD 3-Clause License.
// See the LICENSE file in the project root for more information.

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <boost/asio.hpp>
#include "RBIC1RpcBridge.h"

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);

    // Create Boost.Asio's io_context
    boost::asio::io_context io_context;
    auto work_guard = boost::asio::make_work_guard(io_context);

    // Define the reader and writer topics
    QString reader_topic = "ReplyTopic";
    QString writer_topic = "RequestTopic";

    // Create the RBIC1RpcBridge instance
    RBIC1RpcBridge rpcBridge(io_context, reader_topic, writer_topic);

    // Set up the QML engine
    QQmlApplicationEngine engine;

    // set the import path for QML modules
    engine.addImportPath(":/erturk.me/imports");

    // Expose RBIC1RpcBridge to QML as "rpcBridge"
    engine.rootContext()->setContextProperty("rpcBridge", &rpcBridge);

    // Load the QML file
    const QUrl url(u"qrc:/erturk.me/imports/fastdds_qml_app/src/qml/main.qml"_qs);

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