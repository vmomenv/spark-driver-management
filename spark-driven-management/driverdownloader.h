#ifndef DRIVERDOWNLOADER_H
#define DRIVERDOWNLOADER_H

#include <QWidget>
#include<QDebug>
#include <QNetworkReply>
#include <QJsonArray>
#include<QDebug>
#include <QJsonDocument>
class DriverDownloader : public QWidget
{
    Q_OBJECT
public:
    explicit DriverDownloader(QWidget *parent = nullptr);
    void executeCommand();

    QJsonDocument getFilesByDeviceIds();
    QJsonDocument getFileByType(QString type);
private:
    QStringList deviceIDs;
    QJsonArray intermediateJsonArray;
    QNetworkAccessManager *manager;
    QString responseData;
signals:
    void driverInfoReceived(const QJsonArray &driverInfoArray);

public slots:
};

#endif // DRIVERDOWNLOADER_H
