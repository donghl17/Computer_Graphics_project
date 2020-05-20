#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>
#include <QDebug>
#include <QMouseEvent>

void MainWindow::DRAWLINE_PRESS(QMouseEvent *event)
{
    lastpoint=event->pos();
    drawpix(event->x(),event->y());//起始点画
    imgtmp=img;
}
void MainWindow::DRAWLINE_MOVE(QMouseEvent *event)
{
    img=imgtmp;
    curpoint=event->pos();
    drawline(lastpoint,curpoint);//持续画线
}
void MainWindow::DRAWLINE_UP(QMouseEvent *event)//松开左键，画线
{
    img=imgtmp;
    curpoint=event->pos();
    drawline(lastpoint,curpoint);
}
