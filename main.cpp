#include "style.h"
#include "login.h"
#include "mainwindow.h"

#include <QApplication>
#include <QDir>
#include <QSettings>
#include <QPushButton>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QWidget>
#include <QVBoxLayout>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setStyleSheet(STYLE_CSS);

    QSettings settings(QString(QDir::home().absolutePath() + "/.config/reowolt/config.ini"), QSettings::IniFormat);
    QString token = settings.value("token", "").toString();
    QMainWindow *mainWidget = nullptr;

    if (token.isEmpty()) {
        mainWidget = new Login(&settings);
    } else {
        mainWidget = new MainWindow(&settings);
    }

    mainWidget->show();
    return a.exec();
}
