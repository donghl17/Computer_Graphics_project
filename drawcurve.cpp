#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>
#include <QtMath>
#include <QDebug>
#include <QMouseEvent>
/*
 * 画贝赛尔曲线 初始化状态archstate=0
 * archstate=1 画完起始点
 * archstate=2 画完终点
 * archstate=2+i 画完第i个控制点    最终到frac+1
 * archstate=frac+1+i 左键按下第i个控制点并拖动 i=1,2,......
*/

void MainWindow::showarch()
{
    QRgb coltmp=color;
    color=QColor::fromRgb(247,98,175).rgba();//转颜色
    color=coltmp;
    //绘制贝赛尔曲线
    QVector<QPoint>ps;
    ps.push_back(lastpoint);
    for(int i=0;i<frac-1;i++)ps.push_back(ctrpoint[i]);//放入控制点
    ps.push_back(curpoint);
    maxlevel=20;
    drawhighbezier(frac+1,ps,1);//frac阶贝赛尔曲线，frac+1个点
}

bool MainWindow::innerdis_point(QPoint a,QPoint b)//判断两个点之间的距离是否小于某一值
{
    int tmp=abs(a.x()-b.x())+abs(a.y()-b.y());//两点之间的距离
    if(tmp<=20)return true;
    return false;
}
void MainWindow::DRAWARCH_PRESS(QMouseEvent *event)
{
    if(archstate==0)//刚开始画
    {
        ctrpoint.clear();//清除控制点
        lastpoint=event->pos();//起始点
        archstate=1;//准备结束
    }
    else if(archstate>=2&&archstate<frac+1)//结束后archstate=frac+1
    {
        ctrpoint.push_back(event->pos());
        archstate++;
        if(archstate>=frac+1)showarch();

    }
    if(archstate==frac+1)
    {
        archstate=0;
    }

}
void MainWindow::DRAWARCH_MOVE(QMouseEvent *event)//用于拖动控制点
{
//无事发生
}
void MainWindow::DRAWARCH_UP(QMouseEvent *event)
{
    if(archstate==1)//准备结束
    {
        curpoint=event->pos();//终止点
        archstate=2;//准备设置第一个控制点
    }

}
double dot(QLine A,QLine B)
{
    int x1=A.x2()-A.x1();
    int y1=A.y2()-A.y1();
    int x2=B.x2()-B.x1();
    int y2=B.y2()-B.y1();
    return x1*x2+y1*y2;
}
double d(QPoint a,QPoint b,QPoint c)
{
    QLine A(b,a);
    QLine B(b,c);
    QPoint d=b+(c-b)*dot(A,B)/dot(B,B);
    QPoint e=d-a;
    return sqrt(dot(QLine(QPoint(0,0),e),QLine(QPoint(0,0),e)));
}
void MainWindow::drawbezier(QPoint p00,QPoint p10,QPoint p20,QPoint p30)//绘制贝赛尔曲线
{
    if(p00==p30)return ;//递归终止
    //p0~p3[0]的点
    QPoint p01=(p00+p10)/2;
    QPoint p11=(p10+p20)/2;
    QPoint p21=(p20+p30)/2;
    QPoint p02=(p01+p11)/2;
    QPoint p12=(p11+p21)/2;
    QPoint p03=(p02+p12)/2;
    //二分法得到3阶递推关系点
    drawpix(p03.x(),p03.y());//绘制中间点
    drawbezier(p00,p01,p02,p03);//左段递归
    drawbezier(p03,p12,p21,p30);//右段递归
}
void MainWindow::drawhighbezier(int cnt,QVector<QPoint>ps,int level)//level为递归层数
{
    if(level==maxlevel)
    {
        drawline(ps[0],ps[cnt-1]);
        return ;//
    }
    if(ps[0]==ps[cnt-1])return ;//递归终止
    double tmp=0;
    for(int i=1;i<cnt-1;i++)
    {
        tmp=qMax(tmp,d(ps[i],ps[0],ps[cnt-1]));
    }
    if(tmp<=2)
    {
        drawline(ps[0],ps[cnt-1]);
        return ;
        //收敛
    }
    //p0~p3[0]的点
    QVector<QPoint>px;//初始化左段递归
    px.push_back(ps[0]);
    for(int i=0;i<cnt-1;i++)//递归cnt-1次
    {
        for(int j=0;j<cnt-1-i;j++)
        {
            ps[j]=(ps[j]+ps[j+1])/2;
            if(j==0)px.push_back(ps[0]);
        }
    }
    //二分法得到3阶递推关系点
    drawpix(ps[0].x(),ps[0].y());//绘制中间点
    drawhighbezier(cnt,px,level+1);//左段递归
    drawhighbezier(cnt,ps,level+1);//右段递归
}
