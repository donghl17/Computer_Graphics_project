#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>
#include <QDebug>
#include <QMouseEvent>

void MainWindow::DEFAULT_PRESS(QMouseEvent *event)//画笔按下
{
    int x=event->x();
    int y=event->y();
    curpoint=lastpoint=event->pos();//当前点
    drawpix(x,y);
}
void MainWindow::DEFAULT_MOVE(QMouseEvent *event)//画笔拖动
{
    lastpoint=curpoint;//上一次的点
    curpoint=event->pos();//当前的点
    drawline(lastpoint,curpoint);

}

void MainWindow::DEFAULT_UP(QMouseEvent *event)//画笔抬起
{
    int x=event->x();
    int y=event->y();
    drawpix(x,y);
}


