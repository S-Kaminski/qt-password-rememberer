#include "editdialog.h"
#include "ui_editdialog.h"
#include <QMessageBox>

int index;
editdialog::editdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editdialog)
{
    ui->setupUi(this);
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveChanges()));
}

editdialog::~editdialog()
{
    delete ui;
}

void editdialog::receiveCrdentials(int row, QString website, QString login, QString password)
{
    ui->websiteEdit->setText(website);
    ui->loginEdit->setText(login);
    ui->passwordEdit->setText(password);
    qDebug() << "signal from mainwindow " << website << " => ROW: " << row;
    index = row;
}

void editdialog::saveChanges()
{
    if(ui->websiteEdit->text().length()>0 && ui->loginEdit->text().length()>0 && ui->passwordEdit->text().length()>0)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Save changes", "Are you sure you want to save this changes?",QMessageBox::Yes|QMessageBox::Cancel);
        if(reply == QMessageBox::Yes)
        {
            emit sendChanges(index,ui->websiteEdit->text(),ui->loginEdit->text(),ui->passwordEdit->text());
            this->close();
        }
    }
    else QMessageBox::information(this, tr("Provide information"), tr("Fields cannot be empty! Each information must be provided."));
}

void editdialog::cancelChanges()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Cancel changes", "Are you sure you want to close this window? All implied changes will be discarded.",QMessageBox::Yes|QMessageBox::Cancel);
    if(reply == QMessageBox::Yes)
    {
        this->close();
    }

}
