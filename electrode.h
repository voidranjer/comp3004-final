#ifndef ELECTRODE_H
#define ELECTRODE_H

#include <QObject>
#include <QColor>

class Electrode : public QObject
{
    Q_OBJECT
public:
    explicit Electrode(QObject *parent, QColor color, double delta, double theta, double alpha, double beta, double gamma);
    void setBaseline(double baseline);

    // Getters
    QColor getColor();
    double getFreqSum();
    double getAmplitudeSum();
    double getDominantFrequency();

    // Used in constructor to generate random amplitudes
    const int AMPLITUDE_MIN = 10;
    const int AMPLITUDE_MAX = 100;
private:
    // others
    QColor color;

    // freqs
    double delta;
    double theta;
    double alpha;
    double beta;
    double gamma;
    double baseline;

    // amplitudes
    double deltaAmplitude;
    double thetaAmplitude;
    double alphaAmplitude;
    double betaAmplitude;
    double gammaAmplitude;

signals:
};

#endif // ELECTRODE_H
