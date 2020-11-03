#include <QDebug>
#include "mainscene.h"

MainScene::MainScene()
{
    // 初始化橡皮擦画笔
    pen_eraser = new QPen(*(new QColor("white")));
    pen_eraser->setWidth(10);
    // 初始化文字画刷
    brush_text = new QBrush(*(new QColor("black")), Qt::SolidPattern);
    // 初始化文字绘制字体
    font = QFont("Times", 18, QFont::Normal);
}

void MainScene::setType(QString t)
{
    draw_type = t;
    qDebug()<<"in MainScene::setType: set type"<<t;
}

QString MainScene::getType()
{
    return draw_type;
}

void MainScene::setRect(QRectF rect)
{
    draw_rect = rect;
}

QRectF MainScene::getRect()
{
    return draw_rect;
}

void MainScene::setPen(QPen *p)
{
    pen = p;
}

QPen *MainScene::getPen()
{
    return pen;
}

void MainScene::setBrush(QBrush *b)
{
    brush = b;
}

QBrush *MainScene::getBrush()
{
    return brush;
}

void MainScene::setFont(QFont f)
{
    font = f;
}

QFont MainScene::getFont()
{
    return font;
}

void MainScene::setText(QString t)
{
    text = t;
}

void MainScene::mousePressd(QMouseEvent *event)
{
    // 记录鼠标按下时的位置
    // 在鼠标释放之前 start_point 将不再改变，而 end_point 的值会随鼠标移动而改变
    start_point = end_point = event->pos();
    // 根据字符串的值确定不同的绘制类型
    if(draw_type == "Line")             // 直线
    {
        // 新建一个直线类型并存入 items vector
        QGraphicsLineItem *lineItem = new QGraphicsLineItem();
        lineItem->setLine(start_point.x(), start_point.y(),
                          end_point.x(), end_point.y());
        lineItem->setPen(*pen);
        // 将当前 item 添加到 items vector 中，即由 items 索引场景中存在的所有元素
        items.push_back(lineItem);
        // 通过 QGrsphicsScene 的 addItem() 函数将当前 item 添加到场景中，从而进一步显示
        addItem(lineItem);
    }
    else if(draw_type == "Rectangle")   // 矩形
    {
        QGraphicsRectItem *rectItem = new QGraphicsRectItem();
        rectItem->setRect(*(new QRect(start_point, end_point)));
        rectItem->setPen(*pen);
        rectItem->setBrush(*brush);
        items.push_back(rectItem);
        addItem(rectItem);
    }
    else if(draw_type == "Ellipse")     // 椭圆
    {
        QGraphicsEllipseItem *ellipseItem = new QGraphicsEllipseItem();
        ellipseItem->setRect(*(new QRect(start_point, end_point)));
        ellipseItem->setPen(*pen);
        ellipseItem->setBrush(*brush);
        items.push_back(ellipseItem);
        addItem(ellipseItem);
    }
    else if(draw_type == "Polygon")     // 多边形
    {
        // 多边形在绘制过程中需要多次点击鼠标左键确定关键点，最后点击鼠标右键确认所有关键点
        if(event->button() == Qt::LeftButton)   // 点击鼠标左键，添加关键点
        {
            // 关键点集非空，说明之前已有添加的点，除添加关键点外还需添加目前已有的边线
            // 边线由线条元素临时画出，在关键点选取完毕（即点击鼠标右键）后全部删除
            if(!polygonPoints.isEmpty())
            {
                QPoint p1 = polygonPoints.last();
                QPoint p2 = start_point;
                QGraphicsLineItem *lineItem = new QGraphicsLineItem();
                lineItem->setLine(p1.x(), p1.y(), p2.x(), p2.y());
                polygonLines.push_back(lineItem);   // 将所有临时线段添加到一个 vector 中管理
                polygonPoints.push_back(start_point);   // 在关键点集中添加关键点
                addItem(lineItem);
                // update() 是 QGraphicsScene 的一个槽函数，用于刷新指定矩形框内的画面
                update(draw_rect);
            }
            // 关键点集为空集，说明现在是第一个关键点，只需要添加点即可，不需要画线
            else
            {
                polygonPoints.push_back(start_point);
            }
        }
        else if(event->button() == Qt::RightButton) // 点击鼠标右键，多边形绘制完毕
        {
            if(!polygonPoints.isEmpty())
            {
                // 删除所有临时绘制的边线
                while(!polygonLines.isEmpty())
                {
                    removeItem(polygonLines.last());
                    polygonLines.pop_back();
                }
                // 用现有的关键点新建一个多边形类型的元素
                QGraphicsPolygonItem *polygonItem = new QGraphicsPolygonItem();
                polygonItem->setPolygon(*(new QPolygonF(polygonPoints)));
                polygonItem->setPen(*pen);
                polygonItem->setBrush(*brush);
                items.push_back(polygonItem);
                addItem(polygonItem);
                // 根据官方文档，清空 QVector 且释放空间的推荐写法如下
                QVector<QPoint>().swap(polygonPoints);
            }
        }
    }
    else if(draw_type == "Free" || draw_type == "Eraser")   // 自由曲线或橡皮擦
    {
        // 在鼠标按下并移动的过程中，用大量线段拼接来实现自由曲线
        // 橡皮擦本质上即为一个白色的、具有一定宽度的自由曲线，用其覆盖要擦除的内容
        freePointLast = start_point;    // 记录自由曲线的最后一个端点，该端点会不断更新
        // 新建一个元素分组，将所有组成自由曲线的线段添加到该分组内管理
        // 用 createItemGroup() 创建分组后分组元素本身会自动添加到 scene，不需要再单独添加
        freeGroup = createItemGroup(*(new QList<QGraphicsItem*>));
        items.push_back(freeGroup);     // 分组整体作为一个元素添加到 items
        // 生成第一条线段（点）并添加到分组
        QGraphicsLineItem* lineItem = new QGraphicsLineItem();
        lineItem->setLine(freePointLast.x(), freePointLast.y(),
                          end_point.x(), end_point.y());
        if(draw_type == "Free")
        {
            lineItem->setPen(*pen);
        }
        else
        {
            lineItem->setPen(*pen_eraser);
        }
        freeGroup->addToGroup(lineItem);
    }
    else if(draw_type == "Text")        // 文字
    {
        QGraphicsSimpleTextItem* textItem = new QGraphicsSimpleTextItem();
        if(!text.isEmpty())
        {
            textItem->setText(text);
            textItem->setFont(font);
            textItem->setPos(end_point);
            textItem->setPen(*pen);
            brush_text->setColor(pen->color()); // 每次绘制都保持和线条颜色一致
            textItem->setBrush(*brush_text);
            items.push_back(textItem);
            addItem(textItem);
        }
    }
}

