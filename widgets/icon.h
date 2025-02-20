#ifndef ICON_H
#define ICON_H

#include <QPushButton>
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QImage>
#include <QStyleOption>

#include <iostream>

class Icon : public QPushButton {
    Q_OBJECT

public:
    explicit Icon(const QImage &image, int size, QWidget *parent = nullptr)
        : QPushButton(parent), m_image(image) {
        setFixedSize(size, size);
    }

    void setImage(QImage &image) {
        std::cout << "icon edited" << std::endl;
        m_image = image;
        repaint();
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        Q_UNUSED(event);

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        QRectF rect(0, 0, width(), height());
        QPainterPath path;
        path.addRoundedRect(rect, 1000, 1000);

        painter.setClipPath(path);

        QPixmap pixmap = QPixmap::fromImage(m_image).scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        painter.drawPixmap(rect.toRect(), pixmap);

        painter.setPen(Qt::white);
        painter.drawText(rect, Qt::AlignCenter, text());
    }

private:
    QImage m_image;
};

#endif // ICON_H
