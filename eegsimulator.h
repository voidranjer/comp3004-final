#ifndef EEGSIMULATOR_H
#define EEGSIMULATOR_H

#include <QObject>
#include <QTimer>
#include <QDateTime>
#include <QRandomGenerator>
#include "qcustomplot.h"

class EEGSimulator : public QObject
{
    Q_OBJECT

public:
    explicit EEGSimulator(QCustomPlot *plotWidget, int numElectrodes, QObject *parent = nullptr);
    void selectElectrode(int electrodeIndex);
    ~EEGSimulator();
    double calculateDominantFrequency(double currentTime, int electrodeIndex);
    void calculateBaseline();
    void startTreatment();

private:
    QCustomPlot *m_customPlot;
    QTimer *m_eegUpdateTimer;
    int m_numElectrodes = 0;
    QVector<double> m_baselineFrequencies;

    QVector<double> deltas;
    QVector<double> thetas;
    QVector<double> alphas;
    QVector<double> betas;
    QVector<double> gammas;

    QVector<double> deltaAmplitudes;
    QVector<double> thetaAmplitudes;
    QVector<double> alphaAmplitudes;
    QVector<double> betaAmplitudes;
    QVector<double> gammaAmplitudes;

    int m_currentElectrodeIndex;

    void setupEEGPlot();
    void updateEEGPlot();

    double generateEEGData(double currentTime, int electrodeIndex, double offset);

signals:

};

#endif // EEGSIMULATOR_H
