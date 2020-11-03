#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 窗口、视区设置
    scene = new MainScene();
    view = new MainView(scene);
    setCentralWidget(view);         // 将 view 设置为窗口的中心控件，中心控件是一个布局，即 view 会随着窗口大小变化自动改变尺寸
    view->setMinimumSize(630, 420); // 最小尺寸
    view->setAlignment(Qt::AlignLeft | Qt::AlignTop);   // scene 超出 view 的视野时展示区域对齐到 scene 的左上方区域
    view->setRenderHint(QPainter::Antialiasing);
    this->resize(800, 600);         // 初始化窗口大小，注意：整个窗口除了中心区域还包含了 menubar, toolbar 和 statusbar 等区域

    // 初始化画笔、画刷
    pen = new QPen();
    scene->setPen(pen);
    brush = new QBrush(*(new QColor("black")), Qt::NoBrush);
    scene->setBrush(brush);

    init_action_groups();
    init_status_bar();

    // 随便加点初始文字
    QGraphicsSimpleTextItem* textItem = new QGraphicsSimpleTextItem();
    textItem->setText(QString("一个迷你画图小程序😉"));
    textItem->setFont(QFont("Times", 12, QFont::Normal));
    textItem->setPos(QPoint(20, 20));
    textItem->setPen(*pen);
    textItem->setBrush(*(new QBrush(*(new QColor("black")), Qt::SolidPattern)));
    scene->addItem(textItem);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);                                                // 声明未使用变量，避免编译器警告
    scene->setSceneRect(0, 0, view->width()-2, view->height()-8);   // 若 scene 与 view 大小接近会触发 scrollbar，经尝试减去这两个数刚好能消去
    scene->setRect(QRect(0, 0, view->width(), view->height()));     // 更新需要刷新绘图的矩形区域
}

void MainWindow::init_action_groups()
{
    // 分组可保证一个分组内在同一时间只有一个按钮可被按下
    // 所有的画图模式合并为一组，不设默认模式
    QActionGroup *typeGroup = new QActionGroup(this);
    typeGroup->addAction(ui->actionLine);
    typeGroup->addAction(ui->actionRectangle);
    typeGroup->addAction(ui->actionEllipse);
    typeGroup->addAction(ui->actionPolygon);
    typeGroup->addAction(ui->actionFree);
    typeGroup->addAction(ui->actionText);
    typeGroup->addAction(ui->actionEraser);

    // 所有画笔线型合并为一组，并设实线为默认线型
    QActionGroup *lineStyleGroup = new QActionGroup(this);
    lineStyleGroup->addAction(ui->actionSolidLine);
    lineStyleGroup->addAction(ui->actionDashLine);
    lineStyleGroup->addAction(ui->actionDotLine);
    lineStyleGroup->addAction(ui->actionDashDotLine);
    lineStyleGroup->addAction(ui->actionDashDotDotLine);
    lineStyleGroup->addAction(ui->actionNoPen);
    ui->actionSolidLine->setChecked(true);

    // 所有填充类型合并为一组，并设无填充为默认类型
    QActionGroup *brushStyleGroup = new QActionGroup(this);
    brushStyleGroup->addAction(ui->actionNoBrush);
    brushStyleGroup->addAction(ui->actionSolidPattern);
    brushStyleGroup->addAction(ui->actionDense1);
    brushStyleGroup->addAction(ui->actionDense2);
    brushStyleGroup->addAction(ui->actionDense3);
    brushStyleGroup->addAction(ui->actionDense4);
    brushStyleGroup->addAction(ui->actionDense5);
    brushStyleGroup->addAction(ui->actionDense6);
    brushStyleGroup->addAction(ui->actionDense7);
    brushStyleGroup->addAction(ui->actionHorizontal);
    brushStyleGroup->addAction(ui->actionVertical);
    brushStyleGroup->addAction(ui->actionCross);
    brushStyleGroup->addAction(ui->actionForward);
    brushStyleGroup->addAction(ui->actionBackward);
    brushStyleGroup->addAction(ui->actionDiagCross);
    ui->actionNoBrush->setChecked(true);
}

