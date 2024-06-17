#include "stoxwindow.h"
#include "wosell.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    StoxWindow mainwindow;

    mainwindow.show();

    return a.exec();
}
