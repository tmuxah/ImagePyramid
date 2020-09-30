#include "imagepyr.h"

ImagePyr::ImagePyr(std::string filePath)
    : _isEmpty(true)
{
    _image = cv::imread(filePath, 3);

    if (_image.empty())
        return;

    _isEmpty = false;

    // fixing colors by converting BGR to RGB
    cv::cvtColor(_image, _image, cv::COLOR_BGR2RGB);

    // Calculating the maximum amount of images in the pyramid
    int imgMinSize = std::min(_image.cols, _image.rows);
    _count = std::log2(imgMinSize);
    if (_count == 0) // image with either width or height equal to one
        _count = 1;  // sets pyrHeight to zero what leads to crash
}

bool ImagePyr::IsEmpty()
{
    return _isEmpty;
}

cv::Mat ImagePyr::GetLayer(int index)
{
    cv::Mat layer = _image;
    for (int i = 0; i < index; ++i)
        cv::pyrDown(layer, layer);

    return layer;
}

int ImagePyr::Width()
{
    return _image.cols;
}

int ImagePyr::Height()
{
    return _image.rows;
}

int ImagePyr::Count()
{
    return _count;
}

bool ImagePyr::operator<(const ImagePyr &rhs) const
{
    // No reason for calculating sqrt()
    // since it won't change the result of comparison
    int lDiagonal = _image.cols * _image.cols
            + _image.rows * _image.rows;
    int rDiagonal = rhs._image.cols * rhs._image.cols
            + rhs._image.rows * rhs._image.rows;

    return lDiagonal < rDiagonal;
}
