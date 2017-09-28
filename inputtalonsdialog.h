#ifndef INPUTTALONSDIALOG_H
#define INPUTTALONSDIALOG_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class InputTalonsDialog;
}

class InputTalonsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InputTalonsDialog(QWidget *parent = 0);
    ~InputTalonsDialog();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
private:
    void getTalonsNumbers();
private:
    Ui::InputTalonsDialog *ui;
    QStringList talonsNumber;
public:
    QStringList talonsListReturn();
};

#endif // INPUTTALONSDIALOG_H
