#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QColorDialog>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);//禁止最大化
    ui->graphicsView->viewport()->installEventFilter(this);//事件过滤器
    ui->graphicsView->viewport()->setMouseTracking(true);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    model=DEFAULT;
}

void MainWindow::on_pushButton_2_clicked()
{
    model=DRAWLINE;
}

void MainWindow::on_pushButton_3_clicked()
{
    model=DRAWARCH;
}

void MainWindow::on_pushButton_5_clicked()
{
    model=DRAWPAINT;
}

void MainWindow::on_pushButton_4_clicked()
{
    QColorDialog col;//调出颜色选择器对话框
    QColor c = col.getColor();
    color=qRgb(c.red(),c.green(),c.blue());
}

void MainWindow::on_pushButton_6_clicked()
{
    init();
}
