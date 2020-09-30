#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/imgcodecs.hpp>
#include "imagepyr.h"
#include "filecontainer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

Q_DECLARE_METATYPE(ImagePyr*)

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_file_triggered();
    void on_comboBoxLayer_activated(int index);
    void on_comboBoxFile_activated(int index);

private:
    Ui::MainWindow *ui;
    FileContainer<ImagePyr> _files;

    void ShowCVImage(const cv::Mat& cvImage,
                     int scaleWidth,
                     int scaleHeight);
};
#endif // MAINWINDOW_H
