#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include <QStandardItemModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionInputNumber_triggered();
private:
    void crModelSerials();
    void crModelTalons();
    long long getTalonID(long long ser, long long num, int clientID);
private:
    Ui::MainWindow *ui;
    QStringList listTalons;
    QSqlQueryModel *modelSerials;
    QStandardItemModel *modelTalons;

};

#endif // MAINWINDOW_H
