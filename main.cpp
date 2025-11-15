#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 设置应用程序信息
    a.setApplicationName("简易记事本");
    a.setApplicationVersion("1.0");
    a.setOrganizationName("flairziv");
    a.setOrganizationDomain("flairziv.com");

    // 设置应用程序样式（可选）
    // a.setStyle("Fusion");

    MainWindow w;
    w.show();

    return a.exec();
}
