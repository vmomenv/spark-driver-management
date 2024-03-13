#include "driverdownloader.h"

#include <QListWidget>
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
