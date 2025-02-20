// #include "icon.h"

// #include <iostream>

// #include <QPainter>
// #include <QPainterPath>

// Icon::Icon(
//     std::string image_path,
//     int size,
//     QWidget *parent
// )
//     : QPushButton{parent}
// {
//     QPainter painter(this);
//     QImage icon(image_path.c_str());
//     QImage roundedImage(icon.size(), QImage::Format_ARGB32);
//     roundedImage.fill(Qt::transparent);
//     QRectF rect(0, 0, roundedImage.width(), roundedImage.height());
//     QPainterPath path;

//     path.addRoundedRect(rect, 1000, 1000);
//     painter.setClipPath(path);
//     painter.drawImage(0, 0, icon);

//     // setPixmap(QPixmap::fromImage(roundedImage).scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
//     // setScaledContents(true);
//     setFixedSize(size, size);
//     setProperty("class", "rounded");
// }
