#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "editdialog.h"
#include "adddialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void getNewCredentials(QString, QString,QString);
private slots:
    void addNewCredentials();
    void deleteCredentials();
    void supplyTable();
    void loadData();
    void saveData();
    void updateList();
    void tooglePasswordsView();
    void togglePasswords(int checked);
    void aboutAuthor();
    void editContent();
    void deleteAllButton();
    void getChanges(int,QString,QString,QString);

signals:
    void sendData(int, QString, QString, QString);
private:
    Ui::MainWindow *ui;
    editdialog *editui;

};
#endif // MAINWINDOW_H
