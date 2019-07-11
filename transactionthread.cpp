#include "transactionthread.h"
#include "imgproc.h"

Transaction * const EndTransaction = nullptr;

QImage ThresholdTransaction::apply()
{
    return ImgProc::threshold(image, thresh, maxval, type);
}

QString ThresholdTransaction::message()
{
    return QObject::tr("Thresholding image...");
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
