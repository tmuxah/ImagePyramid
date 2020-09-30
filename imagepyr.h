#ifndef IMAGEPYR_H
#define IMAGEPYR_H

#include <QWidget>

QT_BEGIN_NAMESPACE
    extern Q_WIDGETS_EXPORT void qt_blurImage(QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0);
QT_END_NAMESPACE

class ImagePyr
{
public:
    static QImage PyrDown(QImage source);

public:
    ImagePyr(const QString& filePath);
    QImage GetLayer(int index);
    bool IsEmpty();
    int Width();
    int Height();
    int Count();
    bool operator<(const ImagePyr &rhs) const;

private:
    int _count;
    QImage _image;
    bool _isEmpty;
};

#endif // IMAGEPYR_H
