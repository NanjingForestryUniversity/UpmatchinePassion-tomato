#include "thread.h"
#include "camera.h"
#include "QMutex"
#include "QSemaphore"
#include <QtEndian> // 引入Qt处理字节序的头文件
#include <QBuffer>
#include "QTimer"
#include <QTime>

ProcessImg *processimg;
QImage image[5];

SendThread *sendthread;
RecvThread *recvthread;
QMutex judge_connect_mutex;
CameraL *cameraL;

SpecCamera *speccamera;
bool is_connected;
extern tomato tomato;
extern QTcpServer server;
extern QTcpSocket *clientSocket;
extern int start_flag;
bool spec_flag = 0;
extern bool is_timeout;
extern modbus_t* CZ;
QMutex imgR_muex;

extern int save_flag;
uint countimgR = 1;
uint countimgL = 1;
uint countimgTopT = 1;
uint countimgTopM = 1;
uint countimgTopB = 1;

//右侧相机回调
extern void __stdcall onImageDataCallBackFunc1(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser)
{

    if (pFrameInfo)   //帧信息有效
    {
        // qDebug()<<"右部相机帧号: "<<pFrameInfo->nFrameNum;

        QImage img(pData,pFrameInfo->nWidth,pFrameInfo->nHeight,QImage::Format_RGB888);        //构造图像
        QImage tempimg = img;
        tempimg = tempimg.rgbSwapped();
        QImage queueImg = tempimg;
        QString filePath = "E:/qt_tomato_passion/new/20240422/image/R";
        QString imagePath = QString("%1/%2.bmp").arg(filePath).arg(countimgR);
        if(save_flag)
        {
            tempimg.save(imagePath);
        }

        countimgR++;
        // qDebug()<<"右侧相机帧号: "<<pFrameInfo->nFrameNum;
        emit cameraL->send_Rgbimage(tempimg);

        cameraL->rightImgQueue.push(queueImg);
        // qDebug()<<"右侧相机队列长度： "<<cameraL->rightImgQueue.size();

    }
    return ;

}

extern void __stdcall onOfflineCallBackFunc1(unsigned int nMsgType, void* pUser)
{
    //qDebug()<<"camera offline";
    judge_connect_mutex.lock();
    is_connected = false;
    judge_connect_mutex.unlock();
    return;
}

//左侧相机回调
extern void __stdcall onImageDataCallBackFunc2(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser)
{
    if (pFrameInfo)
    {
        // qDebug()<<"左侧相机帧号: "<<pFrameInfo->nFrameNum;
        QImage imgL(pData,pFrameInfo->nWidth,pFrameInfo->nHeight,QImage::Format_RGB888);

        QImage tempimg = imgL;
        tempimg = tempimg.rgbSwapped();
        QImage queueImg = tempimg;
        QString filePath = "E:/qt_tomato_passion/new/20240422/image/L";
        QString imagePath = QString("%1/%2.bmp").arg(filePath).arg(countimgL);
        if(save_flag)
        {
            tempimg.save(imagePath);
        }

        countimgL++;

        emit cameraL->send_Rgbimage1(tempimg);

        cameraL->leftImgQueue.push(queueImg);
        // qDebug()<<"左侧相机队列长度： "<<cameraL->leftImgQueue.size();



    }
    return ;
}

