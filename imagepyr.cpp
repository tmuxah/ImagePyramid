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

    // Calculating the maximum amount of images in the pyramid
    int imgMinSize = std::min(_image.width(), _image.height());
    _count = std::log2(imgMinSize);
    if (_count == 0) // image with either width or height equal to one
        _count = 1;  // sets pyrHeight to zero what leads to crash
}

// Passing copy of source image because qt_blurImage changes it
QImage ImagePyr::PyrDown(QImage source)
{
    QSize sourceSize = source.size();
    QImage imageOut(source);
    QPainter painter( &imageOut );
    qt_blurImage( &painter, source, 5, true, false );

    return imageOut.scaled(sourceSize / 2);
}

bool ImagePyr::IsEmpty()
{
    return _isEmpty;
}

QImage ImagePyr::GetLayer(int index)
{
    QImage layer = _image;
    for (int i = 0; i < index; ++i)
        layer = ImagePyr::PyrDown(layer);

    return layer;
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
