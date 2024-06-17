#include "CustomFunc.h"

void SimpleMessageBox(QString &&str, const short unsigned int i) {
    static QMessageBox msg_box;
    msg_box.setWindowIcon(QIcon(":imgs/stox_24x24.png"));
    if (i == 0) { msg_box.setIcon(QMessageBox::Information); }
    else if (i == 1) { msg_box.setIcon(QMessageBox::Warning); }
    msg_box.setText(str);
    msg_box.show();
}
