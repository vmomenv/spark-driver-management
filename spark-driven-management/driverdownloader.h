#ifndef DRIVERDOWNLOADER_H
#define DRIVERDOWNLOADER_H

#include <QWidget>
#include<QDebug>
#include <QNetworkReply>
#include <QJsonArray>
class DriverDownloader : public QWidget
{
    Q_OBJECT
public:
    explicit DriverDownloader(QWidget *parent = nullptr);
    void executeCommand();
    void fetchDriverInfo();
    void onDriverInfoReceived(QNetworkReply *reply);
    void processDriverInfo(const QJsonArray &driverInfoArray);
private:
    QStringList deviceIDs;
    QJsonArray intermediateJsonArray;
    QNetworkAccessManager *networkManager;
signals:
    void driverInfoReceived(const QJsonArray &driverInfoArray);

public slots:
};

#endif // DRIVERDOWNLOADER_H
