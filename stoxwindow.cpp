#include "stoxwindow.h"
#include "ui_stoxwindow.h"

StoxWindow::StoxWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::StoxWindow) {
    ui->setupUi(this);

}

StoxWindow::~StoxWindow() {
    delete ui;
}
