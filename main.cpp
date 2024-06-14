#include "stoxwindow.h"

#include <QApplication>
#include <QMessageBox>
#include <QtSql/QSqlDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

    db.setHostName("127.0.0.1");
    db.setUserName("projectsql");
    db.setPassword("qwe123!@#");

    bool connection = db.open();

    QMessageBox msgBox;

    if(connection) {
        msgBox.setText("Conexão bem sucedida!");
        msgBox.show();
    } else {
        msgBox.setText("Falha na conexão.");
        msgBox.show();
    }

    StoxWindow w;
    w.show();
    return a.exec();
}
