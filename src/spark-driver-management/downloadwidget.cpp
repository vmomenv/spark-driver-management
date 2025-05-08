#include "downloadwidget.h"
#include <QDesktopServices>
#include <QDir>
#include <QPushButton>
#include <QStandardPaths>
#include <QVBoxLayout>

DownloadWidget::DownloadWidget(const QString &url, QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("下载进度");
    setFixedSize(400, 150);

    QVBoxLayout *layout = new QVBoxLayout(this);

    progressBar = new QProgressBar(this);
    progressBar->setRange(0, 100);

    statusLabel = new QLabel("准备下载...", this);
    statusLabel->setAlignment(Qt::AlignCenter);

    QPushButton *openBtn = new QPushButton("打开下载目录", this);
    connect(openBtn, &QPushButton::clicked, this, &DownloadWidget::openDownloadDir);

    layout->addWidget(statusLabel);
    layout->addWidget(progressBar);
    layout->addWidget(openBtn);

    // 设置默认保存路径
    savePath = "/tmp/spark-driver";
    QDir().mkpath(savePath);
}

void DownloadWidget::updateProgress(qint64 bytesReceived, qint64 bytesTotal) {
    if(bytesTotal > 0) {
        int progress = (bytesReceived * 100) / bytesTotal;
        progressBar->setValue(progress);
        statusLabel->setText(QString("下载中: %1/%2 MB")
                                 .arg(bytesReceived/1024/1024)
                                 .arg(bytesTotal/1024/1024));
    }
}

void DownloadWidget::setDownloadStatus(const QString &status) {
    statusLabel->setText(status);
    if(status == "下载完成") {
        progressBar->setValue(100);
    }
}

void DownloadWidget::openDownloadDir() {
    QDesktopServices::openUrl(QUrl::fromLocalFile(savePath));
}

