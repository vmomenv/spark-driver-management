#ifndef DRIVERDOWNLOADER_H
#define DRIVERDOWNLOADER_H

#include <QWidget>
#include<QDebug>
class DriverDownloader : public QWidget
{
    Q_OBJECT
public:
    explicit DriverDownloader(QWidget *parent = nullptr);
    void executeCommand();
signals:

public slots:
};

#endif // DRIVERDOWNLOADER_H
