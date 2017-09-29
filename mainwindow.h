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
    void on_tableViewTalons_doubleClicked(const QModelIndex &idx);

    void on_pushButton_clicked();

private:
    void crModelSerials();
    void crModelTalons();
    long long getTalonID(long long ser, long long num, int clientID);
private:
    Ui::MainWindow *ui;
    QStringList listTalons;
    QSqlQueryModel *modelSerials;
    QSqlQueryModel *modTalon;
    QStandardItemModel *modelTalons;
    int clientNumber;
    QString numberTalon;

};

#endif // MAINWINDOW_H
