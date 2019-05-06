#include "mynetwork.h"

MyNetwork::MyNetwork(QObject *parent) : QObject(parent)
{
    m_udpSocket = new QUdpSocket(this);
    if(!m_udpSocket->bind(udpPort, QUdpSocket::ShareAddress )) //
        qDebug() << "m_udpSocket : " + m_udpSocket->errorString()  ;
    else
        qDebug() <<"UDP bind 127.0.0.1:" +QString::number(udpPort)+" LISTENING." ;

    connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(readyReadUDP()));
    avgBPM(0);
    minBPM(0);
    maxBPM(0);
    currentBPM(0);
    currentSpO2(0);
    for(int i = 0 ; i < m_max ; i++)
        m_vectorBPM << 0;
}

void MyNetwork::sendUDP(QString msg)
{
    if(enable_debug_udp)
        qDebug() << "sendUDP: " << msg ;
    QByteArray datagram = msg.toLatin1() ;
    m_udpSocket->writeDatagram(datagram.data(), datagram.size(),
                               QHostAddress::Broadcast, udpPort);
}

void MyNetwork::removeBackgroundGridChart(QAbstractSeries *xSeries, QAbstractSeries *ySeries)
{
    xSeries->setVisible(false);
    ySeries->setVisible(false);
}

void MyNetwork::readyReadUDP()
{
    while (m_udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(m_udpSocket->pendingDatagramSize());
        m_udpSocket->readDatagram(datagram.data(), datagram.size());

        QString data = datagram;
        if(enable_debug_udp)
            qDebug() << "readyReadUDP: " << data ;

        if(data.contains("<BPM>"))
        {
            data.replace("<BPM>","");
            currentBPM(data.toDouble());
            m_vectorBPM[m_index] = data.toDouble();

            m_index++ ;
            if(m_index >= m_max)
                m_index = 0;

            minBPM(getMin(m_vectorBPM));
            maxBPM(getMax(m_vectorBPM));
            avgBPM(getAvg(m_vectorBPM));

        }
        else if(data.contains("<Beat>"))
        {
            data.replace("<Beat>","");
            updateBPM(data.toDouble());

        }
        else if(data.contains("<SpO2>"))
        {
            data.replace("<SpO2>","");
            currentSpO2(data.toDouble());

        }





    }
}

int MyNetwork::getMin(QList<int> vector)
{
    int tmp = vector.at(0) ;
    for(int i = 1 ; i < vector.length() ; i++ )
        if( tmp > vector.at(i) )
            tmp = vector.at(i);
    return tmp ;
}

int MyNetwork::getMax(QList<int> vector)
{
    int tmp = 0 ;
    for(int i = 0 ; i < vector.length() ; i++ )
        if( tmp < vector.at(i) )
            tmp = vector.at(i);
    return tmp ;
}

int MyNetwork::getAvg(QList<int> vector)
{
    int tmp = 0 ;
    for(int i = 0 ; i < vector.length() ; i++ )
       tmp += vector.at(i);
    return tmp / vector.length() ;
}
