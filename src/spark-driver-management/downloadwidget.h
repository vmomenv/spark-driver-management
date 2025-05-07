#ifndef DOWNLOADWIDGET_H
#define DOWNLOADWIDGET_H

#include <QLabel>
#include <QProgressBar>
#include <QWidget>

class DownloadWidget : public QWidget {
    Q_OBJECT
public:
    explicit DownloadWidget(const QString &url, QWidget *parent = nullptr);
    void updateProgress(qint64 bytesReceived, qint64 bytesTotal);
    void setDownloadStatus(const QString &status);
    QString getSavePath() const { return savePath; }
private slots:
    void openDownloadDir();

private:
    QProgressBar *progressBar;
    QLabel *statusLabel;
    QString savePath;
};

#endif // DOWNLOADWIDGET_H
