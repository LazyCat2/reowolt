#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>

class Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(QObject *parent = nullptr);
    std::string download(const QUrl &url, const std::string);

private:
    QNetworkAccessManager *manager;
    QMap<QUrl, QNetworkReply*> ongoingDownloads;

    void save(const QString &, const QByteArray &);

signals:
};

#endif // IMAGEDOWNLOADER_H
