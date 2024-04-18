#include <eegsimulator.h>

EEGSimulator::EEGSimulator(QCustomPlot *plotWidget, int numElectrodes, QObject *parent)
    : QObject(parent), m_customPlot(plotWidget), m_numElectrodes(numElectrodes)
{
    setupEEGPlot();

    deltas.resize(numElectrodes);
    thetas.resize(numElectrodes);
    alphas.resize(numElectrodes);
    betas.resize(numElectrodes);
    gammas.resize(numElectrodes);

    deltaAmplitudes.resize(numElectrodes);
    thetaAmplitudes.resize(numElectrodes);
    alphaAmplitudes.resize(numElectrodes);
    betaAmplitudes.resize(numElectrodes);
    gammaAmplitudes.resize(numElectrodes);

    for (int i = 0; i < numElectrodes; ++i) {
        deltas[i] = QRandomGenerator::global()->bounded(1, 4);
        thetas[i] = QRandomGenerator::global()->bounded(4, 8);
        alphas[i] = QRandomGenerator::global()->bounded(8, 12);
        betas[i] = QRandomGenerator::global()->bounded(12, 30);
        gammas[i] = QRandomGenerator::global()->bounded(25, 140);

        deltaAmplitudes[i] = QRandomGenerator::global()->bounded(10, 100);
        thetaAmplitudes[i] = QRandomGenerator::global()->bounded(10, 100);
        alphaAmplitudes[i] = QRandomGenerator::global()->bounded(10, 100);
        betaAmplitudes[i] = QRandomGenerator::global()->bounded(10, 100);
        gammaAmplitudes[i] = QRandomGenerator::global()->bounded(10, 100);
    }

    m_eegUpdateTimer = new QTimer(this);
    connect(m_eegUpdateTimer, &QTimer::timeout, this, &EEGSimulator::updateEEGPlot);
    m_eegUpdateTimer->start(100);

    selectElectrode(0);
}

EEGSimulator::~EEGSimulator()
{
    delete m_eegUpdateTimer;
}

void EEGSimulator::setupEEGPlot() {
    m_customPlot->clearGraphs();
    QVector<QColor> electrodeColors = {
        QColor(Qt::red),
        QColor(Qt::green),
        QColor(Qt::blue),
        QColor(Qt::yellow),
        QColor(Qt::cyan),
        QColor(Qt::magenta),
        QColor(Qt::gray)
    };
    for (int i = 0; i < m_numElectrodes; ++i) {
        QCPGraph *graph = m_customPlot->addGraph();
        graph->setPen(QPen(electrodeColors[i % electrodeColors.size()], 2));

        graph->setLineStyle(QCPGraph::lsLine);
        graph->setAdaptiveSampling(true);
        graph->setScatterStyle(QCPScatterStyle::ssNone);
        graph->setAntialiased(true);

        m_customPlot->xAxis->setRange(0, 4000);
        m_customPlot->yAxis->setRange(-700, 700);
        m_customPlot->xAxis->setTickLabels(false);
        m_customPlot->xAxis->setTicks(false);
        m_customPlot->yAxis->setTickLabels(false);
        m_customPlot->yAxis->setTicks(false);

    }
    qDebug() << "Processing input waveform...";
}

void EEGSimulator::updateEEGPlot() {
    double currentTime = QDateTime::currentMSecsSinceEpoch();
    for (int i = 0; i < m_numElectrodes; ++i) {
        double eegValue = generateEEGData(currentTime, i, 0);
        m_customPlot->graph(i)->addData(currentTime, eegValue);
    }
    m_customPlot->xAxis->setRange(currentTime - 3000, currentTime);
    m_customPlot->replot();
}

void EEGSimulator::selectElectrode(int electrodeIndex) {
    if (m_customPlot != nullptr) {
        for (int i = 0; i < m_numElectrodes; ++i) {
            if (m_customPlot->graph(i) != nullptr) {
                m_customPlot->graph(i)->setVisible(i == electrodeIndex);
            } else {
                qDebug() << "Graph" << i << "is null";
            }
        }
        m_customPlot->replot();
    } else {
        qDebug() << "m_customPlot is null";
    }
}

