#include "deviceinfo.h"
#include "QDebug"
DeviceInfo::DeviceInfo(QObject *parent) : QObject(parent)
{

}
QString DeviceInfo::scanHardwareInfo()
{
    QProcess process;
//    process.start("hwinfo --short");
    process.start("lspci -nn");
    process.waitForFinished();
    return process.readAllStandardOutput();
}
