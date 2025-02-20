#include "client.h"
#include <iostream>
#include "nlohmann/json.hpp"
#include "api_structs.h"
#include <functional>
#include <QJsonObject>
#include <QJsonDocument>

#define BASE_API_URL "https://api.revolt.chat"

Client::Client(std::string token)
    : QObject() {
    this->token = token;
}

void Client::connect_to_ws() {
    connect(&m_webSocket, &QWebSocket::connected, this, &Client::onConnected);
    connect(&m_webSocket, &QWebSocket::errorOccurred, this, &Client::onError);
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &Client::onMessage);

    m_webSocket.open(QUrl("wss://app.revolt.chat/events"));
}

void Client::onConnected() {
    m_webSocket.sendTextMessage(QString::fromStdString(R"({"type": "Authenticate", "token": ")" + this->token + R"("} )"));
}

void Client::onMessage(QString message) {
    std::string raw_data = message.toStdString();
    nlohmann::json json_data = nlohmann::json::parse(raw_data);
    // std::cout << raw_data << std::endl;
    if (json_data["type"] == "Message") {
        onMessageRecieved(Message(json_data));
        auto user = json_data.at("user");
        if (user.is_object()) {
            cache.users[user.at("_id")] = user;
        }
    } else if (json_data["type"] == "Ready") {
        for (auto user : json_data["users"]) {
            if (user["relationship"] == "User") {
                cache.self = user["_id"];
                std::cout << user["_id"] << " " << cache.self << std::endl;
                break;
            }
        }

        onReady(ReadyData(json_data));
    }
}

void Client::onError(QAbstractSocket::SocketError error) {
    std::cout << "Error: " << error << std::endl;
}

void Client::onMessageRecieved(Message message) {
    cache.messages[message.id] = message;
    emit message_recieve(message);
}

void Client::onReady(ReadyData readyData) {
    //cache.variable.insert(cache.variable.end(), readyData.variable.begin(), readyData.variable.end());

    #define CLIENT_PUSH_DATA_FROM_READY(variable) \
    for (auto& item : readyData.variable) { \
        cache.variable[item.id] = item; \
    }

    CLIENT_PUSH_DATA_FROM_READY(servers)
    CLIENT_PUSH_DATA_FROM_READY(channels)
    CLIENT_PUSH_DATA_FROM_READY(users)

    emit ready();
}

QNetworkRequest Client::makeRequest(std::string url) {
    QNetworkRequest request(QUrl((BASE_API_URL + url).c_str()));
    request.setRawHeader("X-Session-Token", token.c_str());
    return request;
}

void Client::sendMessage(std::string content, channel_id channel) {
    auto request = makeRequest("/channels/" +channel+ "/messages");
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject message;
    message["content"] = content.c_str();
    QByteArray jsonData = QJsonDocument(message).toJson();
    auto reply = network.post(request, jsonData);

    // connect(reply, &QNetworkReply::finished, [reply]() {

    // });
}

void Client::fetchMessages(channel_id channel, std::function<void(std::vector<Message>)> callback) {
    auto reply = network.get(makeRequest("/channels/" +std::string(channel)+ "/messages?include_users=true"));
    connect(reply, &QNetworkReply::finished, [this, reply, callback]() {
        if (reply->error() == QNetworkReply::NoError) {
            auto text = reply->readAll().toStdString();
            auto json_response = nlohmann::json::parse(text);

            std::vector<Message> messages;
            for (auto &msg : json_response.at("messages")) {
                Message m(msg);
                messages.push_back(m);
                cache.messages[m.id] = m;
            }

            for (auto &user : json_response.at("users")) {
                User u(user);
                cache.users[u.id] = u;
            }

            callback(messages);
        }

        reply->deleteLater();
    });
}


