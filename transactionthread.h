#ifndef TRANSACTIONTHREAD_H
#define TRANSACTIONTHREAD_H

#include <QImage>
#include <QMutex>
#include <QQueue>
#include <QThread>
#include <QWaitCondition>

class Transaction //'Transaction' has no out-of-line virtual method definition; its vtable will be emitted in every translation unit.
{
protected:
    Transaction(QImage img) : image(img) {}
    QImage image;
public:

    virtual ~Transaction() {}

    virtual QImage apply() = 0;
    virtual QString message() = 0;
};

class BlurTransaction : public Transaction
{
public:
    BlurTransaction(QImage img, int w, int h) :
        Transaction(img), width(w), height(h) {}
    ~BlurTransaction() {}

    QImage apply();
    QString message();

private:
    int width;
    int height;
};

class MedianBlurTransaction : public Transaction
{
public:
    MedianBlurTransaction(QImage img, int s) :
        Transaction(img), size(s) {}
    ~MedianBlurTransaction() {}

    QImage apply();
    QString message();

private:
    int size;
};

class GaussianBlurTransaction : public Transaction
{
public:
    GaussianBlurTransaction(QImage img, int w, int h, double x, double y) :
        Transaction(img), width(w), height(h), sigmaX(x), sigmaY(y) {}
    ~GaussianBlurTransaction() {}

    QImage apply();
    QString message();

private:
    int width;
    int height;
    double sigmaX;
    double sigmaY;
};

class BilateralFilterTransaction : public Transaction
{
public:
    BilateralFilterTransaction(QImage img, int d, double c, double s) :
        Transaction(img), diam(d), color(c), space(s) {}
    ~BilateralFilterTransaction() {}

    QImage apply();
    QString message();

private:
    int diam;
    double color;
    double space;
};

class ThresholdTransaction : public Transaction
{
public:
    ThresholdTransaction(QImage img, int th, int maxv, QString ty) :
        Transaction(img), thresh(th), maxval(maxv), type(ty) {}
    ~ThresholdTransaction() {}

    QImage apply();
    QString message();

private:
    int thresh;
    int maxval;
    QString type;
};

class EqualizeHistTransaction : public Transaction
{
public:
    EqualizeHistTransaction(QImage img) : Transaction(img) {}
    ~EqualizeHistTransaction() {}

    QImage apply();
    QString message();
};

class ErodeTransaction : public Transaction
{
public:
    ErodeTransaction(QImage img, QString k, int s) :
        Transaction(img), kernel(k), size(s) {}
    ~ErodeTransaction() {}

    QImage apply();
    QString message();

private:
    QString kernel;
    int size;
};

class DilateTransaction : public Transaction
{
public:
    DilateTransaction(QImage img, QString k, int s) :
        Transaction(img), kernel(k), size(s) {}
    ~DilateTransaction() {}

    QImage apply();
    QString message();

private:
    QString kernel;
    int size;
};

class TransactionThread : public QThread
{
    Q_OBJECT

public:
    TransactionThread();
    ~TransactionThread();

    void addTransaction(Transaction *transact);
    QImage image();

signals:
    void transactionStarted(const QString &message);
    void allTransactionDone();

protected:
    void run();

private:
    QImage currentImage;
    QQueue<Transaction *> transactions;
    QWaitCondition transactionAdded;
    QMutex mutex;
};

#endif // TRANSACTIONTHREAD_H
