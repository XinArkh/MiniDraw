#include <QApplication>
#include <QTextCodec>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    // 使用 UTF-8 支持中文字符
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("MiniDraw");   // 设置窗口标题
    w.show();
    return a.exec();
}
