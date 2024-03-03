#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QDebug"
#include "QMenuBar"
#include "QMouseEvent"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_6_clicked();

    void on_pushButton_2_clicked();

    void on_closeButton_clicked();

    void on_minimizeButton_clicked();
    void onStackedWidgetPageChanged(int currentIndex);
private:
    Ui::MainWindow *ui;
    bool isMousePressed;
    QPoint dragPosition;

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);
};
#endif // MAINWINDOW_H
