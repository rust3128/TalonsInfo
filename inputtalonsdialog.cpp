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

void InputTalonsDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    switch (ui->buttonBox->standardButton(button)) {
    case QDialogButtonBox::Ok:
        getTalonsNumbers();
        this->close();
        break;
    case QDialogButtonBox::Cancel:
        this->close();
        break;
    case QDialogButtonBox::Reset:
        ui->plainTextEdit->clear();
        break;
    default:
        break;
    }

}

void InputTalonsDialog::getTalonsNumbers()
{
    QString string = ui->plainTextEdit->toPlainText();
    talonsNumber = string.split(QRegExp("[\\s\\.\\,\\;]+"),QString::SkipEmptyParts);
    for(auto& str : talonsNumber)
            str = str.trimmed();
    ui->plainTextEdit->clear();
    ui->plainTextEdit->insertPlainText(talonsNumber.join('\n'));
}

QStringList InputTalonsDialog::talonsListReturn()
{
    return talonsNumber;
}
