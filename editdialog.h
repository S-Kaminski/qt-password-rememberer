#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QDialog>

namespace Ui {
class editdialog;
}

class editdialog : public QDialog
{
    Q_OBJECT

public:
    explicit editdialog(QWidget *parent = nullptr);
    ~editdialog();

private:
    Ui::editdialog *ui;
public slots:
    void receiveCrdentials(int, QString, QString, QString);
    void cancelChanges();
    void saveChanges();
private slots:
signals:
    void sendChanges(int,QString,QString,QString);
};

#endif // EDITDIALOG_H
