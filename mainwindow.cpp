#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QtCore>
#include <iostream>
#include <QIODevice>
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>

struct Password
{
    QString website;
    QString login;
    QString password;
    Password(QString _website, QString _login, QString _password)
    {
        website = _website;
        login = _login;
        password = _password;
    }
};
QString mainPassword;
QList<Password> list;
editdialog *edit;
adddialog *add;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->toogleVisibility->setCheckable(true);
    ui->tableWidget->setColumnWidth(0,143);
    ui->tableWidget->setColumnWidth(1,143);
    ui->tableWidget->setColumnWidth(2,250);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior( QAbstractItemView::SelectRows );
    ui->tableWidget->setSelectionMode( QAbstractItemView::SingleSelection );
    loadData();
    //connect(sender, SIGNAL(destroyed()), this, SLOT(objectDestroyed()));
    edit = new editdialog(this);
    add = new adddialog(this);
    connect(ui->toogleVisibility, SIGNAL(stateChanged(int)), this, SLOT(togglePasswords(int)));
    connect(ui->loadButton, SIGNAL(clicked()), this, SLOT(loadData()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveData()));
    connect(ui->editButton, SIGNAL(clicked()), this, SLOT(editContent()));
    connect(this, SIGNAL(sendData(int,QString,QString,QString)), edit, SLOT(receiveCrdentials(int,QString,QString,QString))); //edit
    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addNewCredentials()));
    connect(ui->deleteButton, SIGNAL(clicked()), this, SLOT(deleteCredentials()));
    connect(ui->deleteAllButton, SIGNAL(clicked()), this, SLOT(deleteAllButton()));
    connect(ui->aboutAuthor, SIGNAL(clicked()), this, SLOT(aboutAuthor()));
    connect(add, SIGNAL(sendData(QString,QString,QString)), this, SLOT(getNewCredentials(QString,QString,QString)));
    connect(edit, SIGNAL(sendChanges(int,QString,QString,QString)), this, SLOT(getChanges(int,QString,QString,QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getChanges(int _index, QString _website, QString _login, QString _password)
{
    qDebug() << "Changes should be received, index: " << _index;
    struct Password changedCredentials = {_website, _login, _password};
    list[_index] = changedCredentials;
    supplyTable();
}
void MainWindow::addNewCredentials()
{
    add->show();
}

void MainWindow::getNewCredentials(QString _website, QString _login, QString _password)
{
    qDebug() << "Received new credentials from add-dialog";
    struct Password passwd = { _website, _login, _password };
    list.append(passwd);
    supplyTable();
}

void MainWindow::deleteAllButton()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Remove ALL credentials", "Are you sure you want to delete ALL credentials? This action does not make automatically impact on file; you can re-load passwords until you perform an action (add, edit or delete) which auto saves it.",QMessageBox::Yes|QMessageBox::Cancel);
    if(reply == QMessageBox::Yes)
    {
        ui->tableWidget->setRowCount(0); // clear tableWidget
        updateList();
    }

}

void MainWindow::deleteCredentials()
{
    auto selected = ui->tableWidget->selectionModel()->selectedRows();
    bool toogled = ui->toogleVisibility->isChecked();
    if(toogled) ui->toogleVisibility->setChecked(false);
    qDebug() << "Selected count => " << selected.count();
    if(selected.count() > 0)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Remove credentials", "Are you sure you want to delete this credentials? This acction is irreversible.",QMessageBox::Yes|QMessageBox::Cancel);
        if(reply == QMessageBox::Yes)
        {
            int row = selected.constFirst().row();
            qDebug() << "from deleteCredentials row: "<< row;
            ui->tableWidget->removeRow(row);
            updateList();
            if(toogled) ui->toogleVisibility->setChecked(true);
        }

    }
}
void MainWindow::loadData()
{
    QFile file("C://Users//User//Documents//PasswordsRememberer//passwords.json");
    list.clear();
    qDebug() << "Exist? " << file.exists();
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray bytes = file.readAll();
        qDebug() << bytes;
        file.close();

        QJsonParseError jsonError;
        QJsonDocument document = QJsonDocument::fromJson(bytes, &jsonError);
        if(jsonError.error != QJsonParseError::NoError)
        {
            qDebug()  << "fromJson failed: " << QString(jsonError.errorString());
            return;
        }
        if(document.isObject())
        {
            QJsonObject jsonObj = document.object();
            bool ok;
            mainPassword = QInputDialog::getText(this,"Veryfication", "Enter password:", QLineEdit::Password,"",&ok);
            QJsonArray jsonArray = jsonObj.value(QString(mainPassword)).toArray();
            if(jsonArray.count() > 0)
            {
                qDebug() << jsonArray.count();
                for(int i = 0; i < jsonArray.count(); i++)
                {
                    qDebug() << "HELLO!";
                    //struct Password credential = {"", "", ""};
                    QJsonObject obj1 = jsonArray[i].toObject();
                    foreach(const QString& key, obj1.keys())
                    {
                        struct Password credential = {"", "", ""};
                        QJsonValue value = obj1.value(key);
                        QJsonArray credentialArray = value.toArray();
                        foreach(const QJsonValue & v, credentialArray)
                        {
                            credential.website = key;
                            credential.login = v.toObject().value("login").toString();
                            credential.password = v.toObject().value("password").toString();
                            qDebug() << "Credentials: " << credential.website << ": " << "login: " << credential.login << ", password: " << credential.password;
                            list.append(credential);
                        }
                    }
                }
            }
            else
            {
                QMessageBox::warning(this, tr("Error"), tr("Wrong password! Cannot get access."));
            }

        }
    }
    supplyTable();
}

