#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>
#include <QMessageBox>
#include <QString>

#define DATABASE_HOSTNAME   "10.28.8.78"
#define DATABASE_NAME       "cso"
#define DATABASE_USER       "csouser"
#define DATABASE_PASS       "SuP0rT"

static bool createConnection()
{
    QSqlDatabase dbto = QSqlDatabase::addDatabase("QODBC3","talonsonline");
//    db.setDatabaseName("DRIVER={ODBC Driver 11 for SQL Server};SERVER =192.168.123.12\\SQLEXPRESS;DATABASE=talonsonline;Trusted_Connection=no;");
    dbto.setDatabaseName("Driver={SQL SERVER};SERVER=192.168.123.12;DATABASE=talonsonline;Trusted_Connection=no;");
    dbto.setUserName("user1c");
    dbto.setPassword("**********");
    if(!dbto.open()) {
        qDebug() <<  "Не возможно подключиться к базе данных." << endl << "Причина:" << dbto.lastError().text();
        QMessageBox::critical(0, qApp->tr("Не могу открыть базу данных"),
                              QString("Не могу установить соединение с базой данных TalonsOnline\nПричина: %1").arg(dbto.lastError().text()),
                              QMessageBox::Cancel);
        return false;
    }

    QSqlDatabase dbfb = QSqlDatabase::addDatabase("QIBASE","central");
        dbfb.setHostName("192.168.123.11");
        dbfb.setDatabaseName("sunoil");
        dbfb.setUserName("SYSDBA");
        dbfb.setPassword("sunoil00");

        if(!dbfb.open()) {
            qDebug() <<  "Не возможно подключиться к базе данных." << endl << "Причина:" << dbfb.lastError().text();
            QMessageBox::critical(0, qApp->tr("Не могу открыть базу данных"),
                                  QString("Не могу установить соединение c центральной базой данных MPos\nПричина: %1\n Проверьте настройки подключения.").arg(dbfb.lastError().text()),
                                  QMessageBox::Cancel);
            return false;
        }
    return true;

}

#endif // DATABASE_H