void MainWindow::init_status_bar()
{
    // 主窗口下方状态栏文字设置及初始化
    status_text_pen_color = new QLabel((QString)"Line Color: " + pen->color().name());
    status_text_pen_color->setMinimumWidth(150);
    ui->statusbar->addPermanentWidget(status_text_pen_color);

    status_text_pen_style = new QLabel((QString)"Style: " +
                                       pen_styles[QString::number(pen->style()).toInt()]);
    status_text_pen_style->setMinimumWidth(170);
    ui->statusbar->addPermanentWidget(status_text_pen_style);

    status_text_pen_width = new QLabel((QString)"Width: " +
                                       QString::number(pen->widthF()) + "px");
    status_text_pen_width->setMinimumWidth(120);
    ui->statusbar->addPermanentWidget(status_text_pen_width);

    status_text_brush_color = new QLabel((QString)"Brush Color: " + brush->color().name());
    status_text_brush_color->setMinimumWidth(150);
    ui->statusbar->addPermanentWidget(status_text_brush_color);

    status_text_brush_style = new QLabel((QString)"Style: " +
                                         brush_styles[QString::number(brush->style()).toInt()]);
    status_text_brush_style->setMinimumWidth(170);
    ui->statusbar->addPermanentWidget(status_text_brush_style);
}

void MainWindow::update_status_text()
{
    // 更新状态栏字符串，与各字符串关联的状态栏信息会自动更新内容
    status_text_pen_color->setText((QString)"Line Color: " + pen->color().name());
    status_text_pen_style->setText((QString)"Style: " +
                                    pen_styles[QString::number(pen->style()).toInt()]);
    status_text_pen_width->setText((QString)"Width: " +
                                    QString::number(pen->widthF()) + "px");
    status_text_brush_color->setText((QString)"Brush Color: " + brush->color().name());
    status_text_brush_style->setText((QString)"Style: " +
                                     brush_styles[QString::number(brush->style()).toInt()]);
}

void MainWindow::on_actionAbout_Me_triggered()
{
    QMessageBox::about(this, "About Me", QString("<h2>武鑫</h2>")+
                                         "<p>学号:&nbsp;&nbsp;11925063<p>" +
                                         "<p>邮箱:&nbsp;&nbsp;wuhsin@zju.edu.cn" +
                                         "<p>GitHub:&nbsp;&nbsp;" +
                                         "<a href=\"https://github.com/XinArkh\">" +
                                         "https://github.com/XinArkh</a>"
                       );
}

void MainWindow::on_actionAbout_QT_triggered()
{
    QMessageBox::aboutQt(this, "About QT");
}

void MainWindow::on_actionLine_triggered()
{
    // 设置当前画图模式
    scene->setType(QString("Line"));
    // 更新状态栏
    update_status_text();
}

void MainWindow::on_actionRectangle_triggered()
{
    scene->setType(QString("Rectangle"));
    update_status_text();
}

void MainWindow::on_actionEllipse_triggered()
{
    scene->setType(QString("Ellipse"));
    update_status_text();
}

void MainWindow::on_actionPolygon_triggered()
{
    scene->setType(QString("Polygon"));
    update_status_text();
}

void MainWindow::on_actionFree_triggered()
{
    scene->setType(QString("Free"));
    update_status_text();
}

void MainWindow::on_actionText_triggered()
{
    scene->setType(QString("Text"));
    QString input_text = QInputDialog::getText(this,
                                               "Input Drawing Text",
                                               "Enter Text: ");
    scene->setText(input_text);
    update_status_text();
}

void MainWindow::on_actionEraser_triggered()
{
    scene->setType(QString("Eraser"));
    update_status_text();
}

void MainWindow::on_actionCancel_triggered()
{
    scene->deleteOne();
}

void MainWindow::on_actionClear_triggered()
{
    scene->deleteAll();
}

void MainWindow::on_actionFont_triggered()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, scene->getFont(), this);
    if (ok) {
        scene->setFont(font);
    }
}