void MainWindow::updateList()
{
    int rows = ui->tableWidget->rowCount();
    list.clear();
    qDebug() << "clear list!";
    for(int i = 0; i < rows; i++)
    {
        struct Password credential = {ui->tableWidget->item(i,0)->text(), ui->tableWidget->item(i,1)->text(), ui->tableWidget->item(i,2)->text()};
        list.append(credential);
        qDebug() << "Append to list!";
    }
}

void MainWindow::supplyTable()
{
    qDebug() << "Supply table!";
    int row = 0;
    ui->tableWidget->setRowCount(list.count());
    ui->tableWidget->setColumnCount(3);

    foreach(const Password passwd, list)
    {
        QTableWidgetItem* item = new QTableWidgetItem();
        item->setText(passwd.website);
        ui->tableWidget->setItem(row,0,item);

        QTableWidgetItem* item1 = new QTableWidgetItem();
        item1->setText(passwd.login);
        ui->tableWidget->setItem(row,1,item1);

        QTableWidgetItem* item2 = new QTableWidgetItem();
        item2->setText(passwd.password);
        ui->tableWidget->setItem(row,2,item2);
        row++;
    }
}

void MainWindow::tooglePasswordsView()
{
    for(int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        QTableWidgetItem *passwordItem = ui->tableWidget->item(i,2);
        passwordItem->setText("*H*I*D*D*E*N*");
    }

}

void MainWindow::saveData()
{
    qDebug() << "saveData signal";
    if(mainPassword.length()>0)
    {
        QJsonObject passwordsFile;
        passwordsFile.insert(mainPassword,"");
        QJsonArray passwordsArray;

        QJsonObject websiteObj;

        for(int i = 0; i < list.count(); i++)
        {
            struct Password credential = list.at(i);
            QJsonArray websitesArray;
            auto loginAndPasswordPair = QJsonObject({
                qMakePair(QString("login"), credential.login),
                qMakePair(QString("password"), credential.password)
            });
            websitesArray.push_back(QJsonValue(loginAndPasswordPair));
            websiteObj.insert(QString(credential.website), QJsonValue(websitesArray));
        }
        passwordsArray.push_back(QJsonValue(websiteObj)); // ?????
        passwordsFile.insert(QString(mainPassword), passwordsArray);

        QJsonDocument mainDocument;
        mainDocument.setObject(passwordsFile);
        QByteArray bytes = mainDocument.toJson(QJsonDocument::Indented);
        qDebug() << "NEW DOCUMENT" << bytes;
        QFile file("C://Users//User//Documents//PasswordsRememberer//passwords.json");
        if( file.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate ) )
        {
            QTextStream iStream(&file);
            auto toUtf8 = QStringDecoder(QStringDecoder::Utf8);
            QString string = toUtf8(bytes);
            iStream << string;
            file.close();
        }
    }
    else
        QMessageBox::information(this, tr("Missing password"), tr("Must load data in order to save changes. No main password detected!"));
    //-----------foreach?
//    struct Password cred = {"1.1", "1.2", "1.3"};
//    QJsonArray websitesArray;
//    auto loginAndPasswordPair = QJsonObject({
//        qMakePair(QString("login"), cred.login),
//        qMakePair(QString("password"), cred.password)
//    });
//    websitesArray.push_back(QJsonValue(loginAndPasswordPair));
//    websiteObj.insert(QString(cred.website), QJsonValue(websitesArray)); //array pair website
    //--------------2nd
//    struct Password cred1 = {"2.1", "2.2", "2.3"};
//    QJsonArray websitesArray1;
//    auto loginAndPasswordPair1 = QJsonObject({
//        qMakePair(QString("login"), cred1.login),
//        qMakePair(QString("password"), cred1.password)
//    });
//    websitesArray1.push_back(QJsonValue(loginAndPasswordPair1));
//    websiteObj.insert(QString(cred1.website), QJsonValue(websitesArray1)); //array pair website
    // ---------- end foreach
}


void MainWindow::togglePasswords(int checked)
{
    qDebug() << "checkedStatus" << checked;
    if(checked==2) tooglePasswordsView();
    else supplyTable();

}

void MainWindow::aboutAuthor()
{
    QMessageBox::information(this, tr("About author"), tr("Application by Szymon Kamiński."));
}

void MainWindow::editContent()
{
    auto selected = ui->tableWidget->selectionModel()->selectedRows();
    qDebug() << "Selected count => " << selected.count();
    if(selected.count() > 0)
    {
        int row = selected.constFirst().row();
        qDebug() << "from editContent // row: "<< row;
        //ui->tableWidget->removeRow(row);
        struct Password editable = list.at(row);
        emit this->sendData(row, editable.website, editable.login, editable.password);
        edit->show();// updateList();
    }
}

