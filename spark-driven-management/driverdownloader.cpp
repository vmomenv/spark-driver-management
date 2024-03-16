#include "driverdownloader.h"
#include "driverdownloader.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QListWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QProcess>
#include <QPushButton>
#include <QUrlQuery>
#include <QVBoxLayout>

DriverDownloader::DriverDownloader(QWidget *parent) : QWidget(parent)
{
    executeCommand();
}
void DriverDownloader::executeCommand()
{
    // 创建 QProcess 对象
    QProcess process;
    QListWidget driverListWidget;

    // 设置要执行的命令和参数
    QString command = "lspci";
    QStringList arguments;
    arguments << "-nn";

    // 启动进程并等待完成
    process.start(command, arguments);
    process.waitForFinished();

    // 从进程中读取输出
    QString result = process.readAllStandardOutput();
    qDebug()<<result;    // 使用正则表达式提取设备ID
    QRegExp regex("\\[(\\w+):(\\w+)\\]");
    int pos = 0;
    while ((pos = regex.indexIn(result, pos)) != -1) {
        deviceIDs << regex.cap(1) + ":" + regex.cap(2);
        pos += regex.matchedLength();
    }
    qDebug()<<deviceIDs;
    getFilesByDeviceIds();

}
QString DriverDownloader::getFilesByDeviceIds()
{
    QList<QString> deviceIDs = this->deviceIDs;
    manager = new QNetworkAccessManager(this);
    QString responseData;
    for (const QString &deviceID : deviceIDs) {
        QUrl requestUrl("http://127.0.0.1:8000/api/FindFilesByHardwareId?driver_type=pci&device_id=" + deviceID);
        QNetworkRequest request(requestUrl);
        QNetworkReply *reply = manager->get(request);
        connect(reply, &QNetworkReply::finished, this, [this, &responseData, reply]() {
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray response_data = reply->readAll();
                // 将响应数据存储在中间变量中
                qDebug()<<response_data;
                responseData += QString(response_data);
            } else {
                qDebug() << "Error:" << reply->errorString();
            }
            reply->deleteLater();
        });

    }
//     return responseData;
}
