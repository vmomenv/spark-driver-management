#ifndef DRIVERLIST_H
#define DRIVERLIST_H

#include <QJsonDocument>
#include <QWidget>
#include<QJsonObject>
#include <QVBoxLayout>
class DriverList : public QWidget
{
    Q_OBJECT
public:
    explicit DriverList(QString type,QWidget *parent = nullptr);

    void infoWidget(QJsonDocument json);
    QJsonDocument json;
//    void infoWidget(const QJsonObject &deviceObject, QWidget *listWidget);
    void infoWidget(const QJsonObject &deviceObject, QVBoxLayout *listLayout);
    void requestType(QString type);
private:
    QJsonDocument tmpJson;
signals:

public slots:
};

#endif // DRIVERLIST_H
