#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "inputtalonsdialog.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    modTalon = new QSqlQueryModel();
    modelTalons = new QStandardItemModel(this);
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
    ui->frameInfo->hide();
    if(modTalon->rowCount()!=0) modTalon->clear();
    if(modelTalons->rowCount()!=0) modelTalons->clear();
    crModelTalons();

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
//    modelTalons = new QStandardItemModel(this);
    modelTalons->setColumnCount(7);
    modelTalons->setHorizontalHeaderLabels(QStringList() << "Введено" << "Серия" << "Номер" << "ID Талона"
                                           << "Тип талона" << "Литраж" << "Примечание");
    ui->tableViewTalons->setModel(modelTalons);

    ui->tableViewTalons->verticalHeader()->hide();



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
        standardItemsList.append(new QStandardItem(modelSerials->data(modelSerials->index(numRec,0)).toString()));



        modelTalons->insertRow(modelTalons->rowCount(),standardItemsList);
    }
    ui->tableViewTalons->resizeColumnsToContents();
    ui->tableViewTalons->hideColumn(7);
//    ui->tableViewSerials->horizontalHeader()->setStretchLastSection(true);
    ui->tableViewTalons->verticalHeader()->setDefaultSectionSize(ui->tableViewTalons->verticalHeader()->minimumSectionSize());
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

void MainWindow::on_tableViewTalons_doubleClicked(const QModelIndex &idx)
{
    numberTalon = modelTalons->data(modelTalons->index(idx.row(),0)).toString();
    clientNumber = modelTalons->data(modelTalons->index(idx.row(),7)).toInt();
    bool mposOff=false, dbaseOff = false;
    QString message;
    ui->frameInfo->show();
    ui->pushButton->setEnabled(true);
    ui->pushButton->setToolTip("Активация талона "+ numberTalon +" !");
    long long talonID=modelTalons->data(modelTalons->index(idx.row(),3)).toLongLong();
    ui->labelTalonInfo->setText("Информация по талону " + numberTalon);
        QSqlDatabase dbto = QSqlDatabase::database("talonsonline");
        QSqlDatabase dbfb = QSqlDatabase::database("central");
        QString strSql, strCloseData, strMpos;
        QSqlQuery q = QSqlQuery(dbto);
        QSqlQuery qc = QSqlQuery(dbfb);
        strMpos=QString("select a.TERMINAL_ID, a.dat from PLG$TLN_FTALONS a "
                        "WHERE a.TALON_ID=%1").arg(talonID);

        strCloseData=QString("select l.CloseDate from dbo.ONLINE$TLN_ALLOW_TALONS l "
                             "where l.talon_id = %1")
                .arg(talonID);
    //    qDebug() << "CloseDate" << strCloseData;
        if (!qc.exec(strMpos)) {
            qDebug() << "Не удалось выполнить запрос" << qc.lastError().text();
        } else {
            if(qc.next()) {
                message = "Талон по данным МПос отоварен на АЗС "+qc.value(0).toString() + " " +
                        qc.value(1).toDateTime().toString("dd MMMM yyyy hh:mm");
                ui->labelMposInfo->setText(message);
                mposOff=true;

            }
            else {
                ui->labelMposInfo->setText("Талон по данным МПос на АЗС не погашался");

            }
        }

        if(!q.exec(strCloseData)) {
                qDebug() << "Не удалось выполнить запрос" << q.lastError().text();
        }
        else {
           if(q.next())
               ui->labelCloseData->setText("Конечная дата реализации: "+q.value(0).toDateTime().toString("dd MMMM yyyy hh:mm"));
           else
               ui->labelCloseData->setText("Талон не имеет конечного срока реализации.");
        }

        strSql = QString("select l.dat, l.TERMINAL_ID, t.name, l.AMOUNT_MOVE from dbo.ONLINE$TLN_LOGS l "
                             "LEFT JOIN dbo.ONLINE$TLN_MOVE_TYPE t on t.TLN_MOVE_TYPE_ID = l.AMOUNT_MOVE "
                             "where l.talon_id = %1 "
                         "ORDER BY l.dat")
                        .arg(talonID);
        modTalon->setQuery(strSql,dbto);
        if(modTalon->rowCount()==0) {
            ui->labelTalonInfo->setText("Для данного талона записей в талонной базе обнаружено. Проверьте номер талона.");
            return;
        }
        int status = modTalon->data(modTalon->index(modTalon->rowCount()-1,3),Qt::DisplayRole).toInt();
        if (status==5) {
            dbaseOff=true;
            message = "Талон активен в базе данных!";
        }

        modTalon->setHeaderData(0,Qt::Horizontal,"Дата");
        modTalon->setHeaderData(1,Qt::Horizontal,"Терминал");
        modTalon->setHeaderData(2,Qt::Horizontal,"Тип операции");
        ui->tableViewInfo->setModel(modTalon);
        ui->tableViewInfo->verticalHeader()->hide();
        ui->tableViewInfo->hideColumn(3);
        ui->tableViewInfo->resizeColumnsToContents();
    //    ui->tableViewInfo->horizontalHeader()->setStretchLastSection(true);
        ui->tableViewInfo->verticalHeader()->setDefaultSectionSize(ui->tableViewInfo->verticalHeader()->minimumSectionSize());
        if(mposOff || dbaseOff) {
            ui->pushButton->setEnabled(false);
            ui->pushButton->setToolTip("Активация талона "+ numberTalon +" не возможна!\n"+message);
        }
}

void MainWindow::on_pushButton_clicked()
{

    QString strProc;
    QSqlDatabase dbto = QSqlDatabase::database("talonsonline");
    QSqlQuery q = QSqlQuery(dbto);
    strProc = QString("{ CALL dbo.ONLINE$p_AddTalonsToWhiteList ( '%1', '%2', '%3', '0', '5', '' ) }")
            .arg(numberTalon)
            .arg(numberTalon)
            .arg(clientNumber);
    qDebug() << "Proc" << endl << strProc;
    q.setForwardOnly(true);
    if(!q.exec(strProc)) {
        qDebug() << "Не удалось активировать талон!" << endl << q.lastError().text();
        return;
    }


    QMessageBox::information(0, qApp->tr("Активация талона"),
                          QString("Активация талона "+numberTalon+" успешно произведена!"),
                          QMessageBox::Ok);
}