extern void __stdcall onOfflineCallBackFunc2(unsigned int nMsgType, void* pUser)
{
    judge_connect_mutex.lock();
    is_connected = false;
    judge_connect_mutex.unlock();
    return;
}
//顶部相机回调（ip[4] 100）
extern void __stdcall onImageDataCallBackFunc(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser)
{

    if (pFrameInfo)   //帧信息有效
    {
        // qDebug()<<"顶部相机帧号: "<<pFrameInfo->nFrameNum;

        QImage imgT(pData,pFrameInfo->nWidth,pFrameInfo->nHeight,QImage::Format_RGB888);        //构造图像

        /*
         * 裁切图片三等分
         * */
        int width = imgT.width();
        int height = imgT.height();
        int partHeight = height / 3;

        QImage topPart = imgT.copy(0, 0, width, partHeight);
        QImage middlePart = imgT.copy(0, partHeight, width, partHeight);
        QImage bottomPart = imgT.copy(0, 2 * partHeight, width, partHeight);

        /*
         * 颜色转换+存图
         * */
        QImage tempimg = bottomPart;
        tempimg = tempimg.rgbSwapped();
        QImage queueImg = tempimg;
        QString filePath = "E:/qt_tomato_passion/new/20240422/image/T/bottom";
        QString imagePath = QString("%1/%2.bmp").arg(filePath).arg(countimgTopB);
        if(save_flag)
        {
            tempimg.save(imagePath);

        }
        countimgTopB++;

        QImage tempimg1 = middlePart;
        tempimg1 = tempimg1.rgbSwapped();
        QImage queueImg1 = tempimg1;
        QString filePath1 = "E:/qt_tomato_passion/new/20240422/image/T/middle";
        QString imagePath1 = QString("%1/%2.bmp").arg(filePath1).arg(countimgTopM);
        if(save_flag)
        {
            tempimg1.save(imagePath1);
        }

        countimgTopM++;

        QImage tempimg2 = topPart;
        tempimg2 = tempimg2.rgbSwapped();
        QImage queueImg2 = tempimg2;
        QString filePath2 = "E:/qt_tomato_passion/new/20240422/image/T/top";
        QString imagePath2 = QString("%1/%2.bmp").arg(filePath2).arg(countimgTopT);
        if(save_flag)
        {
            tempimg2.save(imagePath2);
        }

        countimgTopT++;


        /*
         * 触发显示信号
         * */
        emit cameraL->send_Rgbimage2(tempimg,tempimg1,tempimg2);

        /*
         * 顶部裁切后的图片入队
         * */
        cameraL->topImgTQueue.push(queueImg);
        cameraL->topImgMQueue.push(queueImg1);
        cameraL->topImgBQueue.push(queueImg2);

        // qDebug()<<"顶部上相机队列长度： "<<cameraL->topImgTQueue.size();
        // qDebug()<<"顶部中相机队列长度： "<<cameraL->topImgMQueue.size();
        // qDebug()<<"顶部下相机队列长度： "<<cameraL->topImgBQueue.size();



    }
    return ;
}

extern void __stdcall onOfflineCallBackFunc(unsigned int nMsgType, void* pUser)
{
    //qDebug()<<"camera offline";
    judge_connect_mutex.lock();
    is_connected = false;
    judge_connect_mutex.unlock();
    return;
}



