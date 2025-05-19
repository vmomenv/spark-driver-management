#ifndef DOWNLOADWIDGET_H
#define DOWNLOADWIDGET_H

#include <QLabel>
#include <QProgressBar>
#include <QWidget>
#include <QVBoxLayout>
#include <QMap>

class DownloadTaskWidget : public QWidget {
    Q_OBJECT
public:
    explicit DownloadTaskWidget(const QString &fileName, QWidget *parent = nullptr);
    void updateProgress(qint64 bytesReceived, qint64 bytesTotal);
    void setDownloadStatus(const QString &status);
    QString getFileName() const { return fileName; }
private:
    QProgressBar *progressBar;
    QLabel *statusLabel;
    QString fileName;
};

class DownloadWidget : public QWidget {
    Q_OBJECT
public:
    static DownloadWidget* instance(QWidget *parent = nullptr);
    void addDownloadTask(const QString &fileName);
    DownloadTaskWidget* getTaskWidget(const QString &fileName);
    QString getSavePath() const { return "/tmp/spark-driver/"; }
private slots:
    void openDownloadDir();
private:
    explicit DownloadWidget(QWidget *parent = nullptr);
    QVBoxLayout *mainLayout;
    QMap<QString, DownloadTaskWidget*> taskWidgets;
    static DownloadWidget* m_instance;
};

#endif // DOWNLOADWIDGET_H
