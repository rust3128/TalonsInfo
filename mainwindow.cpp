#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "inputtalonsdialog.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_actionInputNumber_triggered()
{
    InputTalonsDialog *inTlnDlg = new InputTalonsDialog();
    inTlnDlg->exec();
    listTalons=inTlnDlg->talonsNumber;
    qDebug() << listTalons;
}
