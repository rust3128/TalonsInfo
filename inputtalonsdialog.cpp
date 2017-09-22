#include "inputtalonsdialog.h"
#include "ui_inputtalonsdialog.h"

InputTalonsDialog::InputTalonsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputTalonsDialog)
{
    ui->setupUi(this);
}

InputTalonsDialog::~InputTalonsDialog()
{
    delete ui;
}
