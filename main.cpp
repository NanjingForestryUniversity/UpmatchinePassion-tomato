#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    try {
        Widget w;
        w.show();
        return a.exec();
    } catch (const ExpiredException& e) {
        QMessageBox::critical(nullptr, "程序过期", e.what());
        return -1;
    }
}
