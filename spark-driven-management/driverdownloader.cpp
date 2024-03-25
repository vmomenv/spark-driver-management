#include "driverdownloader.h"
#include "driverdownloader.h"

#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QListWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QProcess>
#include <QPushButton>
#include <QUrlQuery>
#include <QVBoxLayout>
#include <QJsonObject>
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

QJsonDocument DriverDownloader::getFilesByDeviceIds() {
    QList<QString> deviceIDs = this->deviceIDs;
    this->manager = new QNetworkAccessManager(this); // 将 manager 设置为成员变量
    QJsonArray jsonArray;
    QEventLoop loop;

    // 记录未完成的请求数量
    int pendingRequests = deviceIDs.size();

    // 构建请求的 URL
    auto buildRequestUrl = [](const QString &deviceID) -> QUrl {
        QUrl url("http://127.0.0.1:8000/api/FindFilesByHardwareId");
        QUrlQuery query;
        query.addQueryItem("driver_type", "pci");
        query.addQueryItem("device_id", deviceID);
        url.setQuery(query);
        return url;
    };

    for (const QString &deviceID : deviceIDs) {
        QUrl requestUrl = buildRequestUrl(deviceID);
        QNetworkRequest request(requestUrl);
        QNetworkReply *reply = this->manager->get(request); // 使用 this->manager

        // 当请求完成时触发该 lambda 表达式
        connect(reply, &QNetworkReply::finished, this, [&jsonArray, reply, &pendingRequests, &loop]() {
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray responseData = reply->readAll();
                QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
                QJsonArray smallArray = jsonDoc.array();
                if (!smallArray.isEmpty()) {
                    jsonArray.append(smallArray);
                }
            } else {
                qDebug() << "Error:" << reply->errorString();
            }
            reply->deleteLater();

            // 每当一个请求完成时，将未完成的请求数量减一
            pendingRequests--;

            // 如果所有请求都已完成，则退出事件循环
            if (pendingRequests == 0) {
                loop.quit();
            }
        });
    }

    // 等待所有请求完成
    loop.exec();

    // 将所有 JSON 数据组合成一个大 JSON
    QJsonDocument finalJson(jsonArray);

    return finalJson;
}
QJsonDocument DriverDownloader::getFileByType(QString type) {
    QNetworkAccessManager manager;
    QEventLoop loop;
    QObject::connect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

    QString url = "http://127.0.0.1:8000/api/FileDisplayByType?driver_type=" + type;
    QUrl urlObject(url);
    QNetworkRequest request(urlObject);


    QNetworkReply *reply = manager.get(request);
    loop.exec();

    QByteArray responseData = reply->readAll();
    reply->deleteLater();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
    return jsonDoc;
}