void __stdcall RTSpecStreamingCallback(void *pContext, unsigned char *pData, unsigned long dataLength)
{
    // unsigned char* pPData = new unsigned char[speccamera->m_iWidth*speccamera->m_iHeight*224]; //2048*224*1
    // memcpy(pPData, pData, dataLength); // 直接从pData拷贝全部数据到pPData
    // unsigned char* pPData1 = new unsigned char[speccamera->m_iWidth*speccamera->m_iHeight*224];
    // int k = 0;
    // for( int i = 0; i < speccamera->m_iHeight; i++ )
    // {
    //     for(uint j = 91; j < 101; j++ )
    //     {
    //         memcpy(pPData1 + speccamera->m_iWidth * k, pPData + (i * speccamera->m_iHeight + j) * speccamera->m_iWidth,
    //                sizeof(unsigned char) * speccamera->m_iWidth);
    //         k++;
    //     }
    // }
    // speccamera->specImgQueue.push(pPData1);
    // qDebug()<<"spec queue length: "<<speccamera->specImgQueue.size();
    // delete []pPData;
    // delete []pPData1;

    if(pData)
    {
        // qDebug()<<"pData: "<<pData;
        speccamera->cal(pData);
        unsigned short* spec_total_data= new unsigned short[2048 * 100 * speccamera->totalFrames];//2048*224*1;
        unsigned short* spec_data_short = (unsigned short *)pData;
        memcpy(spec_total_data + speccamera->test_count * speccamera->m_iWidth * 100, spec_data_short, 2048 * 100 * sizeof(unsigned short));
        speccamera->test_count++;
        if(speccamera->test_count == speccamera->totalFrames)
        {
            qDebug()<<"光谱相机触发了一张图";
            // unsigned short* spec_data_short_copy = new unsigned short[speccamera->m_iWidth * 100 * speccamera->totalFrames]; //2048*224*1
            // memcpy(spec_data_short_copy, spec_total_data,speccamera->m_iWidth * 100 * speccamera->totalFrames * sizeof(unsigned short));
            // emit speccamera->process_spec(spec_data_short_copy);
            spec_total_data = nullptr;
            speccamera->test_count = 0;

            // if(save_flag)
            // {

            //     static int file_index = 1;
            //     std::string spec_filename = "E:/qt_tomato_passion/new/20240422/image/specImg/spec" + std::to_string(file_index);
            //     FILE *fp = fopen(spec_filename.c_str(), "wb");

            //     int ret = fwrite(spec_data_short_copy,  1, speccamera->totalFrames * speccamera->m_iWidth * 40 * sizeof(unsigned short), fp);
            //     // qDebug()<<"光谱队列长度: "<<frameData.size()<<" ret : "<<ret;

            //     std::fflush(fp);
            //     fclose(fp);
            //     file_index++;
            //     std::cout<<"save img success"<<std::endl;
            //     // qDebug()<<"test_count: "<<speccamera->test_count;



            // }
        }



    }



    return;
}
void __stdcall RTRGBViewCallback(void* pContext, unsigned char* pRData, unsigned char* pGData, unsigned char* pBData, unsigned long dataLength)
{
    unsigned short* pShortRData = (unsigned short*)pRData;
    unsigned short* pShortGData = (unsigned short*)pGData;
    unsigned short* pShortBData = (unsigned short*)pBData;

    unsigned short m_usCurrentMax = 1;
    unsigned short m_usCurrentMin = 65535;

    for (int i = 0; i < speccamera->m_iWidth; i++)           //拉伸
    {
        if (pShortRData[i] > m_usCurrentMax)
            m_usCurrentMax = pShortRData[i];
        if (pShortGData[i] > m_usCurrentMax)
            m_usCurrentMax = pShortGData[i];
        if (pShortBData[i] > m_usCurrentMax)
            m_usCurrentMax = pShortBData[i];
        if (pShortRData[i] < m_usCurrentMin)
            m_usCurrentMin = pShortRData[i];
        if (pShortGData[i] < m_usCurrentMin)
            m_usCurrentMin = pShortGData[i];
        if (pShortBData[i] < m_usCurrentMin)
            m_usCurrentMin = pShortBData[i];
    }
    int iLineByte = (speccamera->m_iWidth * 3 + 3) / 4 * 4;        //字节对齐
    memcpy(speccamera->m_pusSaveBuffer, speccamera->m_pusSaveBuffer + iLineByte, (speccamera->m_iHeight - 1)*iLineByte*sizeof(unsigned short));
    for (int i = speccamera->m_iWidth - 1; i >= 0; i = i - 1)
    {
        int sel = (speccamera->m_iHeight - 1)*  iLineByte + i  * 3;
        if (sel < iLineByte * speccamera->m_iHeight )
        {
            speccamera->m_pusSaveBuffer[sel + 2] = pShortRData[i];
            speccamera->m_pusSaveBuffer[sel + 1] = pShortGData[i];
            speccamera->m_pusSaveBuffer[sel + 0] = pShortBData[i];
        }
    }                                                                           //拷贝
    speccamera->m_iDrawCount++;


    // qDebug()<<"speccamera->m_iDrawCount: "<<speccamera->m_iDrawCount;
    for (int i = 0; i < speccamera->m_iHeight; i++)
    {
        for (int j = 0; j < speccamera->m_iWidth; j++)
        {
            if (m_usCurrentMax - m_usCurrentMin*0.9 == 0)
            {
                speccamera->m_pRefreshBuffer[i*iLineByte+j*3 + 0] = 0;
                speccamera->m_pRefreshBuffer[i*iLineByte+j*3 + 1] = 0;
                speccamera->m_pRefreshBuffer[i*iLineByte+j*3 + 2] = 0;

            }
            else
            {
                speccamera->m_pRefreshBuffer[i*iLineByte+j*3 + 0] = (speccamera->m_pusSaveBuffer[i*iLineByte+j*3 + 2]-m_usCurrentMin*0.9)*255.0/(m_usCurrentMax-m_usCurrentMin*0.9);
                speccamera->m_pRefreshBuffer[i*iLineByte+j*3 + 1] = (speccamera->m_pusSaveBuffer[i*iLineByte+j*3 + 1]-m_usCurrentMin*0.9)*255.0/(m_usCurrentMax-m_usCurrentMin*0.9);
                speccamera->m_pRefreshBuffer[i*iLineByte+j*3 + 2] = (speccamera->m_pusSaveBuffer[i*iLineByte+j*3 + 0]-m_usCurrentMin*0.9)*255.0/(m_usCurrentMax-m_usCurrentMin*0.9);
            }
        }
    }


    QImage image(speccamera->m_pRefreshBuffer,speccamera->m_iWidth, speccamera->m_iHeight, QImage::Format_RGB888);
    // speccamera->specImgQueue.push(speccamera->m_pRefreshBuffer);

    emit speccamera->show_SpecImg(image);

    return;
}
//图像处理线程，通过相机得到图片，然后裁剪后放入队列，并一同发给python
ProcessImg::ProcessImg(QObject *parent) : QThread(parent)
{

}

