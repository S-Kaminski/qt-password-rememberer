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
    void receiveCrdentials(QString, QString, QString);
private slots:
    void test();
};

#endif // EDITDIALOG_H
