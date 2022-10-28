#include "adddialog.h"
#include "mainwindow.h"
#include "ui_adddialog.h"
#include <QMessageBox>

adddialog::adddialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::adddialog)
{
    ui->setupUi(this);
    ui->passwordLength->setValue(7);
    connect(ui->generatePassword, SIGNAL(clicked()), this, SLOT(generatePassword()));
    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(saveCredentials()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(close()));

}

adddialog::~adddialog()
{
    delete ui;
}

void adddialog::saveCredentials()
{
    if(ui->websiteInput->text().length()>0 && ui->loginInput->text().length()>0 && ui->passwordInput->text().length()>0)
    {
        qDebug() << "All information provided!";
        emit sendData(ui->websiteInput->text(),ui->loginInput->text(),ui->passwordInput->text());
        ui->websiteInput->setText("");
        ui->loginInput->setText("");
        ui->passwordInput->setText("");
        this->close();
    }
    else QMessageBox::information(this, tr("Missing information"), tr("All fields must be filled!"));
}

void adddialog::generatePassword()
{
    const char alphanum[] = "0123456789!@#$%^&*abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int chars = sizeof(alphanum)-1;
    int passwordLength = ui->passwordLength->value();
    QString newPassword = "";
    srand(time(0)+rand());
    for(int i = 0; i < passwordLength; i++)
    {
        newPassword += alphanum[rand() % chars];
    }
    ui->passwordInput->setText(newPassword);
    qDebug() << newPassword;
}