ProcessImg::~ProcessImg()
{

}

void ProcessImg::exitThread()
{
    stop_mutex.lock();
    m_stop = true;
    stop_mutex.unlock();
}

void ProcessImg::run()
{
    connect(speccamera,SIGNAL(process_spec(unsigned short*)),this,SLOT(processSpecSlots(unsigned short*)));

    // while(1)
    // {
    //     uint16_t czData[8];
    //     imgR_muex.lock();
    //     modbus_read_registers(CZ,0x0000,1,czData);
    //     qDebug()<<"重量数据： "<<czData;
    //     if(czData[5]==1)
    //     {

    //         spec_flag = 1;
    //         modbus_write_register(CZ,0x0005,0);
    //     }
    //     imgR_muex.unlock();


    // }
    // while(1)
    // {

    //         // 检查是否已收集到足够的帧
    //         if (speccamera->frameData.size() == speccamera->totalFrames)
    //         {
    //             // 处理这些帧，例如拼接成一张大图

    //             speccamera->specfullImage = new unsigned short[speccamera->m_iWidth * 224 * speccamera->totalFrames];
    //             for (int i = 0; i < speccamera->totalFrames; ++i)
    //             {
    //                 memcpy(speccamera->specfullImage + i * speccamera->m_iWidth * 224, speccamera->frameData[i], speccamera->m_iWidth * 224 * sizeof(unsigned short));
    //             }
    //             speccamera->specImgQueue.push(speccamera->specfullImage);

    //             if(save_flag)
    //             {

    //                 static int file_index = 1;
    //                 std::string spec_filename = "E:/qt_tomato_passion/new/20240422/image/specImg/spec" + std::to_string(file_index);
    //                 FILE *fp = fopen(spec_filename.c_str(), "wb");

    //                 int ret = fwrite(speccamera->specfullImage,  1, speccamera->totalFrames * speccamera->m_iWidth * 224 * sizeof(unsigned short), fp);
    //                 // qDebug()<<"光谱队列长度: "<<frameData.size()<<" ret : "<<ret;

    //                 std::fflush(fp);
    //                 fclose(fp);
    //                 file_index++;
    //                 std::cout<<"save img success"<<std::endl;
    //                 qDebug()<<"test_count: "<<speccamera->test_count;
    //             }
    //             speccamera->frameData.clear();
    //             speccamera->test_count = 0;
    //         }



    //         // 检查是否已收集到足够的帧
    //         if (speccamera->frameData_copy.size() == speccamera->totalFrames)
    //         {
    //             speccamera->vector_flag = 0;
    //             // speccamera->vector_flag = 1;
    //             // 处理这些帧，例如拼接成一张大图
    //             speccamera->specfullImage = new unsigned short[speccamera->m_iWidth * 224 * speccamera->totalFrames];
    //             for (int i = 0; i < speccamera->totalFrames; ++i)
    //             {
    //                 memcpy(speccamera->specfullImage + i * speccamera->m_iWidth * 224, speccamera->frameData_copy[i], speccamera->m_iWidth * 224 * sizeof(unsigned short));
    //             }
    //             speccamera->specImgQueue.push(speccamera->specfullImage);

    //             if(save_flag)
    //             {
    //                 static int file_index = 1;
    //                 std::string spec_filename = "E:/qt_tomato_passion/new/20240422/image/specImg/spec" + std::to_string(file_index);
    //                 FILE *fp = fopen(spec_filename.c_str(), "wb");

    //                 int ret = fwrite(speccamera->specfullImage,  1, speccamera->totalFrames * speccamera->m_iWidth * 224 * sizeof(unsigned short), fp);
    //                 // qDebug()<<"光谱队列长度: "<<frameData.size()<<" ret : "<<ret;

    //                 std::fflush(fp);
    //                 fclose(fp);
    //                 file_index++;
    //                 std::cout<<"save img success"<<std::endl;
    //                 qDebug()<<"test_count: "<<speccamera->test_count;
    //             }
    //             speccamera->frameData_copy.clear();
    //             speccamera->test_count = 0;
    //         }

        // }

}

