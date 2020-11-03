#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QInputDialog>
#include <QFontDialog>
#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <QScreen>
#include "mainscene.h"
#include "mainview.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // 画笔风格名称集合，排序与 Qt::PenStyle 对应
    const QString pen_styles[6]= {"NoPen","SolidLine","DashLine",
                                  "DotLine","DashDotLine","DashDotDotLine"};
    // 画刷风格名称集合，排序与 Qt::BrushStyle 对应
    const QString brush_styles[15] = {"NoBrush", "SolidPattern", "Dense1Pattern",
                                      "Dense2Pattern", "Dense3Pattern", "Dense4Pattern",
                                      "Dense5Pattern", "Dense6Pattern", "Dense7Pattern",
                                      "HorPattern", "VerPattern", "CrossPattern",
                                      "BDiagPattern", "FDiagPattern", "DiagCrossPattern"};

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void on_actionAbout_Me_triggered();     // 显示“关于我”信息

    void on_actionAbout_QT_triggered();     // 显示 Qt 信息

    void on_actionLine_triggered();         // 设置为绘制直线

    void on_actionRectangle_triggered();    // 设置为绘制矩形

    void on_actionEllipse_triggered();      // 设置为绘制椭圆

    void on_actionPolygon_triggered();      // 设置为绘制多边形

    void on_actionFree_triggered();         // 设置为绘制自由曲线

    void on_actionText_triggered();         // 设置为绘制文字

    void on_actionEraser_triggered();       // 设置为橡皮擦模式

    void on_actionCancel_triggered();       // 回撤上一个操作

    void on_actionClear_triggered();        // 清空画板

    void on_actionFont_triggered();         // 设置文字绘制的字体

    void on_actionLineWidth_triggered();    // 设置画笔的线宽

    void on_actionLineColor_triggered();    // 设置画笔颜色

    void on_actionNoPen_triggered();        // 设置画笔线型为无线条

    void on_actionSolidLine_triggered();    // 设置画笔线型为实线

    void on_actionDashLine_triggered();     // 设置画笔线型为 DashLine

    void on_actionDotLine_triggered();      // 设置画笔线型为 DotLine

    void on_actionDashDotLine_triggered();  // 设置画笔线型为 DashDotLine

    void on_actionDashDotDotLine_triggered();// 设置画笔线型为 DashDotDotLine

    void on_actionBrushColor_triggered(); // 设置填充颜色

    void on_actionNoBrush_triggered();      // 设置填充类型为无填充

    void on_actionSolidPattern_triggered(); // 设置填充类型为实填充

    void on_actionDense1_triggered();       // 设置填充类型为 Dense1

    void on_actionDense2_triggered();       // 设置填充类型为 Dense2

    void on_actionDense3_triggered();       // 设置填充类型为 Dense3

    void on_actionDense4_triggered();       // 设置填充类型为 Dense4

    void on_actionDense5_triggered();       // 设置填充类型为 Dense5

    void on_actionDense6_triggered();       // 设置填充类型为 Dense6

    void on_actionDense7_triggered();       // 设置填充类型为 Dense7

    void on_actionHorizontal_triggered();   // 设置填充类型为 Horizontal

    void on_actionVertical_triggered();     // 设置填充类型为 Vertical

    void on_actionCross_triggered();        // 设置填充类型为 Crossing

    void on_actionForward_triggered();      // 设置填充类型为 Forward Diagnoal

    void on_actionBackward_triggered();     // 设置填充类型为 Backward Diagnoal

    void on_actionDiagCross_triggered();    // 设置填充类型为 Crossing Diagnoal

    void on_actionSave_As_triggered();      // 将画板另存为图片

private:
    void init_action_groups();              // 初始化动作按钮分组
    void init_status_bar();                 // 初始化底部状态栏
    void update_status_text();              // 更新状态栏文字信息

private:
    Ui::MainWindow *ui;

    // 采用 Graphics View Framework 作为绘图架构
    MainScene *scene;   // QGraphicsScene 部分（场景）
    MainView *view;     // QGraphicsView 部分（视图）
    QPen *pen;          // 主画笔
    QBrush *brush;      // 主画刷（画刷用于填充图形内部区域）

    // 存储窗口下方状态栏的相关文字信息
    QLabel *status_text_pen_color;
    QLabel *status_text_pen_style;
    QLabel *status_text_pen_width;
    QLabel *status_text_brush_color;
    QLabel *status_text_brush_style;
};
#endif // MAINWINDOW_H
