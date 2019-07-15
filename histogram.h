#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QWidget>
#include <QPainter>
#include <QImage>

class Histogram : public QWidget
{
    Q_OBJECT

public:
    Histogram(QImage image, QWidget *parent = nullptr);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void calcHist();

    QImage image;
    QVector<QPoint> red;
    QVector<QPoint> green;
    QVector<QPoint> blue;
};

#endif // HISTOGRAM_H
