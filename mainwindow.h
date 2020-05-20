#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QStack>
#include <QEvent>
#include <QLabel>
namespace Ui {
class MainWindow;
#define DEFAULT -1//画笔
#define DRAWLINE 0
#define DRAWARCH 1//曲线，贝塞尔曲线拟合
#define DRAWPAINT 2//喷桶
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
//    bool fontweight;//字体宽度
    QImage img;//图片img
    const int dx[8]={-1,-1,-1,0,0,1,1,1};
    const int dy[8]={-1,0,1,-1,1,-1,0,1};
    QPoint lastpoint;//上一次的点
    QPoint curpoint;//当前点
    int datawidth;
    int dataheight;
   // QImage simg;//抗锯齿图片
    QGraphicsScene *scene;//绘制场景
    QRgb color;//当前颜色，初始化为黑色
    bool mouse_press;
    QStack<QImage>preimg;//上一次图片，用于撤回
    int archstate;//用于记录贝赛尔曲线设置的状态
     int model;//绘制模式
QImage imgtmp;//图片暂存
int frac;//用来记录贝塞尔函数的阶数
 QVector<QPoint>ctrpoint;//贝塞尔曲线控制点
    int maxlevel;//贝赛尔曲线递归层数
   // bool curvedone;//曲线是否画完
QStack<QPoint>unpaint;//区域填充（种子填充算法）所用的栈
//int realwidth;//实际数据对应的宽度
//int depth;//图片位宽
// QStack<QImage>reimg;//用于重做
 int lens;//图片总长
 unsigned char *data;//数组img，一个动态空间
void init();//初始化，和新建一个效果
    void getrealwidth();
        void uiset();//初始化按钮是否可用
protected:
    void drawbezier(QPoint ,QPoint ,QPoint ,QPoint );//绘制3阶贝赛尔曲线
    void drawhighbezier(int cnt,QVector<QPoint>ps,int level);//绘制高阶贝赛尔曲线 cnt多少个点 cnt-1阶贝赛尔曲线
    void fflush();//刷新img
    void drawpix(int x,int y);//在(x,y)处画一个点，颜色为color
    void drawline(QPoint,QPoint);
    //void bigpoint(QPoint &p);//画一个大点
    void showarch();//显示贝赛尔曲线
    void DEFAULT_PRESS(QMouseEvent *event);
    void DEFAULT_MOVE(QMouseEvent *event);
    void DEFAULT_UP(QMouseEvent *event);
    void DRAWLINE_PRESS(QMouseEvent *event);
    void DRAWLINE_MOVE(QMouseEvent *event);
    void DRAWLINE_UP(QMouseEvent *event);
    void DRAWARCH_UP(QMouseEvent *event);
    void DRAWARCH_PRESS(QMouseEvent *event);
    void DRAWARCH_MOVE(QMouseEvent *event);
    void DRAWPAINT_PRESS(QMouseEvent *event);
    bool innerdis_point(QPoint a,QPoint b);
    void colorfill(int x, int y);
private:
    Ui::MainWindow *ui;
public slots:
    bool eventFilter(QObject *obj,QEvent *event);
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_6_clicked();
};

#endif // MAINWINDOW_H
