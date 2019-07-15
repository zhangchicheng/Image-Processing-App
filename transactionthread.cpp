#include "transactionthread.h"
#include "imgproc.h"

Transaction * const EndTransaction = nullptr;

QImage BlurTransaction::apply()
{
    return ImgProc::blur(image, width, height);
}

QString BlurTransaction::message()
{
    return QObject::tr("Blurring image...");
}

QImage MedianBlurTransaction::apply()
{
    return ImgProc::medianBlur(image, size);
}

QString MedianBlurTransaction::message()
{
    return QObject::tr("Median Blurring image...");
}

QImage GaussianBlurTransaction::apply()
{
    return ImgProc::GaussianBlur(image, width, height, sigmaX, sigmaY);
}

QString GaussianBlurTransaction::message()
{
    return QObject::tr("Gaussian Blurring image...");
}

QImage BilateralFilterTransaction::apply()
{
    return ImgProc::bilateralFilter(image, diam, color, space);
}

QString BilateralFilterTransaction::message()
{
    return QObject::tr("Median Blurring image...");
}

QImage ThresholdTransaction::apply()
{
    return ImgProc::threshold(image, thresh, maxval, type);
}

QString ThresholdTransaction::message()
{
    return QObject::tr("Thresholding image...");
}

QImage EqualizeHistTransaction::apply()
{
    return ImgProc::equalizeHist(image);
}

QString EqualizeHistTransaction::message()
{
    return QObject::tr("Equalizing image histogram... ");
}

QImage ErodeTransaction::apply()
{
    return ImgProc::erode(image, kernel, size);
}

QString ErodeTransaction::message()
{
    return QObject::tr("Eroding image...");
}

QImage DilateTransaction::apply()
{
    return ImgProc::dilate(image, kernel, size);
}

QString DilateTransaction::message()
{
    return QObject::tr("Dilating image...");
}

TransactionThread::TransactionThread()
{
    start();
}

TransactionThread::~TransactionThread()
{
    {
        QMutexLocker locker(&mutex);
        while (!transactions.isEmpty())
            delete transactions.dequeue();
        transactions.enqueue(EndTransaction);
        transactionAdded.wakeOne();
    }

    wait();
}

void TransactionThread::addTransaction(Transaction *transact)
{
    QMutexLocker locker(&mutex);
    transactions.enqueue(transact);
    transactionAdded.wakeOne();
}

void TransactionThread::run()
{
    Transaction *transact = nullptr;

    forever
    {
        {
            QMutexLocker locker(&mutex);

            if (transactions.isEmpty())
                transactionAdded.wait(&mutex);
            transact = transactions.dequeue();
            if (transact == EndTransaction)
                break;
        }

        emit transactionStarted(transact->message());
        QImage newImage = transact->apply();
        delete transact;

        {
            QMutexLocker locker(&mutex);
            currentImage = newImage;
            if (transactions.isEmpty())
                emit allTransactionDone();
        }
    }
}

QImage TransactionThread::image()
{
    QMutexLocker locker(&mutex);
    return currentImage;
}