void MainScene::mouseMoved(QMouseEvent *event)  // 鼠标移动时不断刷新元素状态
{
    end_point = event->pos();               // 记录鼠标移动到的位置
    if(draw_type == "Line")
    {
        // 强制类型转换，取出 items 中的最后一个元素，即为当前操作的元素，然后更新该元素
        QGraphicsLineItem* lineItem = static_cast<QGraphicsLineItem*>(items.last());
        lineItem->setLine(start_point.x(), start_point.y(),
                          end_point.x(), end_point.y());
    }
    else if(draw_type == "Rectangle")
    {
        QGraphicsRectItem* rectItem = static_cast<QGraphicsRectItem*>(items.last());
        rectItem->setRect(*(new QRect(start_point, end_point)));
    }
    else if(draw_type == "Ellipse")
    {
        QGraphicsEllipseItem* ellipseItem = static_cast<QGraphicsEllipseItem*>(items.last());
        ellipseItem->setRect(*(new QRect(start_point, end_point)));
    }
    else if(draw_type == "Polygon")
    {
    }
    else if(draw_type == "Free" || draw_type == "Eraser")
    {
        // 每一次检测到鼠标移动信号，都马上画一条从最后端点到鼠标当前端点的线段
        QGraphicsLineItem* lineItem = new QGraphicsLineItem();
        lineItem->setLine(freePointLast.x(), freePointLast.y(),
                          end_point.x(), end_point.y());
        if(draw_type == "Free")
        {
            lineItem->setPen(*pen);
        }
        else
        {
            lineItem->setPen(*pen_eraser);
        }
        freeGroup->addToGroup(lineItem);    // 将该线段添加到自由曲线分组中
        freePointLast = end_point;          // 更新自由曲线的最后一个端点
    }
    else if(draw_type == "Text")
    {
    }
    update(draw_rect);                      // 利用 update() 槽函数刷新画面显示
}

void MainScene::mouseReleased(QMouseEvent *event)   // 鼠标释放时最终确定元素参数，结束本次绘制
{
    end_point = event->pos();
    if(draw_type == "Line")
    {
        QGraphicsLineItem* lineItem = static_cast<QGraphicsLineItem*>(items.last());
        lineItem->setLine(start_point.x(), start_point.y(),
                          end_point.x(), end_point.y());
    }
    else if(draw_type == "Rectangle")
    {
        QGraphicsRectItem* rectItem = static_cast<QGraphicsRectItem*>(items.last());
        rectItem->setRect(*(new QRect(start_point, end_point)));
    }
    else if(draw_type == "Ellipse")
    {
        QGraphicsEllipseItem* ellipseItem = static_cast<QGraphicsEllipseItem*>(items.last());
        ellipseItem->setRect(*(new QRect(start_point, end_point)));
    }
    else if(draw_type == "Polygon")
    {
    }
    else if(draw_type == "Free")
    {
    }
    else if(draw_type == "Text")
    {
    }
    else if(draw_type == "Eraser")
    {
    }
    update(draw_rect);
}

void MainScene::deleteOne()
{
    if(!items.isEmpty())
    {
        removeItem(items.last());
        items.pop_back();
        update(draw_rect);
    }
}

void MainScene::deleteAll()
{
    while(!items.isEmpty())
    {
        removeItem(items.last());
        items.pop_back();
    }
    update(draw_rect);
}
