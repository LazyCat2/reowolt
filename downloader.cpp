#include "downloader.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QDir>
#include <iostream>

Downloader::Downloader(QObject *parent)
    : QObject{parent}
{
    manager = new QNetworkAccessManager(this);
}

std::string Downloader::download(const QUrl &url, const std::string fileName) {
    if (url.toString().toStdString().empty()) return "";

    std::string output = QDir::home().absolutePath().toStdString() + "/.cache/reowolt/" + fileName;
    if (QFile(QString(output.c_str())).exists()) return output;

    std::cout << "Downloading " << url.toString().toStdString() << " to " << output << "..." << std::endl;
    if (ongoingDownloads.contains(url)) {
        qDebug() << "Download already ongoing for:" << url.toString();
        return output;
    }

    ongoingDownloads.insert(url, nullptr);
    QNetworkRequest request(url);
    QNetworkReply *reply = manager->get(request);
    ongoingDownloads[url] = reply;

    connect(reply, &QNetworkReply::finished, this, [this, url, reply, output]() {
        std::cout << "yay :D (" << reply->error() << ")" << std::endl;
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();
            std::cout << data.size() << std::endl;
            if (data.size() < 1000) {
                std::cout << data.data() << std::endl;
            }
            save(QString(output.c_str()), data);
        } else {
            qDebug() << "Error downloading:" << reply->errorString();
        }
        ongoingDownloads.remove(url);
        reply->deleteLater();
    });

    return output;
}

void Downloader::save(const QString &fileName, const QByteArray &data) {
    QFile file(fileName);
    QFileInfo(file).absoluteDir().mkpath(".");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(data);
        file.close();
        std::cout << "saved as: " << fileName.toStdString() << std::endl;
    } else {
        std::cout << "Failed to save: " << file.errorString().toStdString() << std::endl;
    }
}
