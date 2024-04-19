#include <eegsimulator.h>

EEGSimulator::EEGSimulator(QObject *parent, QCustomPlot *plotWidget)
    : QObject(parent), m_customPlot(plotWidget)
{
    QColor electrodeColors[] = {
        QColor(Qt::red),
        QColor(Qt::green),
        QColor(Qt::blue),
        QColor(Qt::black),
        QColor(Qt::cyan),
        QColor(Qt::magenta),
        QColor(Qt::gray)
    };

    for (int i = 0; i < NUM_ELECTRODES; ++i) {
        double delta = QRandomGenerator::global()->bounded(1, 4);
        double theta = QRandomGenerator::global()->bounded(4, 8);
        double alpha = QRandomGenerator::global()->bounded(8, 12);
        double beta = QRandomGenerator::global()->bounded(12, 30);
        double gamma = QRandomGenerator::global()->bounded(25, 140);

        QColor color = electrodeColors[i % 7];

        electrodes[i] = new Electrode(this, color, delta, theta, alpha, beta, gamma);
    }

    setupEEGPlot();
    selectElectrode(0);

    m_eegUpdateTimer = new QTimer(this);
    connect(m_eegUpdateTimer, &QTimer::timeout, this, &EEGSimulator::updateEEGPlot);
    m_eegUpdateTimer->start(GRAPH_SPEED);
}

EEGSimulator::~EEGSimulator()
{

}

void EEGSimulator::setupEEGPlot() {
    m_customPlot->clearGraphs();

    for (int i = 0; i < NUM_ELECTRODES; ++i) {
        Electrode *electrode = electrodes[i];

        QCPGraph *graph = m_customPlot->addGraph();
        graph->setPen(QPen(electrode->getColor(), 2));

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
    // if (!inSession) return;

    if (!inContact && inSession) {
        qDebug() << "Waiting for contact";
    }

    double currentTime = QDateTime::currentMSecsSinceEpoch();
    for (int i = 0; i < NUM_ELECTRODES; ++i) {
        Electrode *electrode = electrodes[i];

        double eegValue = inContact ? generateEEGData(currentTime, electrode, 0) : 0;
        m_customPlot->graph(i)->addData(currentTime, eegValue);
    }

    m_customPlot->xAxis->setRange(currentTime - 3000, currentTime);
    m_customPlot->replot();
}

void EEGSimulator::selectElectrode(int electrodeIndex) {
    for (int i = 0; i < NUM_ELECTRODES; ++i) {
        if (m_customPlot->graph(i) == nullptr) {
            qDebug() << "Graph" << i << "is null";
            continue;
        }

        m_customPlot->graph(i)->setVisible(i == electrodeIndex);
    }

    m_currentElectrodeIndex = electrodeIndex;
    m_customPlot->replot();
}

double EEGSimulator::generateEEGData(double currentTime, Electrode *electrode, double offset) {
    double frequency = electrode->getFreqSum();
    double totalAmplitude = electrode->getAmplitudeSum();
    frequency += offset;
    return totalAmplitude * sin(2 * M_PI * frequency * currentTime / 1000.0);
}



void EEGSimulator::calculateBaseline() {
    qDebug() << "Measuring baseline...";
    QTimer::singleShot(1000, this, [=]() {
        for (int i = 0; i < NUM_ELECTRODES; ++i) {
            double dominantFrequency = electrodes[i]->getDominantFrequency();
            m_baselineFrequencies[i] = dominantFrequency;
        }
    });
}

void EEGSimulator::startSession() {
    inSession = true;

    qDebug() << "Therapy session started.";
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

        for (int i = 0; i < NUM_ELECTRODES; ++i) {
            Electrode *electrode = electrodes[i];

            double feedbackFrequency = (m_baselineFrequencies[i] / 16.0) + offsetFrequencies[round];
            double feedbackStartTime = currentTime;

            for (int j = 0; j < NUM_ELECTRODES; ++j) {
                double eegValue = generateEEGData(feedbackStartTime, electrode, feedbackFrequency);
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

void EEGSimulator::endSession() {
    inSession = false;
}

bool EEGSimulator::toggleContact()
{
    inContact = !inContact;
    return inContact;
}

bool EEGSimulator::getInContact() const
{
    return inContact;
}

bool EEGSimulator::getInSession() const
{
    return inSession;
}






