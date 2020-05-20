#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>
#include <QDebug>
#include <QMouseEvent>
#include <QEvent>

void MainWindow::drawpix(int x, int y)
{
    for(int i=x-1;i<=x+1;i++)
    {
        for(int j=y-1;j<=y+1;j++)
        {
            if(i>=0&&i<datawidth&&j>=0&&j<dataheight)img.setPixel(i,j,color);
        }
    }
}
void swapx(int &a,int &b)
{
    a=a^b;
    b=a^b;
    a=a^b;
}
void MainWindow::drawline(QPoint p0,QPoint p1)//画线函数
{
        int x0=p0.x(),y0=p0.y();
        int x1=p1.x(),y1=p1.y();
        int x,y,dx,dy,e;
        dx=x1-x0,dy=y1-y0;
        int state=0;//0为0<k<1,1为-1<k<0,2为k>1,3为k<-1
        if((dy>=0&&dx>=0)||(dy<=0&&dx<=0))//斜率为正,或水平，或竖直(水平线不影响，竖直线等价于斜率>1)
        {
            if(abs(dy)>=abs(dx)||dx==0)//斜率>1
            {
                swapx(x0,y0);
                swapx(x1,y1);
                swapx(dx,dy);
                state=2;
                //交换所有点的(x,y)计算，然后在绘图的时候交换(x,y)
            }
            else state=0;
        }
        else//斜率为负
        {
            if(abs(dy)>abs(dx))//  k<-1
            {
                x0=-x0;
                x1=-x1;
                swapx(x0,y0);
                swapx(x1,y1);
                dx=-dx;
                swapx(dx,dy);
                state=3;
                //(x,y)对应(y,-x)
            }
            else  //0>k>-1
            {
                x0=-x0;
                x1=-x1;
                dx=-dx;
                state=1;
                //关于y轴对称，然后绘图的时候绘制(-x,y)
            }
        }
        //特殊处理
        if(x0>x1)
        {
            swapx(x0,x1);
            swapx(y0,y1);
            dx=-dx;
            dy=-dy;
        }
        x=x0,y=y0;
        e=2*dy-dx;//初值e
        for(int i=0;i<=dx;i++)
        {
//            qDebug()<<p0<<p1<<state<<x<<y<<dx<<dy<<e<<endl;
            if(state==0)drawpix(x,y);
            else if(state==2)drawpix(y,x);
            else if(state==1)drawpix(-x,y);
            else if(state==3)drawpix(-y,x);
            x++;
            if(e<0)
            {
                e=2*dy+e;
            }
            else if(e>=0){
                y++;
                e=e+2*(dy-dx);
            }
        }

}


void MainWindow::fflush()
{
    if(scene!=nullptr)delete scene;
    scene = new QGraphicsScene;
//    simg=img;
    for(int i=0;i<dataheight;i++)
    {
        for(int j=0;j<datawidth;j++)
        {
            int cnt=0;
            int r=0,g=0,b=0;
            if(j-1>=0)
            {
                QRgb col=img.pixel(j-1,i);
                r+=qRed(col);
                g+=qGreen(col);
                b+=qBlue(col);
                cnt++;
            }
            if(j+1<datawidth)
            {
                QRgb col=img.pixel(j+1,i);
                r+=qRed(col);
                g+=qGreen(col);
                b+=qBlue(col);
                cnt++;
            }
            if(i-1>=0)
            {
                QRgb col=img.pixel(j,i-1);
                r+=qRed(col);
                g+=qGreen(col);
                b+=qBlue(col);
                cnt++;
            }
            if(i+1<dataheight)
            {
                QRgb col=img.pixel(j,i+1);
                r+=qRed(col);
                g+=qGreen(col);
                b+=qBlue(col);
                cnt++;
            }
            r/=cnt;
            g/=cnt;
            b/=cnt;
//            QRgb tmpcol=QColor::fromRgb(r,g,b).rgba();
//            simg.setPixel(j,i,tmpcol);
        }
    }

    scene->addPixmap(QPixmap::fromImage(img));

    ui->graphicsView->setScene(scene);
    ui->graphicsView->resize(ui->graphicsView->width(), ui->graphicsView->height());
    ui->graphicsView->show();

}
bool MainWindow::eventFilter(QObject *obj,QEvent *event)
{
    if(obj==ui->graphicsView->viewport())
    {
        if(event->type()==QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent=(QMouseEvent *)event;
            if(mouseEvent->buttons()&Qt::LeftButton)
            {
                mouse_press=true;
                if(archstate==0&&preimg.size()<=10000)preimg.push(img.copy());//记录上一次图像
                switch(model)
                {
                    case DEFAULT:DEFAULT_PRESS(mouseEvent);break;
                    case DRAWLINE:DRAWLINE_PRESS(mouseEvent);break;
                    case DRAWPAINT:DRAWPAINT_PRESS(mouseEvent);break;
                    case DRAWARCH:DRAWARCH_PRESS(mouseEvent);break;
                }
                fflush();
            }
            else if(mouseEvent->buttons()&Qt::RightButton)//右键确定
            {
//                if(archstate!=0&&preimg.size()<=10000)preimg.push(img.copy());//记录上一次图像
                switch(model)
                {
                    case DRAWARCH:archstate=0;showarch();break;//回到最初的标记
                }
                fflush();
            }
            return true;
        }
        if(event->type()==QEvent::MouseMove)
        {
           QMouseEvent *mouseEvent = (QMouseEvent *)event;
           if(mouse_press==true)
           {
               switch(model)
               {
                   case DEFAULT:DEFAULT_MOVE(mouseEvent);break;
                   case DRAWLINE:DRAWLINE_MOVE(mouseEvent);break;
                   case DRAWARCH:DRAWARCH_MOVE(mouseEvent);break;
               }
               fflush();
           }
           return true;
        }
        if(event->type()==QEvent::MouseButtonRelease)
        {
            QMouseEvent *mouseEvent=(QMouseEvent *)event;
//            qDebug()<<mouseEvent->pos()<<endl;
            if(mouseEvent->button()&Qt::LeftButton)
            {
                mouse_press=false;
                switch(model)
                {
                    case DEFAULT:DEFAULT_UP(mouseEvent);break;
                    case DRAWLINE:DRAWLINE_UP(mouseEvent);break;
                    case DRAWARCH:DRAWARCH_UP(mouseEvent);break;
                }
                fflush();
            }
            return true;
        }
    }
    return false;
}
