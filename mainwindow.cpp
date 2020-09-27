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
    ui->horizontalPanel->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ShowCVImage(const cv::Mat& cvImage,
                             int scaleWidth = 0,
                             int scaleHeight = 0)
{
    if (cvImage.empty())
        return;

    // Set label text to size of the image
    ui->labelImgSize->setText(QString("Size: %1x%2")
                              .arg(cvImage.cols)
                              .arg(cvImage.rows));

    // Converting cv::Mat > QImage > QPixmap
    QImage qtImage = QImage((uchar*) cvImage.data, cvImage.cols, cvImage.rows,
                            cvImage.step, QImage::Format_RGB888);
    QPixmap pixmap = QPixmap::fromImage(qtImage);

    // If either the width or the height is zero or negative,
    // QPixmap::scaled() returns a null pixmap.
    QPixmap scaledPixmap = pixmap.scaled(scaleWidth, scaleHeight);
    ui->labelImage->setPixmap((scaledPixmap.isNull())
                              ? pixmap
                              : scaledPixmap);
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

        // fixing colors by converting BGR to RGB
        cv::cvtColor(cvImage, cvImage, cv::COLOR_BGR2RGB);

        // Calculating the maximum amount of images in the pyramid
        int imgMinSize = std::min(cvImage.cols, cvImage.rows);
        int pyrHeight = std::log2(imgMinSize);
        if (pyrHeight == 0) // image with either width or height equal to one
            pyrHeight = 1;  // sets pyrHeight to zero what leads to crash
        _imagePyr.resize(pyrHeight);

        // Generating the image pyramid
        _imagePyr[0] = cvImage;
        auto prev = _imagePyr.begin();
        for (auto curr = prev + 1; curr != _imagePyr.end(); ++curr, ++prev)
            cv::pyrDown(*prev, *curr);

        // Adding items [0, 1, 2, ..., pyrHeight) to the layer combo box
        QStringList layerList;
        for (int i = 0; i < pyrHeight; ++i)
            layerList << QString::number(i);
        ui->comboBoxLayer->clear();
        ui->comboBoxLayer->addItems(layerList);

        ShowCVImage(cvImage);

        ui->horizontalPanel->setVisible(true);
    }
}

void MainWindow::on_comboBoxLayer_activated(int index)
{
    ShowCVImage(_imagePyr[index], _imagePyr[0].cols, _imagePyr[0].rows);
}
