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
    explicit DriverList(QWidget *parent = nullptr);

    void infoWidget(QJsonDocument json);
    QJsonDocument json;
//    void infoWidget(const QJsonObject &deviceObject, QWidget *listWidget);
    void infoWidget(const QJsonObject &deviceObject, QVBoxLayout *listLayout);
private:

signals:

public slots:
};

#endif // DRIVERLIST_H
