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
    double calculateBaseline();
    void startSession();
    void endSession();
    bool toggleContact();
    bool getInContact() const;
    bool getInSession() const; // note: do NOT make a setter for inSession (this will mess up the sequence). use `startSession()` and `endSession()` instead.
    void pauseTreatment();
    void continueTreatment();

signals:
    void administerFeedback();
    void sessionCompleted(double startingBaseline, double endingBaseline);

private:
    Electrode* electrodes[NUM_ELECTRODES];

    // Qt
    QCustomPlot *m_customPlot;
    QTimer *m_eegUpdateTimer;
    QTimer *observationTimer;
    QTimer *feedbackTimer;

    // Data
    double m_baselineFrequencies[NUM_ELECTRODES];
    double startingBaseline;

    // State
    bool inContact = false;
    bool inSession = false;
    bool isFeedback = false;
    bool isPaused = false;
    int m_currentElectrodeIndex;
    int therapyRound = 0; // limit: NUM_ROUNDS in defs.h

    // Helper functions
    void setupEEGPlot();
    void updateEEGPlot();
    void beginFeedback();
    void endFeedback(); // automatically called by beginFeedback
    double generateEEGData(double currentTime, Electrode *electrode, double offset, double amplitudeFactor = 1);

    bool waiting;

signals:

};

#endif // EEGSIMULATOR_H
