#include "downloadwidget.h"
#include <QDesktopServices>
#include <QDir>
#include <QPushButton>
#include <QStandardPaths>
#include <QUrl>
#include <QVBoxLayout>
#include <QApplication>
#include <QScreen>

DownloadWidget* DownloadWidget::m_instance = nullptr;

DownloadWidget* DownloadWidget::instance(QWidget *parent) {
    if (!m_instance) {
        m_instance = new DownloadWidget(parent);
    }
    m_instance->show();
    m_instance->raise();
    m_instance->activateWindow();
    return m_instance;
}

DownloadWidget::DownloadWidget(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("下载管理器");
    setFixedSize(420, 500);

    // 居中显示窗口
    QRect screenGeometry = QApplication::primaryScreen()->geometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);

    mainLayout = new QVBoxLayout(this);

    QPushButton *openBtn = new QPushButton("打开下载目录", this);
    connect(openBtn, &QPushButton::clicked, this, &DownloadWidget::openDownloadDir);

    mainLayout->addWidget(openBtn);
    mainLayout->addStretch();

    // 设置默认保存路径
    QDir().mkpath(getSavePath());
}

void DownloadWidget::addDownloadTask(const QString &fileName) {
    if (taskWidgets.contains(fileName)) return;
    DownloadTaskWidget *task = new DownloadTaskWidget(fileName, this);
    mainLayout->insertWidget(mainLayout->count() - 1, task); // 在stretch前插入
    taskWidgets[fileName] = task;
}

DownloadTaskWidget* DownloadWidget::getTaskWidget(const QString &fileName) {
    return taskWidgets.value(fileName, nullptr);
}

void DownloadWidget::openDownloadDir() {
    QDesktopServices::openUrl(QUrl::fromLocalFile(getSavePath()));
}

// --- DownloadTaskWidget 实现 ---
DownloadTaskWidget::DownloadTaskWidget(const QString &fileName, QWidget *parent)
    : QWidget(parent), fileName(fileName)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    statusLabel = new QLabel("准备下载: " + fileName, this);
    statusLabel->setAlignment(Qt::AlignLeft);
    progressBar = new QProgressBar(this);
    progressBar->setRange(0, 100);
    layout->addWidget(statusLabel);
    layout->addWidget(progressBar);
    setFixedHeight(60);
}

void DownloadTaskWidget::updateProgress(qint64 bytesReceived, qint64 bytesTotal) {
    if(bytesTotal > 0) {
        int progress = (bytesReceived * 100) / bytesTotal;
        progressBar->setValue(progress);
        statusLabel->setText(QString("%1: %2/%3 MB")
                                 .arg(fileName)
                                 .arg(bytesReceived/1024/1024)
                                 .arg(bytesTotal/1024/1024));
    }
}

void DownloadTaskWidget::setDownloadStatus(const QString &status) {
    statusLabel->setText(QString("%1: %2").arg(fileName, status));
    if(status == "下载完成") {
        progressBar->setValue(100);
    }
}

