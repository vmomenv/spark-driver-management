#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "deviceinfo.h"

#include <QDesktopWidget>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    isMousePressed = false;
//    ui->backButton->setVisible(false);
    ui->stackedWidget->setCurrentIndex(0);
    QPalette palette;
    QColor backgroundColor("#FFECD8");
    palette.setColor(QPalette::Background,backgroundColor);
    this->setPalette(palette);
    QPalette palette_2;
    QColor backgroundColor_2("#FFFFFF");
    palette_2.setColor(QPalette::Background,backgroundColor_2);
    //设置标题栏隐藏
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
    setWindowTitle("");
    setWindowFlags(Qt::FramelessWindowHint);
    //设置窗口居中
    QDesktopWidget *desktop = QApplication::desktop();
    int screenWidth = desktop->width();
    int screenHeight = desktop->height();
    int windowWidth = this->width();
    int windowHeight = this->height();
    // 计算窗口在屏幕中央的位置
    int x = (screenWidth - windowWidth) / 2;
    int y = (screenHeight - windowHeight) / 2;
    // 设置窗口位置
    this->move(x, y);

    connect(ui->stackedWidget, &QStackedWidget::currentChanged, this, &MainWindow::onStackedWidgetPageChanged);
    onStackedWidgetPageChanged(0);
}
void MainWindow::onStackedWidgetPageChanged(int currentIndex)
{
    if (currentIndex == 0) {
        ui->backButton->hide();
    } else {
        ui->backButton->show();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_6_clicked()
{
    ui->deviceInfoWidget->setStyleSheet("background-color: white;");

    // 创建 DeviceInfo 对象
    DeviceInfo deviceInfo;
    // 调用扫描硬件信息的函数
    QString hardwareInfo = deviceInfo.scanHardwareInfo();
    // 在文本控件中显示硬件信息
    ui->textEditDeviceInfo->setPlainText(hardwareInfo);
    ui->textEditDeviceInfo->setReadOnly(true);
//    qDebug()<<hardwareInfo;
    QString imagePath = ":/res/information.png";
    QPixmap pixmap(imagePath);
    ui->deviceInfoPic->setPixmap(pixmap);
    QPixmap scaledPixmap = pixmap.scaled(QSize(50, 50), Qt::KeepAspectRatio);
    // 设置调整大小后的QPixmap给QLabel
    ui->deviceInfoPic->setPixmap(scaledPixmap);
    ui->stackedWidget->setCurrentIndex(2);

}
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    isMousePressed = true;
    dragPosition = event->globalPos() - frameGeometry().topLeft();
}
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (isMousePressed)
    {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    isMousePressed = false;
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_closeButton_clicked()
{
    this->close();
}

void MainWindow::on_minimizeButton_clicked()
{
    this->showMinimized();
}

void MainWindow::on_backButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_5_clicked()
{
  ui->stackedWidget->setCurrentIndex(1);
  DriverDownloader *w = new DriverDownloader(this);

}
