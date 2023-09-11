#ifndef COEFSTRATEGY_H
#define COEFSTRATEGY_H

#include <QVector>

class CoefStrategy
{
public:
    virtual ~CoefStrategy() = default;
    virtual std::tuple<QVector<double>, QVector<double>> coefCalc(int sample_freq, int cutoff_freq) = 0;
};

class Context {
private:
    CoefStrategy *strategy;

public:
    Context(CoefStrategy *strategy);
    std::tuple<QVector<double>, QVector<double>> coefCalc(int sample_freq, int cutoff_freq);
};

class lowpass : public CoefStrategy {
public:
    std::tuple<QVector<double>, QVector<double>> coefCalc(int sample_freq, int cutoff_freq) override;
};

class highpass : public CoefStrategy {
public:
    std::tuple<QVector<double>, QVector<double>> coefCalc(int sample_freq, int cutoff_freq) override;
};

class bandpass : public CoefStrategy {
public:
    std::tuple<QVector<double>, QVector<double>> coefCalc(int sample_freq, int cutoff_freq) override;
};

class bandstop : public CoefStrategy {
public:
    std::tuple<QVector<double>, QVector<double>> coefCalc(int sample_freq, int cutoff_freq) override;
};

#endif // COEFSTRATEGY_H
