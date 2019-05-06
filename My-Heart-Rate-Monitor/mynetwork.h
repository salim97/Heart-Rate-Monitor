#ifndef MYNETWORK_H
#define MYNETWORK_H

#include <QObject>
#include <QUdpSocket>
#include <QtCharts>

#include "mypropertyhelper.h"

#define enable_debug_udp true

class MyNetwork : public QObject
{
    Q_OBJECT
public:
    explicit MyNetwork(QObject *parent = nullptr);
    AUTO_PROPERTY(bool, isfingerPressed)
    AUTO_PROPERTY(int, currentBPM)
    AUTO_PROPERTY(int, currentSpO2)
    AUTO_PROPERTY(int, avgBPM)
    AUTO_PROPERTY(int, minBPM)
    AUTO_PROPERTY(int, maxBPM)
signals:
    void updateBPM(int bpm);
public slots:
    void sendUDP(QString msg);
    void removeBackgroundGridChart(QAbstractSeries *xSeries, QAbstractSeries *ySeries);
private slots:
    void readyReadUDP();

private:
    int udpPort = 5551 ;
    QUdpSocket *m_udpSocket = nullptr;
    QList<int> m_vectorBPM;
    int m_max = 5  ;
    int m_index = 0 ;
    int getMin(QList<int> vector);
    int getMax(QList<int> vector);
    int getAvg(QList<int> vector);
};

#endif // MYNETWORK_H
