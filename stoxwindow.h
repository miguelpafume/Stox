#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
    class StoxWindow;
}
QT_END_NAMESPACE

class StoxWindow : public QMainWindow {
    Q_OBJECT

    public:
        StoxWindow(QWidget *parent = nullptr);
        ~StoxWindow();

    private:
        Ui::StoxWindow *ui;
};
