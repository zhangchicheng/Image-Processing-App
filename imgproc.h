#ifndef IMGPROC_H
#define IMGPROC_H

#include <QImage>
#include <QDebug>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"

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
    case QImage::Format_ARGB32:
    case QImage::Format_RGBX8888:
    case QImage::Format_RGBA8888:
    case QImage::Format_RGBA8888_Premultiplied:
    case QImage::Format_Alpha8:
    case QImage::Format_Grayscale8:
        format = formatHint;
        break;
    case QImage::Format_RGB16:
    case QImage::Format_RGB32:
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
    case QImage::Format_ARGB32_Premultiplied:
        format = QImage::Format_ARGB32_Premultiplied;
        break;
    default:
        format = QImage::Format_ARGB32;
        break;
    }
    return format;
}

cv::Mat QImage2CvMat(const QImage &img, bool clone=false, bool rgbSwap=false)
{
    if (img.isNull())
        return cv::Mat();

    cv::Mat mat;
    switch(img.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(img.height(), img.width(), CV_8UC4, const_cast<uchar*>(img.constBits()), static_cast<size_t>(img.bytesPerLine()));
        if(clone) mat = mat.clone();
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(img.height(), img.width(), CV_8UC3, const_cast<uchar*>(img.constBits()), static_cast<size_t>(img.bytesPerLine()));
        if(clone) mat = mat.clone();
        if(rgbSwap) cv::cvtColor(mat, mat, CV_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
    case QImage::Format_Grayscale8:
        mat = cv::Mat(img.height(), img.width(), CV_8UC1, const_cast<uchar*>(img.constBits()), static_cast<size_t>(img.bytesPerLine()));
        if(clone) mat = mat.clone();
        break;
    default:
        break;
    }
    return mat;
}

QImage CvMat2QImage(const cv::Mat &mat, bool clone=true, bool rgbSwap=false)
{
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_Grayscale8);
        if(clone) return image.copy();
        return image;
    }
    else if(mat.type() == CV_8UC3)
    {
        QImage image(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_RGB888);
        if(clone)
        {
            if(rgbSwap) return image.rgbSwapped();
            return image.copy();
        }
        else
        {
            if(rgbSwap)
            {
                cv::cvtColor(mat, mat, CV_BGR2RGB);
            }
            return image;
        }

    }
    else if(mat.type() == CV_8UC4)
    {
        QImage image(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_ARGB32);
        if(clone) return image.copy();
        return image;
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}

void threshold(QImage &srcImg, QImage &dstImg, int thresh, int maxVal, QString type)
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

    cv::threshold(grayMat, dstMat, thresh, maxVal, threshType);
    dstImg = CvMat2QImage(dstMat);
}


void testConversion(QImage &srcImg, QImage &dstImg)
{
    cv::Mat srcMat = QImage2CvMat(srcImg);
    dstImg = CvMat2QImage(srcMat);
}

}

#endif // IMGPROC_H
