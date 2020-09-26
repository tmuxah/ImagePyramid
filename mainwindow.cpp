#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <opencv2/imgproc.hpp>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_file_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open"), "", tr("Images (*.png *.jpg)"));

    if (QString::compare(fileName, QString()))
    {
        cv::Mat cvImage = cv::imread(fileName.toLocal8Bit().data(), 3);

        if (cvImage.empty())
        {
            QMessageBox::warning(this, "Warning", "Couldn't load the image.");
            return;
        }

        // Calculating the maximum amount of images in the pyramid
        int imgMinSize = std::min(cvImage.cols, cvImage.rows);
        int pyrHeight = std::log2(imgMinSize);
        _imagePyr.resize(pyrHeight);

        // Generating the image pyramid
        _imagePyr[0] = cvImage;
        auto prev = _imagePyr.begin();
        for (auto curr = prev + 1; curr != _imagePyr.end(); ++curr, ++prev)
            cv::pyrDown(*prev, *curr, cv::Size(prev->cols / 2, prev->rows / 2));

        // Converting cv::Mat > QImage > QPixmap
        // and assigning it to the image label
        cv::cvtColor(cvImage, cvImage, cv::COLOR_BGR2RGB); // fixing colors by converting BGR to RGB
        QImage qtImage = QImage((uchar*) cvImage.data, cvImage.cols, cvImage.rows,
                                cvImage.step, QImage::Format_RGB888);
        ui->labelImage->setPixmap(QPixmap::fromImage(qtImage));
    }
}
