#ifndef INPUTTALONSDIALOG_H
#define INPUTTALONSDIALOG_H

#include <QDialog>

namespace Ui {
class InputTalonsDialog;
}

class InputTalonsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InputTalonsDialog(QWidget *parent = 0);
    ~InputTalonsDialog();

private:
    Ui::InputTalonsDialog *ui;
};

#endif // INPUTTALONSDIALOG_H
