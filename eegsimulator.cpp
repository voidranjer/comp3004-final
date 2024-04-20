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
    m_eegUpdateTimer->start(1000 / 60); // 60 FPS
    // m_eegUpdateTimer->start(100);

    // Init timers
    observationTimer = new QTimer(this);
    observationTimer->setSingleShot(true);
    connect(observationTimer, &QTimer::timeout, this, &EEGSimulator::beginFeedback);

    feedbackTimer = new QTimer(this);
    feedbackTimer->setSingleShot(true);
    connect(feedbackTimer, &QTimer::timeout, this, &EEGSimulator::endFeedback);

    waiting = false;
}

EEGSimulator::~EEGSimulator()
{

}

void EEGSimulator::setupEEGPlot() {
    m_customPlot->clearGraphs();
    m_customPlot->xAxis->setRange(0, GRAPH_XRANGE);
    m_customPlot->yAxis->setRange(-GRAPH_YRANGE, GRAPH_YRANGE);
    m_customPlot->xAxis->setTickLabels(false);
    m_customPlot->xAxis->setTicks(false);
    m_customPlot->yAxis->setTickLabels(false);
    m_customPlot->yAxis->setTicks(false);

    // Add one additional overlay graph for feedback (different color) - this last graph is reserved: m_customPlot->graph(NUM_ELECTRODES)
    int numGraphs = NUM_ELECTRODES + 1;

    for (int i = 0; i < numGraphs; ++i) {

        QCPGraph *graph = m_customPlot->addGraph();

        graph->setLineStyle(QCPGraph::lsLine);
        graph->setAdaptiveSampling(true);
        graph->setScatterStyle(QCPScatterStyle::ssNone);
        graph->setAntialiased(true);

        // set color based on electrodes
        if (i < NUM_ELECTRODES) {
            Electrode *electrode = electrodes[i];
            graph->setPen(QPen(electrode->getColor(), 2));
        } else {
            // special config for feedback graph
            graph->setPen(QPen(Qt::yellow, 5));
        }
    }

    qDebug() << "Processing input waveform...";
}

void EEGSimulator::updateEEGPlot() {
    // if (!inSession) return;

    if (!inContact && inSession && !waiting) {
        waiting = true;
        qDebug() << "Waiting for contact";
    }

    if (waiting && inContact) {
        waiting = false;
    }

    // draw electrode graphs
    double eegValue;
    double currentTime = QDateTime::currentMSecsSinceEpoch();
    for (int i = 0; i < NUM_ELECTRODES; ++i) {
        Electrode *electrode = electrodes[i];
        eegValue = inContact ? generateEEGData(currentTime, electrode, 0) : 0;

        // select feedback graph instead of regular graph if feedback is true
        QCPGraph *graph = isFeedback ? m_customPlot->graph(NUM_ELECTRODES) : m_customPlot->graph(i);
        graph->addData(currentTime, eegValue);
    }

    m_customPlot->xAxis->setRange(currentTime - GRAPH_XRANGE, currentTime);
    m_customPlot->replot();
}

void EEGSimulator::beginFeedback()
{
    isFeedback = true;

    emit administerFeedback();

    // clear previous feedback graph (to prevent jumps in the line)
    m_customPlot->graph(NUM_ELECTRODES)->data()->clear();

    therapyRound++;

    if (therapyRound < NUM_ROUNDS) {
        double baseline = calculateBaseline();
        qDebug() << "Calculating baseline... BASELINE CALCULATED:" << baseline;

        if (therapyRound == 1) {
            startingBaseline = baseline;
        }

        qDebug() << "Administering feedback... ( ROUND: " << therapyRound << ")";
        qDebug() << "=============ROUND " << therapyRound << "– SENDING FEEDBACK (for" << FEEDBACK_DURATION << "ms)=============";
    }

    feedbackTimer->start(FEEDBACK_DURATION);
}

void EEGSimulator::endFeedback()
{
    QVector<double> OFFSETS = {5, 10, 15, 20};

    isFeedback = false;

    if (therapyRound == NUM_ROUNDS) {
        double endingBaseline = calculateBaseline();
        qDebug() << "FINAL MEASUREMENTS COMPLETE. COMPUTING FINAL BASELINE... FINAL BASELINE:" << endingBaseline;
        emit sessionCompleted(startingBaseline, endingBaseline);
        return;
    }

    for (int i = 0; i < NUM_ELECTRODES; ++i) {
        Electrode *electrode = electrodes[i];

        double feedbackFrequency = (m_baselineFrequencies[i] / 16.0) + OFFSETS[therapyRound - 1];

        // Permanently add feedback frequency to electrode
        electrode->addOffset(feedbackFrequency);

        // Reflect in feedback graph
        double feedbackTime = QDateTime::currentMSecsSinceEpoch();
        for (int j = 0; j < NUM_ELECTRODES; ++j) {
            m_customPlot->graph(j)->addData(feedbackTime, 0); // reset all graphs to 0 before making the jump (to avoid visual jumps due to cut off by feedback graph)
            double eegValue = generateEEGData(feedbackTime, electrode, feedbackFrequency, FEEDBACK_FACTOR);
            m_customPlot->graph(j)->addData(feedbackTime, eegValue);
        }

        qDebug() << "Administered hertz to Electrode" << i+1 << "with the frequency" << feedbackFrequency;
    }


    qDebug() << "=============ROUND " << therapyRound + 1 << "– BEGIN MEASUREMENT (for" << OBSERVE_DURATION << "ms)=============";
    observationTimer->start(OBSERVE_DURATION);
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

double EEGSimulator::generateEEGData(double currentTime, Electrode *electrode, double offset, double amplitudeFactor) {
    double frequency = electrode->getFreqSum();
    double totalAmplitude = electrode->getAmplitudeSum() * amplitudeFactor;
    frequency += offset;
    return totalAmplitude * sin(2 * M_PI * frequency * currentTime / 1000.0);
}



double EEGSimulator::calculateBaseline() {
    return electrodes[m_currentElectrodeIndex]->getDominantFrequency();
}

void EEGSimulator::startSession() {
    therapyRound = 0; // reset rounds
    inSession = true;
    qDebug() << "Starting new therapy session...";

    qDebug() << "=============ROUND 1 – BEGIN MEASUREMENT (for " << OBSERVE_DURATION << "ms)=============";
    observationTimer->start(OBSERVE_DURATION);
}

void EEGSimulator::endSession() {
    inSession = false;
    observationTimer->stop();
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

void EEGSimulator::pauseTreatment() {
    if (inSession && !waiting && !isFeedback) {
        observationTimer->stop();
        isPaused = true;
        qDebug() << "Treatment paused.";
    }
}

void EEGSimulator::continueTreatment() {
    if (inSession && isPaused && !isFeedback) {
        observationTimer->start();
        isPaused = false;
        qDebug() << "Treatment resumed.";
    }
}
