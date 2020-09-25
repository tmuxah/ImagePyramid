#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

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
        QImage image;

        if (image.load(fileName))
        {
            ui->labelImage->setPixmap(QPixmap::fromImage(image));
        }
        else
        {
            QMessageBox::warning(this, "Warning", "Couldn't load the image.");
        }
    }
}
