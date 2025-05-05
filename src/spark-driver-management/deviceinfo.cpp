#include "deviceinfo.h"
#include "QDebug"
DeviceInfo::DeviceInfo(QObject *parent) : QObject(parent)
{

}
QString DeviceInfo::scanHardwareInfo()
{
    QProcess process;
//    process.start("hwinfo --short");
    process.start("/usr/bin/lspci", QStringList() << "-nn");
    process.waitForFinished();
    // qDebug()<<process.readAllStandardOutput();
    return process.readAllStandardOutput();
}