void MainWindow::on_actionLineWidth_triggered()
{
    bool ok;
    QString width_text = QInputDialog::getText(this,
                                               "Set Pen Width",
                                               "Enter Width (pixel): ",
                                               QLineEdit::Normal,
                                               QString::number(pen->widthF()),
                                               &ok);
    if(ok && !width_text.isEmpty())
    {
        pen->setWidthF(width_text.toDouble());
    }
    update_status_text();
}

void MainWindow::on_actionLineColor_triggered()
{
    QColor line_color = QColorDialog::getColor(pen->color(), this);
    if(line_color.isValid())
    {
        pen->setColor(line_color);
        update_status_text();
    }
}

void MainWindow::on_actionNoPen_triggered()
{
    pen->setStyle(Qt::NoPen);
    update_status_text();
}

void MainWindow::on_actionSolidLine_triggered()
{
    pen->setStyle(Qt::SolidLine);
    update_status_text();
}

void MainWindow::on_actionDashLine_triggered()
{
    pen->setStyle(Qt::DashLine);
    update_status_text();
}

void MainWindow::on_actionDotLine_triggered()
{
    pen->setStyle(Qt::DotLine);
    update_status_text();
}

void MainWindow::on_actionDashDotLine_triggered()
{
    pen->setStyle(Qt::DashDotLine);
    update_status_text();
}

void MainWindow::on_actionDashDotDotLine_triggered()
{
    pen->setStyle(Qt::DashDotDotLine);
    update_status_text();
}

void MainWindow::on_actionNoBrush_triggered()
{
    brush->setStyle(Qt::NoBrush);
    update_status_text();
}

void MainWindow::on_actionSolidPattern_triggered()
{
    brush->setStyle(Qt::SolidPattern);
    update_status_text();
}

void MainWindow::on_actionDense1_triggered()
{
    brush->setStyle(Qt::Dense1Pattern);
    update_status_text();
}

void MainWindow::on_actionDense2_triggered()
{
    brush->setStyle(Qt::Dense2Pattern);
    update_status_text();
}

void MainWindow::on_actionDense3_triggered()
{
    brush->setStyle(Qt::Dense3Pattern);
    update_status_text();
}

void MainWindow::on_actionDense4_triggered()
{
    brush->setStyle(Qt::Dense4Pattern);
    update_status_text();
}

void MainWindow::on_actionDense5_triggered()
{
    brush->setStyle(Qt::Dense5Pattern);
    update_status_text();
}

void MainWindow::on_actionDense6_triggered()
{
    brush->setStyle(Qt::Dense6Pattern);
    update_status_text();
}

void MainWindow::on_actionDense7_triggered()
{
    brush->setStyle(Qt::Dense7Pattern);
    update_status_text();
}

void MainWindow::on_actionHorizontal_triggered()
{
    brush->setStyle(Qt::HorPattern);
    update_status_text();
}

void MainWindow::on_actionVertical_triggered()
{
    brush->setStyle(Qt::VerPattern);
    update_status_text();
}

void MainWindow::on_actionCross_triggered()
{
    brush->setStyle(Qt::CrossPattern);
    update_status_text();
}

void MainWindow::on_actionForward_triggered()
{
    brush->setStyle(Qt::FDiagPattern);
    update_status_text();
}

void MainWindow::on_actionBackward_triggered()
{
    brush->setStyle(Qt::BDiagPattern);
    update_status_text();
}

void MainWindow::on_actionDiagCross_triggered()
{
    brush->setStyle(Qt::DiagCrossPattern);
    update_status_text();
}

void MainWindow::on_actionBrushColor_triggered()
{
    QColor brush_color = QColorDialog::getColor(brush->color(), this);
    if(brush_color.isValid())
    {
        brush->setColor(brush_color);
        update_status_text();
    }
}

void MainWindow::on_actionSave_As_triggered()
{
    QString file_path = QFileDialog::getSaveFileName(this,
                                                     "Save As",
                                                     "untitled.png",
                                                     "Images (*.png *.bmp *.jpg)");
    QScreen *screen = QGuiApplication::primaryScreen();
    screen->grabWindow(view->winId()).save(file_path);
}
