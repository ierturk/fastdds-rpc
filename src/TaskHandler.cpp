#include "TaskHandler.h"
#include <QDebug>
#include <boost/asio/steady_timer.hpp>
#include <chrono>

TaskHandler::TaskHandler(boost::asio::io_context& io_context, QObject* parent)
    : QObject(parent), io_context_(io_context) {}

void TaskHandler::startTask(const QString& input) {
    qDebug() << "TaskHandler::startTask called with input:" << input;

    // Debug before spawning the coroutine
    qDebug() << "Spawning coroutine for handleTask...";
    boost::asio::co_spawn(io_context_, handleTask(input), boost::asio::detached);
}

boost::asio::awaitable<void> TaskHandler::handleTask(const QString& input) {
    auto executor = co_await boost::asio::this_coro::executor;
    qDebug() << "TaskHandler::handleTask started with executor:" << &executor;

    // Create a timer to simulate a long-running task
    boost::asio::steady_timer timer(executor, std::chrono::seconds(2));

    // Wait asynchronously for the timer to expire
    co_await timer.async_wait(boost::asio::use_awaitable);

    QString result = "Processed: " + input;
    qDebug() << "TaskHandler::handleTask async operation completed with result:" << result;

    // Emit the result back to QML
    emit taskCompleted(result);

    qDebug() << "TaskHandler::handleTask coroutine completed.";
}