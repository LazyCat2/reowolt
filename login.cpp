#include "login.h"
#include "./ui_login.h"
#include <QSettings>

Login::Login(QSettings *settings)
    : QMainWindow()
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    this->settings = settings;
    ui->token_input->setText(settings->value("token").toString());
    connect(ui->login_button, &QPushButton::clicked, this, &Login::on_login_button_clicked);
}

void Login::on_login_button_clicked() {
    settings->setValue("token", ui->token_input->text());
}

Login::~Login()
{
    delete ui;
}
