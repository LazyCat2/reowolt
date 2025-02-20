#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QPushButton>
#include <QKeyEvent>

#include "client/client.h"
#include "client/cache.h"
#include "downloader.h"
#include "widgets/icon.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QSettings*);
    ~MainWindow();

private slots:
    void on_ready();
    void on_message(Message);
    void send_message();
    void keyPressEvent(QKeyEvent *) override;

private:
    Ui::MainWindow *ui;
    QSettings *settings;
    Client client;
    Downloader downloader;
    server_id current_server;
    channel_id current_channel;

    void set_channel(channel_id);
    void set_server(server_id);
    void onMessagesFetched(std::vector<Message>);
};

#endif // MAINWINDOW_H
