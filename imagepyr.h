#ifndef IMAGEPYR_H
#define IMAGEPYR_H

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

class ImagePyr
{
public:
    typedef std::vector<cv::Mat>::size_type size_type;

    ImagePyr(std::string filePath);
    bool IsEmpty();
    cv::Mat& GetLayer(int index);
    size_type Count();
    bool operator<(const ImagePyr &rhs) const;

private:
    std::vector<cv::Mat> _layers;
    bool _isEmpty;
};

#endif // IMAGEPYR_H