void ProcessImg::processSpecSlots(unsigned short *data)
{
    // // qDebug()<<"sssssssssssssssssssssssssssssss";
    // speccamera->frameData.push_back(data);
    // // 检查是否已收集到足够的帧
    // if (speccamera->frameData.size() == speccamera->totalFrames)
    // {
    //     // 处理这些帧，例如拼接成一张大图

    //     speccamera->specfullImage = new unsigned short[speccamera->m_iWidth * 50 * speccamera->totalFrames];
    //     for (int i = 0; i < speccamera->totalFrames + 1; i++)
    //     {
    //         memcpy(speccamera->specfullImage + i * speccamera->m_iWidth * 50, speccamera->frameData[i], speccamera->m_iWidth * 50 * sizeof(unsigned short));
    //     }
        // speccamera->specImgQueue.push(data);


        // speccamera->frameData.clear();
        // speccamera->test_count = 0;
    // }
}

//接收线程
RecvThread::RecvThread(QObject *parent) : QThread(parent)
{


}

RecvThread::~RecvThread()
{

}




void RecvThread::run()
{


    QString rePipe = "\\\\.\\pipe\\rgb_send";

    // 创建并连接到命名管道
    HANDLE RPipe = CreateFile(
        (LPCWSTR)rePipe.utf16(),   // 管道名称
        GENERIC_READ,              // 更改为读取访问
        0,                         // 不共享
        NULL,                      // 默认安全属性
        OPEN_EXISTING,             // 打开现有管道
        FILE_ATTRIBUTE_NORMAL,     // 默认属性
        NULL                       // 没有模板文件
        );

    if (RPipe == INVALID_HANDLE_VALUE) {
        qWarning() << "Failed to open pipe for reading:" << GetLastError();
        return;
    }

    while (1)
    {

        // 读取各个部分的数据
        quint16 long_axis, short_axis, defect_num;
        quint32 total_defect_area, imgSize;
        DWORD readBytes;

        ReadFile(RPipe, &long_axis, sizeof(long_axis), &readBytes, NULL);
        long_axis = qFromBigEndian(long_axis);


        ReadFile(RPipe, &short_axis, sizeof(short_axis), &readBytes, NULL);
        short_axis = qFromBigEndian(short_axis);

        ReadFile(RPipe, &defect_num, sizeof(defect_num), &readBytes, NULL);
        defect_num = qFromBigEndian(defect_num);

        ReadFile(RPipe, &total_defect_area, sizeof(total_defect_area), &readBytes, NULL);
        total_defect_area = qFromBigEndian(total_defect_area);

        ReadFile(RPipe, &imgSize, sizeof(imgSize), &readBytes, NULL);
        imgSize = qFromBigEndian(imgSize);

        // 读取图像数据
        // 根据图像大小读取图像数据
        QByteArray imageData;
        imageData.resize(imgSize - 4);  // 减去4字节的长度信息
        if (!ReadFile(RPipe, imageData.data(), imgSize - 4, &readBytes, NULL) || readBytes != imgSize - 4) {
            qDebug() << "Failed to read image data."<< readBytes;

            return;

        }
        // QByteArray imageData;
        // imageData.resize(imgSize);
        // ReadFile(RPipe, imageData.data(), imgSize, &readBytes, NULL);
        qDebug()<<"long_axis: "<<long_axis<<"short_axis: "<<short_axis<<"defect_num: "<<defect_num<<"total_defect_area: "<<total_defect_area<<"imgSize: "<<imgSize;


        // qDebug()<<"imageData: "<<imageData.size();

        QImage image;
        QBuffer buffer(&imageData);
        buffer.open(QIODevice::ReadOnly);

        if (image.load(&buffer, "bmp")) {  // 指定假定的图像格式，如BMP

            emit recv_Data(long_axis,short_axis,defect_num,total_defect_area,image);
            // 可以在此处添加进一步的图像处理或显示代码
        } else {
            qDebug() << "Failed to load image.";
        }



    }

    // CloseHandle(RPipe);  // Make sure to close the pipe handle


}



