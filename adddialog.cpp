#include "adddialog.h"
#include "ui_adddialog.h"

adddialog::adddialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::adddialog)
{
    ui->setupUi(this);
    connect(ui->generatePassword, SIGNAL(clicked()), this, SLOT(generatePassword()));
    ui->passwordLength->setValue(7);
}

adddialog::~adddialog()
{
    delete ui;
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
