#include "uimessage.h"

#include <QLabel>
#include <QTextEdit>
#include "./icon.h"
#include "../downloader.h"

UiMessage::UiMessage(
    message_id id,
    Cache *cache,
    Downloader *downloader
    ) : QHBoxLayout() {
    auto m_msg = cache->message(id);
    if (!m_msg.has_value()) return;
    Message message = m_msg.value();

    auto m_user = cache->user(message.author);
    std::string m_username = m_user.has_value() ? m_user.value().display() : message.author;

    QLabel *username = new QLabel(m_username.c_str());
    QLabel *content = new QLabel(message.content.c_str());

    auto font = username->font();
    font.setBold(true);
    username->setFont(font);

    content->setTextInteractionFlags(Qt::TextSelectableByMouse);
    content->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    content->setWordWrap(true);
    setContentsMargins(0, 0, 0, 10);

    auto *text_container = new QVBoxLayout();

    text_container->addWidget(username);
    text_container->addWidget(content);
    auto path = downloader->download(QUrl(m_user->avatar.static_image_url().c_str()), m_user->avatar.id);
    auto *user_avatar = new Icon(QImage(path.c_str()), 40);
    user_avatar->setContentsMargins(10, 10, 10, 10);

    addWidget(user_avatar);
    addLayout(text_container);
}
