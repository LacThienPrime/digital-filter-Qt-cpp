#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "coefstrategy.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->myplot->xAxis->setLabel("Time");
    ui->myplot->yAxis->setLabel("Amplitude");
    ui->myplot->xAxis->setRange(0, 1);
    ui->myplot->yAxis->setRange(-3, 3);

    ui->DFTplot->xAxis->setLabel("Frequency");
    ui->DFTplot->yAxis->setLabel("Amplitude");
    ui->DFTplot->xAxis->setRange(0, 100);
    ui->DFTplot->yAxis->setRange(0, 0.5);

    thread = new QThread();
    filterThread = new FilterThread();

    filterThread->moveToThread(thread);

    thread->start();

    // pass sample freq and a, b arguments
    connect(this, &MainWindow::get, filterThread, &FilterThread::test_signal);

    // plot test signal
    connect(filterThread, &FilterThread::generated, this, &MainWindow::signalPlot);

    // plot filtered signal
    connect(filterThread, &FilterThread::finishedFilter, this, &MainWindow::filterPlot);

    // DFT calculation
    connect(filterThread, &FilterThread::generated, this, &MainWindow::DFTsignal);
    connect(filterThread, &FilterThread::finishedFilter, this, &MainWindow::DFTfilter);

    // DFT plot
    connect(this, &MainWindow::getDFTsignal, this, &MainWindow::signalDFT);
    connect(this, &MainWindow::getDFTfilter, this, &MainWindow::filterDFT);
}

MainWindow::~MainWindow()
{
    thread->quit();

    delete ui;
}

void MainWindow::signalPlot(QVector<double> t, QVector<double> y)
{
    ui->myplot->addGraph();
    ui->myplot->graph(0)->setPen(QPen(Qt::blue));
    ui->myplot->graph(0)->setData(t, y);
    ui->myplot->replot();
}

void MainWindow::filterPlot(QVector<double> tt, QVector<double> yy_filtered)
{
    ui->myplot->addGraph();
    ui->myplot->graph(1)->setPen(QPen(Qt::red));
    ui->myplot->graph(1)->setData(tt, yy_filtered);
    ui->myplot->replot();
}

void MainWindow::signalDFT(QVector<double> t, QVector<double> y)
{
    ui->DFTplot->addGraph();
    ui->DFTplot->graph(0)->setPen(QPen(Qt::blue));
    ui->DFTplot->graph(0)->setData(t, y);
    ui->DFTplot->replot();
}

void MainWindow::filterDFT(QVector<double> tt, QVector<double> yy_filtered)
{
    ui->DFTplot->addGraph();
    ui->DFTplot->graph(1)->setPen(QPen(Qt::red));
    ui->DFTplot->graph(1)->setData(tt, yy_filtered);
    ui->DFTplot->replot();
}

void MainWindow::DFTsignal(QVector<double> time, QVector<double> signal)
{
    int N = signal.size();
    int K = N;

    std::complex<double> intSum;

    std::vector<std::complex<double>> output;
    output.reserve(K);

    for (int k = 0; k < K; k++)
    {
        intSum = std::complex<double>(0.0, 0.0);

        for (int n = 0; n < N; n++)
        {
            double realPart = qCos((2 * M_PI / N) * k * n);
            double imagPart = qSin((2 * M_PI / N) * k * n);

            std::complex<double> w(realPart, -imagPart);

            intSum += signal[n] * w;
        }

        output.push_back(intSum);
    }

    int j = time.size();

    QVector<double> xx;
    QVector<double> yy;

    for (int n = 0; n < j; n++)
    {
        xx.push_back(n);
    }

    for (auto &ii : output)
    {
        ii = ii / static_cast<double>(j);
        yy.push_back(std::abs(ii));
    }

    emit getDFTsignal(xx, yy);
}

void MainWindow::DFTfilter(QVector<double> time, QVector<double> signal)
{
    int N = signal.size();
    int K = N;

    std::complex<double> intSum;

    std::vector<std::complex<double>> output;
    output.reserve(K);

    for (int k = 0; k < K; k++)
    {
        intSum = std::complex<double>(0.0, 0.0);

        for (int n = 0; n < N; n++)
        {
            double realPart = qCos((2 * M_PI / N) * k * n);
            double imagPart = qSin((2 * M_PI / N) * k * n);

            std::complex<double> w(realPart, -imagPart);

            intSum += signal[n] * w;
        }

        output.push_back(intSum);
    }

    int j = time.size();

    QVector<double> xx;
    QVector<double> yy;

    for (int n = 0; n < j; n++)
    {
        xx.push_back(n);
    }

    for (auto &ii : output)
    {
        ii = ii / static_cast<double>(j);
        yy.push_back(std::abs(ii));
    }

    emit getDFTfilter(xx, yy);
}

void MainWindow::on_pushButton_clicked()
{
    QString text = ui->lineEdit->text();
    int sampleFreq = text.toInt();

    QString text_2 = ui->lineEdit_2->text();
    int passFreq = text_2.toInt();

    QString text_3 = ui->lineEdit_3->text();
    int stopFreq = text_3.toInt();

    QVector<double> a, b;

    if(ui->radioButton_5->isChecked())
    {
        Context context(new lowpass());
        auto coefficients = context.coefCalc(sampleFreq, passFreq);

        a = std::get<0>(coefficients);
        b = std::get<1>(coefficients);
    }

    if(ui->radioButton_6->isChecked())
    {
        Context context(new highpass());
        auto coefficients = context.coefCalc(sampleFreq, passFreq);

        a = std::get<0>(coefficients);
        b = std::get<1>(coefficients);
    }

    if(ui->radioButton_7->isChecked())
    {
        Context context(new bandpass());
        auto coefficients = context.coefCalc(sampleFreq, passFreq);

        a = std::get<0>(coefficients);
        b = std::get<1>(coefficients);
    }

    if(ui->radioButton_8->isChecked())
    {
        Context context(new bandstop());
        auto coefficients = context.coefCalc(sampleFreq, stopFreq);

        a = std::get<0>(coefficients);
        b = std::get<1>(coefficients);
    }

    emit get(sampleFreq, a, b);
}
