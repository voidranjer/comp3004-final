#include "electrode.h"
#include <QRandomGenerator>

Electrode::Electrode(QObject *parent, QColor color, double delta, double theta, double alpha, double beta, double gamma)
    : QObject{parent}, color(color), delta(delta), theta(theta), alpha(alpha), beta(beta), gamma(gamma)
{
    deltaAmplitude = QRandomGenerator::global()->bounded(AMPLITUDE_MIN, AMPLITUDE_MAX);
    thetaAmplitude = QRandomGenerator::global()->bounded(AMPLITUDE_MIN, AMPLITUDE_MAX);
    alphaAmplitude = QRandomGenerator::global()->bounded(AMPLITUDE_MIN, AMPLITUDE_MAX);
    betaAmplitude = QRandomGenerator::global()->bounded(AMPLITUDE_MIN, AMPLITUDE_MAX);
    gammaAmplitude = QRandomGenerator::global()->bounded(AMPLITUDE_MIN, AMPLITUDE_MAX);
}

void Electrode::setBaseline(double baseline)
{
    this->baseline = baseline;
}

QColor Electrode::getColor() { return color; }

double Electrode::getFreqSum() {
    double noiseFactor = QRandomGenerator::global()->generateDouble() * NOISE_FACTOR + 0.1;
    return (delta + theta + alpha + beta + gamma) * noiseFactor;
}

double Electrode::getAmplitudeSum() {
    double noiseFactor = QRandomGenerator::global()->generateDouble() * NOISE_FACTOR + 0.1;
    return (deltaAmplitude + thetaAmplitude + alphaAmplitude + betaAmplitude + gammaAmplitude) * noiseFactor;
}

double Electrode::getDominantFrequency()
{

    double totalAmplitude = getAmplitudeSum();

    double dominantFrequency = ((delta * deltaAmplitude) + (beta * betaAmplitude) +
                                (alpha * alphaAmplitude) + (gamma * gammaAmplitude) +
                                (theta) * thetaAmplitude) / totalAmplitude;

    return dominantFrequency;
}
