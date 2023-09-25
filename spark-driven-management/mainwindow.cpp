#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPalette palette;
    QColor backgroundColor("#FFECD8");
    palette.setColor(QPalette::Background,backgroundColor);
    this->setPalette(palette);
    QPalette palette_2;
    QColor backgroundColor_2("#FFFFFF");
    palette_2.setColor(QPalette::Background,backgroundColor_2);
    ui->widget->setPalette(palette_2);

}

MainWindow::~MainWindow()
{
    delete ui;
}

