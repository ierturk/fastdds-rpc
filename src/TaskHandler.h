#ifndef TASKHANDLER_H
#define TASKHANDLER_H

#include <QObject>
#include <QString>
#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>

class TaskHandler : public QObject {
    Q_OBJECT
public:
    explicit TaskHandler(boost::asio::io_context& io_context, QObject* parent = nullptr);

    Q_INVOKABLE void startTask(const QString& input);

signals:
    void taskCompleted(const QString& result);

private:
    boost::asio::awaitable<void> handleTask(const QString& input);

    boost::asio::io_context& io_context_;
};

#endif // TASKHANDLER_H