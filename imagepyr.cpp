#include "imagepyr.h"

ImagePyr::ImagePyr(std::string filePath)
    : _isEmpty(true)
{
    cv::Mat cvImage = cv::imread(filePath, 3);

    if (cvImage.empty())
        return;

    _isEmpty = false;

    // fixing colors by converting BGR to RGB
    cv::cvtColor(cvImage, cvImage, cv::COLOR_BGR2RGB);

    // Calculating the maximum amount of images in the pyramid
    int imgMinSize = std::min(cvImage.cols, cvImage.rows);
    int pyrHeight = std::log2(imgMinSize);
    if (pyrHeight == 0) // image with either width or height equal to one
        pyrHeight = 1;  // sets pyrHeight to zero what leads to crash

    // Generating the image pyramid
    _layers.resize(pyrHeight);
    _layers[0] = cvImage;
    auto prev = _layers.begin();
    for (auto curr = prev + 1; curr != _layers.end(); ++curr, ++prev)
        cv::pyrDown(*prev, *curr);
}

bool ImagePyr::IsEmpty()
{
    return _isEmpty;
}

cv::Mat& ImagePyr::GetLayer(int index)
{
    return _layers[index];
}

ImagePyr::size_type ImagePyr::Count()
{
    return _layers.size();
}

bool ImagePyr::operator<(const ImagePyr &rhs) const
{
    cv::Mat lLayer0 = _layers[0];
    cv::Mat rLayer0 = rhs._layers[0];

    // No reason for calculating sqrt()
    // since it won't change the result of comparison
    int lDiagonal = lLayer0.cols * lLayer0.cols
            + lLayer0.rows * lLayer0.rows;
    int rDiagonal = rLayer0.cols * rLayer0.cols
            + rLayer0.rows * rLayer0.rows;

    return lDiagonal < rDiagonal;
}
