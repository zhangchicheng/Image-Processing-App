#include "imgproc.h"

namespace ImgProc
{

QImage::Format adjustFormat(QImage::Format formatHint)
{
    QImage::Format format;
    switch (formatHint)
    {
    case QImage::Format_Mono:
    case QImage::Format_MonoLSB:
    case QImage::Format_Indexed8:
        format = QImage::Format_Indexed8;
        break;
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
    case QImage::Format_RGBX8888:
    case QImage::Format_RGBA8888:
    case QImage::Format_RGBA8888_Premultiplied:
    case QImage::Format_Alpha8:
    case QImage::Format_Grayscale8:
        format = formatHint;
        break;
    case QImage::Format_RGB16:
        format = QImage::Format_RGB32;
        break;
    case QImage::Format_RGB444:
    case QImage::Format_RGB555:
    case QImage::Format_RGB666:
    case QImage::Format_RGB888:
        format = QImage::Format_RGB888;
        break;
    case QImage::Format_ARGB4444_Premultiplied:
    case QImage::Format_ARGB6666_Premultiplied:
    case QImage::Format_ARGB8555_Premultiplied:
    case QImage::Format_ARGB8565_Premultiplied:
        format = QImage::Format_ARGB32_Premultiplied;
        break;
    default:
        format = QImage::Format_ARGB32;
        break;
    }
    return format;
}

cv::Mat QImage2CvMat(const QImage &img)
{
    if (img.isNull())
        return cv::Mat();

    QImage::Format format = adjustFormat(img.format());
    QImage image = (format==img.format()) ? img : img.convertToFormat(format);

    cv::Mat mat = cv::Mat(image.height(), image.width(), CV_8UC(image.depth()/8), const_cast<uchar*>(image.constBits()), static_cast<size_t>(image.bytesPerLine()));
    return mat;
}

QImage CvMat2QImage(const cv::Mat &mat)
{
    if (mat.empty())
        return QImage();

    QImage::Format format;
    switch (mat.type())
    {
    case CV_8UC1:
        format = QImage::Format_Indexed8;
        break;
    case CV_8UC3:
        format = QImage::Format_RGB32;
        break;
    case CV_8UC4:
        format = QImage::Format_ARGB32;
        break;
    default:
        format = QImage::Format_ARGB32;
        break;
    }
    QImage img(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), format);
    return img;
}

void threshold(QImage &srcImg, QImage &dstImg, int thresh, QString type)
{
    cv::Mat srcMat, grayMat, dstMat;
    srcMat = QImage2CvMat(srcImg);
    switch (srcMat.type())
    {
    case CV_8UC3:
        cv::cvtColor(srcMat, grayMat, cv::COLOR_BGR2GRAY);
        break;
    case CV_8UC4:
        cv::cvtColor(srcMat, grayMat, cv::COLOR_BGRA2GRAY);
        break;
    default:
        break;
    }
    int threshType = type=="Binary" ? cv::THRESH_BINARY :
                     type=="Binary_Inverted" ? cv::THRESH_BINARY_INV :
                     type=="Truncate" ? cv::THRESH_TRUNC :
                     type=="To Zero" ? cv::THRESH_TOZERO :
                     cv::THRESH_TOZERO_INV;

    cv::threshold(srcMat, dstMat, thresh, 255, threshType);
    dstImg = CvMat2QImage(dstMat);
}

}
