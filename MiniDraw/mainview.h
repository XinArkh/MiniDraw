#ifndef MAINVIEW_H
#define MAINVIEW_H
#include <QGraphicsView>
#include "mainscene.h"


class MainView : public QGraphicsView
{
public:
    MainView(MainScene *scene);

protected:
    // 鼠标事件在主程序循环中会自动触发，通过重写以下三个函数实现事件的捕获和响应
    void mousePressEvent(QMouseEvent *event);   // 鼠标按下事件
    void mouseMoveEvent(QMouseEvent *event);    // 鼠标移动事件（默认只有在按下之后才触发）
    void mouseReleaseEvent(QMouseEvent *event); // 鼠标释放事件

private:
    MainScene *scene;
};

#endif // MAINVIEW_H
