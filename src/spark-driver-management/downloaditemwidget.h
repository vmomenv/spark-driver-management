#ifndef DOWNLOADITEMWIDGET_H
#define DOWNLOADITEMWIDGET_H

#include <QWidget>
#include <QProgressBar>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class DownloadItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DownloadItemWidget(const QString &fileName, const QUrl &url, const QString &saveDir, QWidget *parent = nullptr);
private slots:
    void onDownloadProgress(qint64 bytesReceived,qint64 bytesTotal);
    void onDownloadFinished();
private:
    QLabel *statusLabel;
    QProgressBar *progressBar;
    QNetworkReply *reply;
    QString savePath;
    QString fileName;
signals:
};

#endif // DOWNLOADITEMWIDGET_H
