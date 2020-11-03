#include <QDebug>
#include "mainview.h"

MainView::MainView(MainScene *scene)
{
    this->scene = scene;
    setScene(scene);
}

void MainView::mousePressEvent(QMouseEvent *event)
{
    scene->mousePressd(event);
    qDebug()<<"in MainView::mousePressEvent: mouse pressed";
}

void MainView::mouseMoveEvent(QMouseEvent *event)
{
    scene->mouseMoved(event);
    qDebug()<<"in MainView::mouseMoveEvent: mouse moved";
}

void MainView::mouseReleaseEvent(QMouseEvent *event)
{
    scene->mouseReleased(event);
    qDebug()<<"in MainView::mouseReleaseEvent: mouse released";
}
