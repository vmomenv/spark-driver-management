#ifndef DOWNLOADWIDGET_H
#define DOWNLOADWIDGET_H

#include <QLabel>
#include <QProgressBar>
#include <QWidget>

class DownloadWidget : public QWidget
{
    Q_OBJECT
public:

    DownloadWidget(const QString &fileName, QWidget *parent);
    void setDownloadStatus(const QString &status);
    void setProgress(int progress);
private:
    QProgressBar *progressBar;
    QLabel *downloadStatus;
signals:

public slots:
};

#endif // DOWNLOADWIDGET_H
