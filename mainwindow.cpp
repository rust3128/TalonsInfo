#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "inputtalonsdialog.h"
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->frame->hide();
    ui->frameInfo->hide();
    crModelSerials();

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_actionInputNumber_triggered()
{
    InputTalonsDialog *inTlnDlg = new InputTalonsDialog();
    inTlnDlg->exec();
    listTalons=inTlnDlg->talonsListReturn();
    if(listTalons.length()==0) {
        QMessageBox::critical(0, qApp->tr("Ошибка ввода"),
                              QString("Не указаны номера талонов.\n"
                                      "Укажите заново и повторите попытку."),
                              QMessageBox::Ok);
        return;
    }
    crModelTalons();

//    qDebug() << listTalons.length();
}

void MainWindow::crModelSerials()
{
    QSqlDatabase dbc = QSqlDatabase::database("central");
    modelSerials = new QSqlQueryModel();
    QString strSql=QString("select s.CLIENT_ID, p.CLIENT_NAME,s.code, s.AMOUNT, s.REMARK "
                           "FROM PLG$TLN_SERIALS s "
                           "LEFT JOIN PLG$TLN_CLIENTS p ON p.CLIENT_ID=s.CLIENT_ID "
                           "where s.CLIENT_ID IN (select a.CLIENT_ID from PLG$TLN_CLIENTS a "
                           "where a.ISACTIVE='T') "
                           "order by p.CLIENT_ID, s.CODE");
    modelSerials->setQuery(strSql,dbc);
}

void MainWindow::crModelTalons()
{
    QString curNumber;
    long long talonsID, serial, code;
    int numRec=-1;

    ui->frame->show();
    modelTalons = new QStandardItemModel(this);
    modelTalons->setColumnCount(7);
    modelTalons->setHorizontalHeaderLabels(QStringList() << "Введено" << "Серия" << "Номер" << "ID"
                                           << "Тип" << "Литраж" << "Примечание");
    ui->tableViewTalons->setModel(modelTalons);

    foreach (curNumber, listTalons) {
        serial = curNumber.left(6).toLongLong();
        code = curNumber.right(curNumber.length()-6).toLongLong();
        for(int i=0; i<modelSerials->rowCount();i++) {
                if(modelSerials->data(modelSerials->index(i,2)).toUInt()==serial) {
                    numRec=i;
                    break;
                }
        }
        talonsID=getTalonID(serial,code,modelSerials->data(modelSerials->index(numRec,0)).toInt());
        QList<QStandardItem *> standardItemsList;
        standardItemsList.append(new QStandardItem(curNumber));
        standardItemsList.append(new QStandardItem(QString::number(serial)));
        standardItemsList.append(new QStandardItem(QString::number(code)));
        standardItemsList.append(new QStandardItem(QString::number(talonsID)));
        standardItemsList.append(new QStandardItem(modelSerials->data(modelSerials->index(numRec,1)).toString()));
        standardItemsList.append(new QStandardItem(modelSerials->data(modelSerials->index(numRec,3)).toString()));
        standardItemsList.append(new QStandardItem(modelSerials->data(modelSerials->index(numRec,4)).toString()));



        modelTalons->insertRow(modelTalons->rowCount(),standardItemsList);
    }
}

long long MainWindow::getTalonID(long long ser, long long num, int clientID)
{
    long long tlnID;
    tlnID=0;
    switch (clientID) {
    case 21:
        tlnID=ser*4294967295+num;
        break;
    case 24:
        tlnID=ser*1048576+num;
        break;
    default:
        break;
    }
    return tlnID;
}
