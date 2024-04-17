#include "downloadwidget.h"
#include <QHBoxLayout>
#include <QProgressBar>
#include <QLabel>

DownloadWidget::DownloadWidget(const QString &fileName, QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    progressBar = new QProgressBar(this);
    progressBar->setMinimum(0);
    progressBar->setMaximum(100);
    layout->addWidget(progressBar);

    QLabel *label = new QLabel(fileName, this);
    layout->addWidget(label);

    downloadStatus = new QLabel("Downloading", this);
    layout->addWidget(downloadStatus);
}

void DownloadWidget::setDownloadStatus(const QString &status)
{
    downloadStatus->setText(status);
}

void DownloadWidget::setProgress(int progress)
{
    progressBar->setValue(progress);
}
