#include "histogram.h"
#include<QDebug>

Histogram::Histogram(QImage image, QWidget *parent) : QWidget (parent)
{
    this->image = image;
    setFixedSize(200,64);
}

void Histogram::calcHist()
{
    QVector<int> r(256,0), g(256,0), b(256,0);
    for (int i=0; i<image.width(); i++)
    {
        for (int j=0; j<image.height(); j++)
        {
            QColor p = image.pixel(i,j);
            r[p.red()]++;
            g[p.green()]++;
            b[p.blue()]++;
        }
    }
    int maxV = 0;
    for (int i=0; i<256; i++)
    {
        if (r[i]>maxV) maxV = r[i];
        if (g[i]>maxV) maxV = g[i];
        if (b[i]>maxV) maxV = b[i];
    }
    for (int i=0; i<255; i++)
    {
        double unitW = 190.0/256.0;
        double unitH = 54.0/maxV;
        red.push_back(QPoint(static_cast<int>(i*unitW)+5, static_cast<int>(59-r[i]*unitH)));
        red.push_back(QPoint(static_cast<int>((i+1)*unitW)+5, static_cast<int>(59-r[i+1]*unitH)));
        green.push_back(QPoint(static_cast<int>(i*unitW)+5, static_cast<int>(59-g[i]*unitH)));
        green.push_back(QPoint(static_cast<int>((i+1)*unitW)+5, static_cast<int>(59-g[i+1]*unitH)));
        blue.push_back(QPoint(static_cast<int>(i*unitW)+5, static_cast<int>(59-b[i]*unitH)));
        blue.push_back(QPoint(static_cast<int>((i+1)*unitW)+5, static_cast<int>(59-b[i+1]*unitH)));
    }
}

void Histogram::paintEvent(QPaintEvent *)
{
    calcHist();
    QPainter painter(this);

    painter.fillRect(QRect(0,0,200,64), "#36393e");
    painter.fillRect(QRect(4,4,191,55), "#535c68");

    painter.setPen(QPen(QColor("#FFFE4365"), 2, Qt::SolidLine));
    painter.drawLines(red);
    painter.setPen(QPen(QColor("#FF32CD32"), 2, Qt::SolidLine));
    painter.drawLines(green);
    painter.setPen(QPen(QColor("#FF005AAB"), 2, Qt::SolidLine));
    painter.drawLines(blue);
}
