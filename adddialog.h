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
private slots:
    void generatePassword();
};

#endif // ADDDIALOG_H
