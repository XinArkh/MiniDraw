#ifndef MAINSCENE_H
#define MAINSCENE_H
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QMouseEvent>


class MainScene : public QGraphicsScene
{
public:
    MainScene();

    void setType(QString t);    // 设置绘制类型
    QString getType();          // 获取绘制类型
    void setRect(QRectF rect);  // 设置刷新区域
    QRectF getRect();           // 获取刷新区域
    void setPen(QPen *p);       // 设置画笔
    QPen *getPen();             // 获取画笔
    void setBrush(QBrush *b);   // 设置画刷
    QBrush *getBrush();         // 获取画刷
    void setFont(QFont f);      // 设置字体
    QFont getFont();            // 获取字体
    void setText(QString t);    // 设置绘制文字
    void deleteOne();           // 回撤上一个操作
    void deleteAll();           // 清空画板
    void mousePressd(QMouseEvent *event);   // 响应鼠标按下事件
    void mouseMoved(QMouseEvent *event);    // 响应鼠标移动事件
    void mouseReleased(QMouseEvent *event); // 响应鼠标释放事件

private:
    QString draw_type = "None";     // 标记绘制类型
    QRectF draw_rect;               // 存储视图刷新区域
    QPen *pen;                      // 主画笔
    QPen *pen_eraser;               // 用于实现橡皮擦的画笔
    QBrush *brush;                  // 主画刷
    QBrush *brush_text;             // 用于文字绘制的画刷
    QFont font;                     // 文字绘制字体
    QString text;                   // 文字绘制内容
    QPoint start_point, end_point;  // 鼠标点击的起始点与终止点
    QVector<QGraphicsItem*> items;  // 存储画图过程中添加的所有元素
    QVector<QPoint> polygonPoints;  // 存储多边形绘制过程中点击鼠标得到的所有点
    QVector<QGraphicsLineItem*> polygonLines;   // 临时存储多边形绘制过程中产生的线段
    QPoint freePointLast;           // 存储绘制自由曲线时的上一个端点
    QGraphicsItemGroup* freeGroup;  // 元素集合，用于存储绘制自由曲线时生成的所有线段
};

#endif // MAINSCENE_H
