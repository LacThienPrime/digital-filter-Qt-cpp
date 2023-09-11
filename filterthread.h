#ifndef FILTERTHREAD_H
#define FILTERTHREAD_H

#include <QObject>
#include <QThread>

class FilterThread : public QObject
{
    Q_OBJECT

public:
    FilterThread();

    void test_signal(int sample, QVector<double> a, QVector<double> b);

signals:
    void generated(QVector<double> t, QVector<double> y);
    void finishedFilter(QVector<double> tt, QVector<double> y_filtered);
};

#endif // FILTERTHREAD_H
