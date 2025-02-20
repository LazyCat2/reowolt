#ifndef CLIENT_H
#define CLIENT_H

#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include "cache.h"

class Client : public QObject
{
    Q_OBJECT

private:
    QWebSocket m_webSocket;
    QNetworkAccessManager network;
    std::string token;

    void onConnected();
    void onMessage(QString);
    void onError(QAbstractSocket::SocketError);
    QNetworkRequest makeRequest(std::string);

public:
    Client(std::string);

    void connect_to_ws();
    void printState();
    void onMessageRecieved(Message);
    void onReady(ReadyData);
    void fetchMessages(channel_id, std::function<void(std::vector<Message>)>);
    void sendMessage(std::string, channel_id);

    Cache cache;

    std::map<std::string, std::function<void(nlohmann::json)>> events;

signals:
    void ready();
    void message_recieve(Message);
};

#endif // CLIENT_H
