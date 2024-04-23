#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "deviceinfo.h"
#include "driverlist.h"
#include<QJsonObject>
#include<QJsonDocument>
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
    connect(ui->tabWidget_3,&QTabWidget::currentChanged, this, &MainWindow::downloadFileByType);
    onStackedWidgetPageChanged(0);


    QNetworkAccessManager manager;
    QEventLoop loop;
    QObject::connect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

    QString url = "http://127.0.0.1:8000/serverlist";
    QUrl urlObject(url);
    QNetworkRequest request(urlObject);

    QNetworkReply *reply = manager.get(request);
    loop.exec();

    QByteArray responseData = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        QJsonObject jsonObj = jsonDoc.object();
        if (jsonObj.contains("links") && jsonObj["links"].isArray()) {
            QJsonArray linksArray = jsonObj["links"].toArray();
            foreach (const QJsonValue &value, linksArray) {
                if (value.isObject()) {
                    QJsonObject linkObj = value.toObject();
                    if (linkObj.contains("name") && linkObj.contains("url")) {
                        QString name = linkObj["name"].toString();
                        QString url = linkObj["url"].toString();
                        // 将数据添加到comboBox中
                        ui->comboBox->addItem(name, url);
                    }
                }
            }
        }
    }
    reply->deleteLater();

//当combobox发生更改时，将数据传给DriverDownloader
    connect(ui->comboBox,&QComboBox::currentTextChanged,this,[=]{
        qDebug()<<ui->comboBox->itemData(ui->comboBox->currentIndex());
    });
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



void MainWindow::on_pushButton_5_clicked() {
    ui->stackedWidget->setCurrentIndex(1);

    // 如果之前已经创建了 driverList，则先释放它
    if (driverList) {
        delete driverList;
        driverList = nullptr;
    }
    // 创建新的 DriverList
    driverList = new DriverList("",this);

    // 检查tab1的布局是否已经存在
    if (ui->tab1->layout()) {
        // 如果存在，则删除旧布局
        QLayoutItem *item;
        while ((item = ui->tab1->layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
    }

    // 添加driverList到tab1
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(ui->tab1->layout());
    if (!layout) {
        layout = new QVBoxLayout(ui->tab1);
    }
    layout->addWidget(driverList);
}

void MainWindow::on_pushButton_7_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    downloadFileByType(0);

}

void MainWindow::downloadFileByType(int index){
    qDebug()<<index;
    // 如果之前已经创建了 driverList，则先释放它
    if (driverList) {
        delete driverList;
        driverList = nullptr;
    }
    // 创建新的 DriverList
    QStringList stringList;
    stringList << "VGA" << "Network" << "Audio" << "Camera" << "Printer" << "Kernel";
    qDebug()<<"----------"<<stringList.at(index);
    driverList = new DriverList(stringList.at(index),this);
    QVector<QWidget*> tabWidgets = {ui->tab_4, ui->tab_5,  ui->tab_6, ui->tab_7, ui->tab_8, ui->tab_9};
    QWidget *tabWidget = tabWidgets[index];
    // 检查tab1的布局是否已经存在
    if (tabWidget->layout()) {
        // 如果存在，则删除旧布局
        QLayoutItem *item;
        while ((item = tabWidget->layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
    }

    // 添加driverList到tab1
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(tabWidget->layout());
    if (!layout) {
        layout = new QVBoxLayout(tabWidget);
    }
    layout->addWidget(driverList);
}

void MainWindow::on_pushButton_8_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);

}
