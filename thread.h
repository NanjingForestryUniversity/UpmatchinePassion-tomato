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

//处理线程 负责光谱相机的完整数据拼接
class ProcessImg : public QThread
{
    Q_OBJECT
public:
    explicit ProcessImg(QObject *parent = nullptr);
    ~ProcessImg();


    void exitThread();
    QMutex stop_mutex;

    int spec_count;


protected:
    void run();
private:
    bool m_stop;


signals:



public slots:

private:

};

//发送线程 负责使用管道 pipe 给python端发送五张图片
//五张图片分别为 顶部 上、中、下三张  左 右 两张
//待修改：
//1、数据对齐
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

    int Yure_Flag = 1;

    int Qt_image_count = 0;
private:

    void SendImgToPython();

signals:

private slots:
    //void sendimg_topySlots(cv::Mat img1,cv::Mat img2,cv::Mat img3,cv::Mat img4,cv::Mat img5);
};

//接收线程  负责接收python端通过 pipe 返回的数据 长径 短径 缺陷数量 缺陷面积 并在窗口展示
//待修改：
//1、具体返回值放在哪里 窗口位置需要调整
//2、判断条件写死 具体数据写活
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

    void recv_Data(quint16,quint8,quint16,quint8,quint16,quint32,quint16,quint16,QImage);

};

#endif // THREAD_H
