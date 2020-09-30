#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->panel->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ShowImage(const QImage& image,
                             int scaleWidth = 0,
                             int scaleHeight = 0)
{
    // Set label text to size of the image
    ui->labelImgSize->setText(QString("Size: %1x%2")
                              .arg(image.width())
                              .arg(image.height()));

    QPixmap pixmap = QPixmap::fromImage(image);

    // If either the width or the height is zero or negative,
    // QPixmap::scaled() returns a null pixmap.
    QPixmap scaledPixmap = pixmap.scaled(scaleWidth, scaleHeight);
    ui->labelImage->setPixmap((scaledPixmap.isNull())
                              ? pixmap
                              : scaledPixmap);
}

ImagePyr* MainWindow::GetCurrImagePyr()
{
    // Extracting image pyramid that is stored in data of the file combo box
    QVariant data = ui->comboBoxFile->itemData(ui->comboBoxFile->currentIndex());
    return data.value<ImagePyr*>();
}

void MainWindow::on_actionOpen_file_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this,
        tr("Open"), "", tr("Images (*.png *.jpg)"));

    if (QString::compare(filePath, QString()))
    {
        if (!_files.Contains(filePath.toStdString()))
        {
            ImagePyr imagePyr(filePath);

            if (imagePyr.IsEmpty())
            {
                QMessageBox::warning(this, " ", "Couldn't load the file.");
                return;
            }

            // Adding file to the file container
            auto filePos = _files.AddFile(filePath.toStdString(), imagePyr);

            // Adding its name to the file combo box as item
            // and pointer to the file as data so after selecting an item
            // we can access the corresponding file
            int itemPos = std::distance(_files.Begin(), filePos);
            QString fileName = filePath.section('/', -1, -1);
            QVariant data = QVariant::fromValue((ImagePyr*)&(*filePos));
            ui->comboBoxFile->insertItem(itemPos, fileName, data);
            ui->comboBoxFile->setCurrentIndex(itemPos);

            // This will update the layer combo box
            // and display the opened image
            on_comboBoxFile_activated(itemPos);
        }
        else
        {
            QMessageBox::warning(this, " ", "This file is already opened.");
        }

        ui->panel->setVisible(true);
    }
}

void MainWindow::on_comboBoxLayer_activated(int index)
{
    ImagePyr* imagePyr = GetCurrImagePyr();
    ShowImage(imagePyr->GetLayer(index),
              imagePyr->Width(),
              imagePyr->Height());
}

void MainWindow::on_comboBoxFile_activated(int index)
{
    ImagePyr* imagePyr = GetCurrImagePyr();
    imagePyr->SetSizeDiv(ui->dSpinBoxSizeDiv->value());
    ui->dSpinBoxSizeDiv->setMaximum(std::min(imagePyr->Width(),
                                             imagePyr->Height()));

    // Updating the layer combo box items [0, 1, 2, ..., N)
    QStringList layerList;
    for (int i = 0; i < imagePyr->Count(); ++i)
        layerList << QString::number(i);
    ui->comboBoxLayer->clear();
    ui->comboBoxLayer->addItems(layerList);

    // Displaying selected image
    on_comboBoxLayer_activated(0);
}

void MainWindow::on_dSpinBoxSizeDiv_valueChanged(double sizeDiv)
{
    // This will update the pyramid with new size divosor
    // the layer check box with new items
    // and reset image to first layer
    on_comboBoxFile_activated(ui->comboBoxFile->currentIndex());
}
