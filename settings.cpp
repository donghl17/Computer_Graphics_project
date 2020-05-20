#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>
#include<QDebug>

void MainWindow::init()
{
        model=DEFAULT;//默认画笔
        color=QColor::fromRgb(0,0,0).rgba();//黑·笔
        frac=4;//默认4阶贝塞尔
    mouse_press=false;
    ctrpoint.clear();//清除
    archstate=0;//初始化
    datawidth=800;
    dataheight=600;
    lens=datawidth*dataheight*3;
    data=new unsigned char[lens];
    for(int i=0;i<lens;i++)data[i]=255;//全置为白色
    img=QImage(data,datawidth,dataheight,QImage::Format_RGB888);
    scene = new QGraphicsScene;
    scene->addPixmap(QPixmap::fromImage(img));
    ui->graphicsView->setScene(scene);
    ui->graphicsView->resize(ui->graphicsView->width(), ui->graphicsView->height());
    ui->graphicsView->show();


}
