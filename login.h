#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui {
class Login;
}
QT_END_NAMESPACE

class Login : public QMainWindow
{
    Q_OBJECT

public:
    Login(QSettings*);
    ~Login();

private slots:
    void on_login_button_clicked();

private:
    QSettings *settings;
    Ui::Login *ui;
};

#endif // LOGIN_H
