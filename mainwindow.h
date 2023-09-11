#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <complex>

#include "filterthread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void DFTsignal(QVector<double> time, QVector<double> signal);
    void DFTfilter(QVector<double> time, QVector<double> filter);

private slots:
    void signalPlot(QVector<double> t, QVector<double> y);
    void filterPlot(QVector<double> tt, QVector<double> yy_filtered);

    void signalDFT(QVector<double> t, QVector<double> y);
    void filterDFT(QVector<double> tt, QVector<double> yy_filtered);

    void on_pushButton_clicked();

signals:
    void get(int sample, QVector<double> a, QVector<double> b);
    void getDFTsignal(QVector<double> x, QVector<double> y);
    void getDFTfilter(QVector<double> xx, QVector<double> yy);

private:
    Ui::MainWindow *ui;

    QThread *thread;
    FilterThread *filterThread;
};

#endif // MAINWINDOW_H
