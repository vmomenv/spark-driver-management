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

}
void DriverDownloader::fetchDriverInfo()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    for (const QString &deviceId : deviceIDs) {
        QUrl apiUrl("http://127.0.0.1:8000/api/FindFilesByHardwareId/");
        QUrlQuery query;
        query.addQueryItem("driver_type", "pci");
        query.addQueryItem("device_id", deviceId);
        apiUrl.setQuery(query);

        QNetworkRequest request(apiUrl);
        QNetworkReply *reply = manager->get(request);

        connect(reply, &QNetworkReply::finished, this, [this, reply]() {
            onDriverInfoReceived(reply);
        });
    }
}
void DriverDownloader::onDriverInfoReceived(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        QJsonArray driverInfoArray = jsonDoc.array();

        // 追加到中间变量
        intermediateJsonArray.append(driverInfoArray);

        // 检查是否有数据，如果有则发送信号
        if (!driverInfoArray.isEmpty()) {
            emit driverInfoReceived(intermediateJsonArray);
        }
    } else {
        qDebug() << "Network error:" << reply->errorString();
    }

    reply->deleteLater();
}
void DriverDownloader::processDriverInfo(const QJsonArray &driverInfoArray)
{
    // 处理驱动信息，你可以在这里做进一步的操作
    qDebug() << "Received driver info:" << driverInfoArray;

    // 清空中间变量，准备下一次的数据
    intermediateJsonArray = QJsonArray();
}
