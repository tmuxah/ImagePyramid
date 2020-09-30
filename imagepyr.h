#ifndef IMAGEPYR_H
#define IMAGEPYR_H

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

class ImagePyr
{
public:
    ImagePyr(std::string filePath);
    bool IsEmpty();
    cv::Mat GetLayer(int index);
    int Width();
    int Height();
    int Count();
    bool operator<(const ImagePyr &rhs) const;

private:
    int _count;
    cv::Mat _image;
    bool _isEmpty;
};

#endif // IMAGEPYR_H
