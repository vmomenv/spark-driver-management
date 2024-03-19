#include "driverdownloader.h"
#include "driverlist.h"

#include <QLabel>
#include <QVBoxLayout>

DriverList::DriverList(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QWidget *listWidget = new QWidget(this);
    QVBoxLayout *listLayout = new QVBoxLayout(listWidget); // 设置垂直布局
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
    QVBoxLayout *layout = new QVBoxLayout(infoWidget);

    // Extracting information from the deviceObject
    QString description = deviceObject.value("description").toString();
    QString fileName = deviceObject.value("file_name").toString();
    QString filePath = deviceObject.value("file_path").toString();
    QString fileSize = deviceObject.value("file_size").toString();
    QString packageName = deviceObject.value("package_name").toString();
    QString version = deviceObject.value("version").toString();

    // Creating QLabel widgets to display information
    QLabel *descriptionLabel = new QLabel("Description: " + description);
    QLabel *fileNameLabel = new QLabel("File Name: " + fileName);
    QLabel *filePathLabel = new QLabel("File Path: " + filePath);
    QLabel *fileSizeLabel = new QLabel("File Size: " + fileSize);
    QLabel *packageNameLabel = new QLabel("Package Name: " + packageName);
    QLabel *versionLabel = new QLabel("Version: " + version);

    // Adding QLabel widgets to the layout
    layout->addWidget(descriptionLabel);
    layout->addWidget(fileNameLabel);
    layout->addWidget(filePathLabel);
    layout->addWidget(fileSizeLabel);
    layout->addWidget(packageNameLabel);
    layout->addWidget(versionLabel);

    // Adding the infoWidget to the listLayout
    listLayout->addWidget(infoWidget);
}