//发送线程
SendThread::SendThread(QObject *parent) : QThread(parent)
{

}

SendThread::~SendThread()
{


}

bool SendThread::sendData(HANDLE &hPipe, const QByteArray &data)
{
    DWORD bytesWritten;
    if (!WriteFile(hPipe, data.constData(), static_cast<DWORD>(data.size()), &bytesWritten, nullptr) || bytesWritten == 0) {
        DWORD dwError = GetLastError();
        qDebug() << "Failed to write to pipe or no bytes written. Error:" << dwError;
        return false;
    }
    return true;
}

HANDLE SendThread::reconnect(const QString &pipeName)
{
    CloseHandle(hPipe); // Close the old handle before reconnecting
    HANDLE hPipe = CreateFile((LPCWSTR)pipeName.utf16(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hPipe == INVALID_HANDLE_VALUE) {
        qWarning() << "Failed to connect to pipe:" << GetLastError();
    }
    return hPipe;
}

void SendThread::SendImgToPython()
{



}



void SendThread::run()
{

    QString pipeName = "\\\\.\\pipe\\rgb_receive";

    // 创建并连接到命名管道
    hPipe = CreateFile(
        (LPCWSTR)pipeName.utf16(),   // 管道名称
        GENERIC_WRITE,               // 写入访问
        0,                           // 不共享
        NULL,                        // 默认安全属性
        OPEN_EXISTING,               // 打开现有管道
        FILE_ATTRIBUTE_NORMAL,       // 默认属性
        NULL                         // 没有模板文件
        );

    QString pipeName2 = "\\\\.\\pipe\\spec_receive";
    DWORD written;

    // 创建并连接到命名管道（光谱相机发送）
    specPipe = CreateFile(
        (LPCWSTR)pipeName2.utf16(),   // 管道名称
        GENERIC_WRITE,               // 写入访问
        0,                           // 不共享
        NULL,                        // 默认安全属性
        OPEN_EXISTING,               // 打开现有管道
        FILE_ATTRIBUTE_NORMAL,       // 默认属性
        NULL                         // 没有模板文件
        );



    while(1)
    {
        // cameraL->rightImgQueue.pop();
        // cameraL->rightImgQueue.pop();
        // cameraL->topImgMQueue.pop();
        // cameraL->topImgBQueue.pop();
        // cameraL->topImgBQueue.pop();

        //(cameraL->topImgTQueue.size()>5)&&(cameraL->topImgMQueue.size()>5)&&(cameraL->topImgBQueue.size()>5)&&(cameraL->leftImgQueue.size()>5)&&(cameraL->rightImgQueue.size()>5)
        if(speccamera->specImgQueue.size()>3)
        {
            QTime starttime = QTime::currentTime();
            QImage temp[5];
            temp[0] = cameraL->topImgTQueue.front();
            cameraL->topImgTQueue.pop();
            qDebug()<<"顶部相机顶队列大小："<<cameraL->topImgTQueue.size();
            temp[1] = cameraL->topImgMQueue.front();
            cameraL->topImgMQueue.pop();
            qDebug()<<"顶部相机中队列大小："<<cameraL->topImgMQueue.size();
            temp[2] = cameraL ->topImgBQueue.front();
            cameraL -> topImgBQueue.pop();
            qDebug()<<"顶部相机下队列大小："<<cameraL->topImgBQueue.size();
            temp[3] = cameraL->leftImgQueue.front();
            cameraL->leftImgQueue.pop();
            qDebug()<<"左侧相机队列大小："<<cameraL->leftImgQueue.size();
            temp[4] = cameraL->rightImgQueue.front();
            cameraL->rightImgQueue.pop();
            qDebug()<<"右侧相机队列大小："<<cameraL->rightImgQueue.size();
            for(int i=0;i<5;i++)
            {

                // std::string filePath = "C:/tomato0410data/top/middle";
                // static int count = 1;
                // QString imgpath = QString("%1/%2.bmp").arg(QString::fromStdString(filePath)).arg(count);
                // count++;
                // QImage img(imgpath);              // 读取图像

                QByteArray imageData;
                QBuffer buffer(&imageData);

                buffer.open(QIODevice::WriteOnly);
                temp[i].save(&buffer, "bmp");  // 将图片保存为 bmp 格式的字节流



                // 创建一个新的 QByteArray 来包含长度和图像数据
                QByteArray lengthToSend;
                QDataStream lengthStream(&lengthToSend, QIODevice::WriteOnly);
                lengthStream.setByteOrder(QDataStream::BigEndian);  // 确保字节顺序与接收方一致
                // 写入图像数据长度（4字节）
                lengthStream << quint32(imageData.size());

                // 追加实际的图像数据
                lengthToSend.append(imageData);
                // qDebug()<<"lengthToSend: "<<lengthToSend.size();
                if (!sendData(hPipe, lengthToSend)) {
                    qWarning() << "Failed to send data, attempting to reconnect...";
                    hPipe = reconnect(pipeName);
                    if (hPipe == INVALID_HANDLE_VALUE) return;
                    if (!sendData(hPipe, lengthToSend)) return;
                    return;
                }
                buffer.close(); // Ensure the buffer is closed after use

                //qDebug() << "Image sent successfully, bytes written:" << lengthToSend.size();

                msleep(10);
                qDebug()<<"光谱队列发送前长度: "<<speccamera->specImgQueue.size();

                if(i==4)
                {
                    unsigned short* specTemp = speccamera->specImgQueue.front();
                    speccamera->specImgQueue.pop();
                    qDebug()<<"光谱队列长度: "<<speccamera->specImgQueue.size();
                    QByteArray specByte;
                    specByte.append(reinterpret_cast<const char*>(specTemp),1024 * 224 * speccamera->totalFrames);
                    QByteArray lengthToSend;
                    QDataStream lengthStream(&lengthToSend, QIODevice::WriteOnly);
                    lengthStream.setByteOrder(QDataStream::BigEndian);  // 确保字节顺序与接收方一致
                    // 写入图像数据长度（4字节）
                    lengthStream << quint32(specByte.size());

                    // 追加实际的图像数据
                    lengthToSend.append(specByte);
                    DWORD written; // 确保声明这个变量
                    BOOL success = WriteFile(
                        specPipe,
                        lengthToSend.constData(),  // 发送包含长度的完整数据
                        lengthToSend.size(),  // 使用完整数据的大小
                        &written,
                        NULL
                        );
                    qDebug() << "write to pipe."<<lengthToSend.size()<<"success: "<<success;
                    msleep(10);
                }
            }
            QTime stoptime = QTime::currentTime();
            qDebug()<<"发送五张图运行时间："<<starttime.msecsTo(stoptime);
        }

    }
    // CloseHandle(hPipe); // Make sure to close the pipe handle


}
