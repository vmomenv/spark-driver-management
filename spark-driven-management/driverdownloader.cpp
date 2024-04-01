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
#include <QFileInfo>
#include <QDialog>
#include <QFileDialog>
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
    qDebug()<<finalJson;
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
    qDebug() << jsonDoc;

    QJsonArray jsonArray;
    if (!jsonDoc.isNull() && (jsonDoc.isObject() || jsonDoc.isArray())) {
        if (jsonDoc.isArray()) {
            jsonArray = jsonDoc.array(); // 如果是数组，则直接使用
        } else {
            QJsonObject jsonObject = jsonDoc.object();
            jsonArray.append(jsonObject); // 如果是对象，则添加到数组中
        }
    }

    // 将新的 JSON 数组转换为 JSON 文档并返回
    QJsonDocument finalJson(QJsonArray({jsonArray}));
    qDebug() << finalJson;
    return finalJson;
}
void DriverDownloader::downloadFile(const QString &filePath)
{
    // 构建下载URL
    QString baseUrl = "http://127.0.0.1/spark-driver-repo/";
    QString fileName = QFileInfo(filePath).fileName();
    QString downloadUrl = baseUrl + fileName + "/" + fileName;
    qDebug()<<downloadUrl<<"---------";
    // 创建下载列表widget
    QDialog *downloadDialog = new QDialog(this);
    QVBoxLayout *layout = new QVBoxLayout(downloadDialog);
    QListWidget *downloadListWidget = new QListWidget(downloadDialog);
    QPushButton *closeButton = new QPushButton("Close", downloadDialog);

    layout->addWidget(downloadListWidget);
    layout->addWidget(closeButton);
    downloadDialog->setLayout(layout);

    // 发起下载请求
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request(downloadUrl);
    QNetworkReply *reply = manager->get(request);

    // 处理下载响应
    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray fileData = reply->readAll();

            // 保存文件到本地（可选）
            QString saveFilePath = QFileDialog::getSaveFileName(this, tr("Save File"), fileName);
            QFile file(saveFilePath);
            if (file.open(QIODevice::WriteOnly)) {
                file.write(fileData);
                file.close();
            }

            // 添加到下载列表
            downloadListWidget->addItem(fileName + " - Downloaded");
        } else {
            // 添加到下载列表
            downloadListWidget->addItem(fileName + " - Error: " + reply->errorString());
        }

        // 清理资源
        reply->deleteLater();
    });

    // 显示下载列表
    downloadDialog->exec();
}
