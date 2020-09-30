#include "imagepyr.h"
#include <QPainter>
#include <cmath>

ImagePyr::ImagePyr(const QString& filePath)
    : _isEmpty(true)
{
    _image = QImage(filePath);

    if (_image.isNull())
        return;

    _isEmpty = false;

    SetSizeDiv(2.0f);
}

// Passing copy of source image because qt_blurImage changes it
QImage ImagePyr::PyrDown(QImage source)
{
    QSize sourceSize = source.size();

    QImage imageOut(source);
    QPainter painter(&imageOut);
    qt_blurImage(&painter, source, 5, true, false);

    return imageOut.scaled(sourceSize / 2);
}

QImage ImagePyr::GetLayer(int index)
{
    int imgMinSize = std::min(_image.width(), _image.height());
    int numBlurs = index * std::log2(imgMinSize) / _count;

    // First, do required amount of downsamplings with bluring
    QImage layer = _image;
    for (int i = 0; i < numBlurs; ++i)
        layer = ImagePyr::PyrDown(layer);

    // Then simply resize the image to result size
    double totalDiv = pow(_sizeDiv, index);
    return layer.scaled(QSize(_image.width() / totalDiv,
                              _image.height() / totalDiv));
}

void ImagePyr::SetSizeDiv(float sizeDiv)
{
    _sizeDiv = sizeDiv;

    // Calculating the maximum amount of images in the pyramid
    int imgMinSize = std::min(_image.width(), _image.height());
    _count = std::log(imgMinSize) / std::log(_sizeDiv) + 1;
}

bool ImagePyr::IsEmpty()
{
    return _isEmpty;
}

int ImagePyr::Width()
{
    return _image.width();
}

int ImagePyr::Height()
{
    return _image.height();
}

int ImagePyr::Count()
{
    return _count;
}

bool ImagePyr::operator<(const ImagePyr &rhs) const
{
    // No reason for calculating sqrt()
    // since it won't change the result of comparison
    int lDiagonal = _image.width() * _image.width()
            + _image.height() * _image.height();
    int rDiagonal = rhs._image.width() * rhs._image.width()
            + rhs._image.height() * rhs._image.height();

    return lDiagonal < rDiagonal;
}
