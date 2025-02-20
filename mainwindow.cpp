#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QSettings>
#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QIcon>
#include "client/client.h"
#include "widgets/icon.h"
#include "widgets/uimessage.h"
#include <QTimer>

MainWindow::MainWindow(QSettings *settings)
    : QMainWindow(),
    ui(new Ui::MainWindow),
    client(settings->value("token").toString().toStdString())
    {
    this->settings = settings;
    ui->setupUi(this);

    // ui->servers_scroll->setContentsMargins(0, 0, 0, 0);
    connect(&client, &Client::ready, this, &MainWindow::on_ready);
    connect(&client, &Client::message_recieve, this, &MainWindow::on_message);
    client.connect_to_ws();
    connect(ui->send_message, &QPushButton::clicked, this, &MainWindow::send_message);
}

void MainWindow::on_ready() {
    auto self_avatar = client.cache.user(client.cache.self)->avatar;
    auto path = downloader.download(QUrl(self_avatar.static_image_url().c_str()), self_avatar.id);
    std::cout << client.cache.self << " " << path << std::endl;
    auto user_icon = new Icon(QImage(path.c_str()), 50);
    ui->top_server_button->addWidget(user_icon);

    for (auto &server_pair: client.cache.servers) {
        auto &server = server_pair.second;
        std::string icon_path = downloader.download(QUrl(server.icon.static_image_url().c_str()), server.icon.id);
        auto *server_icon = new Icon(QImage(icon_path.c_str()), 50, this);
        auto *server_icon_container = new QHBoxLayout;

        server_icon->setProperty("server", server.id.c_str());
        server_icon->setToolTip(server.name.c_str());
        server_icon->setToolTipDuration(0);

        server_icon_container->setProperty("class", "servericon");
        server_icon->setContentsMargins(10, 10, 10, 10);

        connect(server_icon, &QPushButton::clicked, [this, server_icon] {
            set_server(server_icon->property("server").toString().toStdString());
        });

        server_icon_container->addWidget(server_icon);

        ui->server_icons->addLayout(server_icon_container);
    }

    for (auto channel : client.cache.channels) {
        if (channel.second.channel_type != "TextChannel") {
            continue;
        }
        set_channel(channel.second.id);
        break;
    }
}

void MainWindow::on_message(Message message) {
    if (message.channel != current_channel) {
        if (message.content.empty()) return;

        // Create a system tray icon
        QSystemTrayIcon trayIcon;
        trayIcon.setIcon(QIcon::fromTheme("dialog-information")); // Set an icon for the tray
        trayIcon.setVisible(true);

        // Create a context menu for the tray icon
        QMenu menu;
        QAction *quitAction = menu.addAction("Quit");
        QObject::connect(quitAction, &QAction::triggered, this, &QApplication::quit);
        trayIcon.setContextMenu(&menu);

#define ON_MSG_CACHE_OR_ABORT(tmp_variable, variable, thing, get_name) \
        auto tmp_variable = client.cache.thing; \
        if (!tmp_variable.has_value()) { \
        std::cout << #tmp_variable << " is none" << std::endl; \
        } \
        std::string variable = tmp_variable->get_name;

        ON_MSG_CACHE_OR_ABORT(user, username, user(message.author), display())
        ON_MSG_CACHE_OR_ABORT(channel, channel_name, channel(message.channel), name)
        ON_MSG_CACHE_OR_ABORT(server, server_name, server(channel->server), name)

        // Show a notification
        trayIcon.showMessage(("@"+username+" in #"+channel_name + " (" + server_name + ")").c_str(), message.content.c_str(), QSystemTrayIcon::Information, 3000);
    } else {
        UiMessage *msg = new UiMessage(message.id, &client.cache, &downloader);
        ui->messages->addLayout(msg);
    }
}

void MainWindow::set_channel(channel_id cnl) {
    if (current_channel == cnl) return;
    auto m_cnl = client.cache.channel(cnl);
    if (!m_cnl.has_value()) return;

    current_channel = cnl;

    Channel channel = *m_cnl;
    if (current_server != channel.server) {
        set_server(channel.server);
    }

    QLayoutItem *item;
    while ((item = ui->messages->takeAt(0)) != nullptr) {
        auto *layout = item->layout();

        while ((item = layout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }

        layout->deleteLater();
        layout->setParent(nullptr);

        delete layout;
    }

    // It does not works, it still shows messages
    // from different channels overlaping each other.
    // Weird thing is that channel list
    // with same code that removes items works perfect.
    // Also, images are not overlaped, only text.
    ui->messages->parentWidget()->update();
    ui->messages->update();

    auto prev_cnl = current_channel;
    client.fetchMessages(
        current_channel,
        [this](std::vector<Message> messages){onMessagesFetched(messages);}
    );

    ui->channel_name->setText(channel.name.c_str());
}

void MainWindow::onMessagesFetched(std::vector<Message> messages) {
    for (Message message : messages) {
        if (message.channel != current_channel) break;

        UiMessage *msg = new UiMessage(message.id, &client.cache, &downloader);
        ui->messages->insertLayout(0, msg);
    }
}

void MainWindow::set_server(server_id srv) {
    auto m_srv = client.cache.server(srv);
    if (!m_srv.has_value()) return;

    Server server = *m_srv;

    ui->server_name->setText(server.name.c_str());
    current_server = srv;

    QLayoutItem *item;
    while ((item = ui->channels->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    for (channel_id cnl_id : server.channels) {
        auto m_cnl = client.cache.channel(cnl_id);
        if (!m_cnl.has_value()) continue;

        Channel channel = m_cnl.value();
        QPushButton *button = new QPushButton();

        button->setText(channel.name.c_str());
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        button->setStyleSheet("text-align:left; padding:10px 15px;");
        button->setMaximumHeight(50);
        button->setProperty("class", "secondary");
        button->setProperty("channel", cnl_id.c_str());
        connect(button, &QPushButton::clicked, [this, button] {
            set_channel(button->property("channel").toString().toStdString());
        });

        ui->channels->addWidget(button);
    }

    if (!server.channels.empty()) {
        set_channel(server.channels[0]);
    }
}

void MainWindow::send_message() {
    std::string text = ui->message_input->toPlainText().toStdString();
    ui->message_input->setText("");
    client.sendMessage(text, current_channel);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (
        (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
        && !(event->modifiers() & Qt::ShiftModifier)
    ) {
        send_message();
    }
}

MainWindow::~MainWindow() {
    delete ui;
}