double EEGSimulator::generateEEGData(double currentTime, int electrodeIndex, double offset) {
    double frequency = deltas[electrodeIndex] + betas[electrodeIndex] + alphas[electrodeIndex] + gammas[electrodeIndex] + thetas[electrodeIndex];
    double totalAmplitude = deltaAmplitudes[electrodeIndex] + thetaAmplitudes[electrodeIndex] +
                             alphaAmplitudes[electrodeIndex] + betaAmplitudes[electrodeIndex] +
                             gammaAmplitudes[electrodeIndex];
    frequency += offset;
    return totalAmplitude * sin(2 * M_PI * frequency * currentTime / 1000.0);
}


double EEGSimulator::calculateDominantFrequency(double currentTime, int electrodeIndex) {
    double totalAmplitude = deltaAmplitudes[electrodeIndex] + thetaAmplitudes[electrodeIndex] +
                             alphaAmplitudes[electrodeIndex] + betaAmplitudes[electrodeIndex] +
                             gammaAmplitudes[electrodeIndex];

    double dominantFrequency = ((deltas[electrodeIndex] * deltaAmplitudes[electrodeIndex]) + (betas[electrodeIndex] * betaAmplitudes[electrodeIndex]) +
            (alphas[electrodeIndex] * alphaAmplitudes[electrodeIndex]) + (gammas[electrodeIndex] * gammaAmplitudes[electrodeIndex]) +
            (thetas[electrodeIndex]) * thetaAmplitudes[electrodeIndex])/ totalAmplitude;

    return dominantFrequency;
}

void EEGSimulator::calculateBaseline() {
    QTimer::singleShot(1000, this, [=]() {
        QVector<double> baselineFrequencies;
        double currentTime = QDateTime::currentMSecsSinceEpoch();
        for (int i = 0; i < m_numElectrodes; ++i) {
            double dominantFrequency = calculateDominantFrequency(currentTime, i);
            baselineFrequencies.append(dominantFrequency);
        }
        m_baselineFrequencies = baselineFrequencies;
    });
}

void EEGSimulator::startTreatment() {
    qDebug() << "Therapy started.";
    QVector<double> offsetFrequencies = {5, 10, 15, 20};

    calculateBaseline();

    static int round = 0;
    round = 0;

    QTimer *therapyTimer = new QTimer(this);
    QTimer *pauseTimer = new QTimer(this);

    connect(therapyTimer, &QTimer::timeout, this, [=]() {
        double currentTime = QDateTime::currentMSecsSinceEpoch();

        if (round >= 4) {
            qDebug() << "Therapy finished.";
            therapyTimer->stop();
            therapyTimer->deleteLater();
            pauseTimer->stop();
            pauseTimer->deleteLater();
            return;
        }

        qDebug() << "Round" << round + 1 << "of therapy";

        for (int i = 0; i < m_baselineFrequencies.size(); ++i) {
            double feedbackFrequency = (m_baselineFrequencies[i] / 16.0) + offsetFrequencies[round];
            double feedbackStartTime = currentTime;
            for (int j = 0; j < m_numElectrodes; ++j) {
                double eegValue = generateEEGData(feedbackStartTime, j, feedbackFrequency);
                m_customPlot->graph(j)->addData(feedbackStartTime, eegValue);
            }
            qDebug() << "Administered hertz to Electrode" << i+1 << "with the frequency" << feedbackFrequency;
        }

        if (round < 4) {
            pauseTimer->start(5000);
            therapyTimer->stop();
            round++;
        }
    });

    connect(pauseTimer, &QTimer::timeout, this, [=]() {
        therapyTimer->start(1000);
        qDebug() << "Analysis complete.";
    });

    therapyTimer->start(1000);
}







