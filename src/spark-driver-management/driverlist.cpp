#include "driverdownloader.h"
#include "driverlist.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>

DriverList::DriverList(QString type,QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Widget to contain the list and additional spacer
    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);

    // List widget
    QWidget *listWidget = new QWidget(mainWidget);
    QVBoxLayout *listLayout = new QVBoxLayout(listWidget);
    listWidget->setObjectName("listWidget");
    mainLayout->addWidget(listWidget);

    // Spacer item to compress the listWidget's height
    QSpacerItem *spacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    mainLayout->addSpacerItem(spacer);

    layout->addWidget(mainWidget);

    DriverDownloader *jsoninfo = new DriverDownloader(this);
    if (type.isEmpty()) {
        json = jsoninfo->getFilesByDeviceIds();
    } else {
        json = jsoninfo->getFileByType(type);
        qDebug()<<json;
    }
    if (!json.isEmpty()) {
        // Parse the JSON document into smaller JSON objects
        QJsonArray jsonArray = json.array();
        for (int i = 0; i < jsonArray.size(); ++i) {
            QJsonArray deviceArray = jsonArray.at(i).toArray();
            for (int j = 0; j < deviceArray.size(); ++j) {
                QJsonObject deviceObject = deviceArray.at(j).toObject();
                // Create an infoWidget for each device
                infoWidget(deviceObject, listLayout); // 将 listLayout 传递给 infoWidget
            }
        }
    }
}

void DriverList::infoWidget(const QJsonObject &deviceObject, QVBoxLayout *listLayout)
{
    // Create an infoWidget to display information for each device
    QWidget *infoWidget = new QWidget();
    infoWidget->setObjectName("infoWidget");
    QHBoxLayout *layout = new QHBoxLayout(infoWidget);
    infoWidget->setFixedHeight(80);

    // 设置样式表，为infoWidget添加边框和白色背景，以及圆角
    infoWidget->setStyleSheet("QWidget#infoWidget { background-color: white; border: 1px solid black; border-radius: 15px; }");

    QString description = deviceObject.value("description").toString();
    QString fileName = deviceObject.value("file_name").toString();
    QString fileSize = deviceObject.value("file_size").toString();
    QString packageName = deviceObject.value("package_name").toString();
    QString version = deviceObject.value("version").toString();
    QString filePath = deviceObject.value("file_path").toString();

    QLabel *fileNameLabel = new QLabel("文件名: " + fileName);
    QLabel *fileSizeLabel = new QLabel("文件大小: " + fileSize);
    QLabel *packageNameLabel = new QLabel("包名: " + packageName);
    QLabel *versionLabel = new QLabel("版本: " + version);
    QLabel *descriptionLabel = new QLabel("描述: " + description);

    QWidget *infoWidgetLeft = new QWidget();
    QVBoxLayout *infoWidgetLeftLayout = new QVBoxLayout(infoWidgetLeft);
    infoWidgetLeftLayout->addWidget(fileNameLabel);

    QWidget *infoWidgetRight = new QWidget();
    QVBoxLayout *infoWidgetRightLayout = new QVBoxLayout(infoWidgetRight);
    infoWidgetRight->setFixedSize(80, 35);

    QHBoxLayout *infoWidgetBottomLayout = new QHBoxLayout();
    infoWidgetBottomLayout->addWidget(fileSizeLabel);
    infoWidgetBottomLayout->addWidget(packageNameLabel);
    infoWidgetBottomLayout->addWidget(versionLabel);
    infoWidgetBottomLayout->addWidget(descriptionLabel);
    infoWidgetLeftLayout->addLayout(infoWidgetBottomLayout);

    QPushButton *downloadButton = new QPushButton("下载");
    downloadButton->setFixedSize(65, 25);
    connect(downloadButton, &QPushButton::clicked, [=]() {
        DriverDownloader *download=new DriverDownloader;
        download->downloadFile(filePath);
    });
    infoWidgetRightLayout->addWidget(downloadButton);

    layout->addWidget(infoWidgetLeft);
    layout->addWidget(infoWidgetRight);

    listLayout->addWidget(infoWidget);
}
//void DriverList::requestType(QString type){
//    DriverDownloader *jsoninfo;
//    if(type==NULL){
//        this->tmpJson=jsoninfo->getFilesByDeviceIds();
//    }
//}
