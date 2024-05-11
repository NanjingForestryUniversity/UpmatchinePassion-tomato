#ifndef THREAD_H
#define THREAD_H
#include "QThread"
#include "QFile"
#include <queue>
#include <QFileDialog>
#include "camera.h"
#include "queue"
#include "QMutex"
#include <QTcpServer>
#include <QTcpSocket>
#include "src/modbus.h"
#include <QImage>


class ProcessImg : public QThread
{
    Q_OBJECT
public:
    explicit ProcessImg(QObject *parent = nullptr);
    ~ProcessImg();


    void exitThread();
    QMutex stop_mutex;


protected:
    void run();
private:
    bool m_stop;


signals:




public slots:
    void processSpecSlots(unsigned short* data);
private:

};


class SendThread: public QThread
{
    Q_OBJECT

protected:
    void run();

public:
    explicit SendThread(QObject *parent = NULL);
    ~SendThread();
    bool sendData(HANDLE &hPipe, const QByteArray &data);
    HANDLE reconnect(const QString &pipeName);
private:
    HANDLE hPipe;
    HANDLE specPipe;
    void SendImgToPython();

signals:

private slots:
    //void sendimg_topySlots(cv::Mat img1,cv::Mat img2,cv::Mat img3,cv::Mat img4,cv::Mat img5);
};


class RecvThread: public QThread
{
    Q_OBJECT

protected:
    void run();

public:
    explicit RecvThread(QObject *parent = NULL);
    ~RecvThread();



public slots:


signals:

    void recv_Data(quint16,quint16,quint16,quint32,QImage);
};


#endif // THREAD_H
