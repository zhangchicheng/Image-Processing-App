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
