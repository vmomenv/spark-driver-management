#include "driverdownloader.h"
#include "driverlist.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout> // 添加QHBoxLayout头文件

DriverList::DriverList(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QWidget *listWidget = new QWidget(this);
    QVBoxLayout *listLayout = new QVBoxLayout(listWidget); // 设置垂直布局
    listWidget->setObjectName("listWidget"); // 设置对象名以在样式表中使用
    layout->addWidget(listWidget);

    DriverDownloader *jsoninfo = new DriverDownloader(this);
    QJsonDocument json = jsoninfo->getFilesByDeviceIds();

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
    infoWidget->setObjectName("infoWidget"); // 设置对象名以在样式表中使用
    QHBoxLayout *layout = new QHBoxLayout(infoWidget); // 使用水平布局
    infoWidget->setFixedHeight(80); // 设置整体widget高度为45

    // 设置样式表，为infoWidget添加边框和白色背景
    infoWidget->setStyleSheet("QWidget#infoWidget { background-color: white; border: 1px solid black; }");

    // Extracting information from the deviceObject
    QString description = deviceObject.value("description").toString();
    QString fileName = deviceObject.value("file_name").toString();
    QString fileSize = deviceObject.value("file_size").toString();
    QString packageName = deviceObject.value("package_name").toString();
    QString version = deviceObject.value("version").toString();

    // Creating QLabel widgets to display information
    QLabel *fileNameLabel = new QLabel("文件名: " + fileName);
    QLabel *fileSizeLabel = new QLabel("文件大小: " + fileSize);
    QLabel *packageNameLabel = new QLabel("包名: " + packageName);
    QLabel *versionLabel = new QLabel("版本: " + version);
    QLabel *descriptionLabel = new QLabel("描述: " + description);

    // Creating a widget for information (fileNameLabel)
    QWidget *infoWidgetLeft = new QWidget();
    QVBoxLayout *infoWidgetLeftLayout = new QVBoxLayout(infoWidgetLeft);
    infoWidgetLeftLayout->addWidget(fileNameLabel);

    // Creating a widget for buttons
    QWidget *infoWidgetRight = new QWidget();
    QVBoxLayout *infoWidgetRightLayout = new QVBoxLayout(infoWidgetRight);
    infoWidgetRight->setFixedSize(80, 35); // 设置infoWidgetRight的大小为70*25

    // Adding QLabel widgets to the layout
    QHBoxLayout *infoWidgetBottomLayout = new QHBoxLayout();
    infoWidgetBottomLayout->addWidget(fileSizeLabel);
    infoWidgetBottomLayout->addWidget(packageNameLabel);
    infoWidgetBottomLayout->addWidget(versionLabel);
    infoWidgetBottomLayout->addWidget(descriptionLabel);
    infoWidgetLeftLayout->addLayout(infoWidgetBottomLayout);

    // Adding download button
    QPushButton *downloadButton = new QPushButton("下载");
    downloadButton->setFixedSize(65, 25); // 设置按钮大小为65*25
    // Connect the download button's clicked signal to a slot
    connect(downloadButton, &QPushButton::clicked, [=]() {
        // Here you can handle the download action
        // For now, let's just print the filePath
        qDebug() << "Downloading file from: " << deviceObject.value("file_path").toString();
    });
    infoWidgetRightLayout->addWidget(downloadButton);

    // Adding widgets to the main layout
    layout->addWidget(infoWidgetLeft);
    layout->addWidget(infoWidgetRight);

    // Adding the infoWidget to the listLayout
    listLayout->addWidget(infoWidget);
}
