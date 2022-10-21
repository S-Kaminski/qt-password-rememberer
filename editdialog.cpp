#include "editdialog.h"
#include "ui_editdialog.h"

editdialog::editdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editdialog)
{
    ui->setupUi(this);
}

editdialog::~editdialog()
{
    delete ui;
}

void editdialog::test()
{
    qDebug() << "Holy shit! Slots are op";
}
void editdialog::receiveCrdentials(QString website, QString login, QString password)
{
    ui->websiteEdit->setText(website);
    ui->loginEdit->setText(login);
    ui->passwordEdit->setText(password);
    qDebug() << "Holy shit! Slots are op ==> " + website;
}
