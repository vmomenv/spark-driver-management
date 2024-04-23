#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include <QObject>
#include <QProcess>
class DeviceInfo : public QObject
{
    Q_OBJECT
public:
    explicit DeviceInfo(QObject *parent = nullptr);
    QString scanHardwareInfo();

signals:

public slots:
};

#endif // DEVICEINFO_H
