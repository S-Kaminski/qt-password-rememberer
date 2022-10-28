#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>

namespace Ui {
class adddialog;
}

class adddialog : public QDialog
{
    Q_OBJECT

public:
    explicit adddialog(QWidget *parent = nullptr);
    ~adddialog();

private:
    Ui::adddialog *ui;
public slots:
private slots:
    void generatePassword();
    void saveCredentials();

signals:
    void sendData(QString,QString,QString);

};

#endif // ADDDIALOG_H
