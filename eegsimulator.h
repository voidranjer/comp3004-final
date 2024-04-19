#ifndef EEGSIMULATOR_H
#define EEGSIMULATOR_H

#include <QObject>
#include <QTimer>
#include <QDateTime>
#include <QRandomGenerator>
#include "qcustomplot.h"
#include "electrode.h"
#include "defs.h"

class EEGSimulator : public QObject
{
    Q_OBJECT

public:
    explicit EEGSimulator(QObject *parent, QCustomPlot *plotWidget);
    ~EEGSimulator();
    void selectElectrode(int electrodeIndex);
    void calculateBaseline();
    void startSession();
    void endSession();
    bool toggleContact();
    bool getInContact() const;
    bool getInSession() const; // note: do NOT make a setter for inSession (this will mess up the sequence). use `startSession()` and `endSession()` instead.

private:
    Electrode* electrodes[NUM_ELECTRODES];

    QCustomPlot *m_customPlot;
    QTimer *m_eegUpdateTimer;

    double m_baselineFrequencies[NUM_ELECTRODES];
    int m_currentElectrodeIndex;
    bool inContact = false;
    bool inSession = false;

    void setupEEGPlot();
    void updateEEGPlot();

    double generateEEGData(double currentTime, Electrode *electrode, double offset);

signals:

};

#endif // EEGSIMULATOR_H
