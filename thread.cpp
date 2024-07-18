#include "thread.h"
#include "camera.h"
#include "QMutex"
#include "QSemaphore"
#include <QtEndian> // 引入Qt处理字节序的头文件
#include <QBuffer>
#include "QTimer"
#include <QTime>
#include <QElapsedTimer>

QImage image[5];

//三个线程实例化对象
ProcessImg *processimg;
SendThread *sendthread;
RecvThread *recvthread;

QMutex judge_connect_mutex;

//RGB相机类跟光谱相机类实例化对象
CameraL *cameraL;
SpecCamera *speccamera;

bool is_connected;
extern tomato tomato;

//TCP协议
extern QTcpServer server;
extern QTcpSocket *clientSocket;
extern int start_flag;

bool spec_flag = 0;
extern bool is_timeout;
extern modbus_t* CZ;
QMutex imgR_muex;

//外部引用管道
HANDLE hPipe;
HANDLE specPipe;
HANDLE RPipe;
QString pipeName = "\\\\.\\pipe\\rgb_receive";
QString pipeName2 = "\\\\.\\pipe\\spec_receive";
QString rePipe = "\\\\.\\pipe\\rgb_send";

extern int save_flag;
extern int fruit_flag;
uint countimgR = 1;
uint countimgL = 1;
uint countimgTopT = 1;
uint countimgTopM = 1;
uint countimgTopB = 1;

int ImgQueue_Alignment = 1;
//右侧相机回调
extern void __stdcall onImageDataCallBackFunc1(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser)
{

    if (pFrameInfo)   //帧信息有效
    {



        //img 原始图
        QImage img(pData,pFrameInfo->nWidth,pFrameInfo->nHeight,QImage::Format_RGB888);        //构造图像
        //复制临时图 为了后续处理
        QImage tempimg = img;
        tempimg = tempimg.rgbSwapped();
        QImage queueImg = tempimg;
        QString filePath = "./image/R";
        QString imagePath = QString("%1/%2.bmp").arg(filePath).arg(countimgR);
        if(save_flag)
        {
            tempimg.save(imagePath);
        }

        emit cameraL->send_Rgbimage(tempimg);
        countimgR++;
        // cameraL->rightImgQueue.push(queueImg);
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
        QString filePath = "./image/L";
        QString imagePath = QString("%1/%2.bmp").arg(filePath).arg(countimgL);
        if(save_flag)
        {
            tempimg.save(imagePath);
        }

        countimgL++;

        emit cameraL->send_Rgbimage1(tempimg);

        // cameraL->leftImgQueue.push(queueImg);
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

//顶部相机回调（ip[4] 200）                  数据指针 数据是8位 图像默认格式是BGR   当前帧的结构体
extern void __stdcall onImageDataCallBackFunc(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser)
{

    if (pFrameInfo)   //帧信息有效
    {
        qDebug()<<"彩色相机触发次数:"<<countimgTopT;

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
        QString filePath = "./image/T/bottom";
        QString imagePath = QString("%1/%2.bmp").arg(filePath).arg(countimgTopB);
        if(save_flag)
        {
            tempimg.save(imagePath);

        }
        countimgTopB++;

        QImage tempimg1 = middlePart;
        tempimg1 = tempimg1.rgbSwapped();
        QImage queueImg1 = tempimg1;
        QString filePath1 = "./image/T/middle";
        QString imagePath1 = QString("%1/%2.bmp").arg(filePath1).arg(countimgTopM);
        if(save_flag)
        {
            tempimg1.save(imagePath1);
        }

        countimgTopM++;

        QImage tempimg2 = topPart;
        tempimg2 = tempimg2.rgbSwapped();
        QImage queueImg2 = tempimg2;
        QString filePath2 = "./image/T/top";
        QString imagePath2 = QString("%1/%2.bmp").arg(filePath2).arg(countimgTopT);
        if(save_flag)
        {
            tempimg2.save(imagePath2);
        }

        countimgTopT++;

        emit cameraL->send_Rgbimage2(tempimg,tempimg1,tempimg2);

        cameraL->topImgTQueue.push(queueImg);
        cameraL->topImgMQueue.push(queueImg1);
        cameraL->topImgBQueue.push(queueImg2);
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

int spec_count;
//光谱相机的回调函数 拿到数据后触发信号 并转到曹函数 cal（） 处理数据 如果放在回调处理数据会很慢
void __stdcall RTSpecStreamingCallback(void *pContext, unsigned char *pData, unsigned long dataLength)
{
    if(pData)
    {
        // spec_count++;
        // qDebug()<<"光谱相机触发次数:"<<spec_count;
        if(fruit_flag)
        {
            speccamera->cal(pData);

        }
        else
        {
            pData = nullptr;
        }
    }
    return;
}

//显示光谱伪彩色 不用管
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

//光谱数据处理线程 当 槽函数 cal（） 收集到指定数量的照片 也就是 满足 if (speccamera->frameData.size() == speccamera->totalFrames)
//就会进行处理
void ProcessImg::run()
{
    while(1)
    {
        // 检查是否已收集到足够的帧
        if (speccamera->SpecData_vector.size() == 25)
        {
            // 处理这些帧，例如拼接成一张大图
            // qDebug()<<"25行光谱图开始拼接、存队列";
            spec_count++;
            msleep(100);
            speccamera->test_count = 0;
            // qDebug()<<"光谱相机触发次数"<<spec_count;

            speccamera->specfullImage = new unsigned short[30 * 25 * 13];//宽 * 高 * 谱段数


            for (int i = 0; i < 25; i++)
            {
                // qDebug()<<"容量："<<speccamera->SpecData_vector.capacity();
                // qDebug()<<"容量实际大小："<<speccamera->SpecData_vector.size();
                memcpy(speccamera->specfullImage + i * 30 * 13, speccamera->SpecData_vector[i], 30 * 13 * sizeof(unsigned short));
            }
            speccamera->specImgQueue.push(speccamera->specfullImage);
            // qDebug()<<"光谱入队成功";

            if(save_flag)
            {
                static int file_index = 1;
                std::string spec_filename = "./image/specImg/spec" + std::to_string(file_index);
                FILE *fp = fopen(spec_filename.c_str(), "wb");
                int ret = fwrite(speccamera->specfullImage,  1, 30 * 25 * 13 * sizeof(unsigned short), fp);
                std::fflush(fp);
                fclose(fp);
                file_index++;
                qDebug()<<"save img success";
                qDebug()<<"test_count: "<<speccamera->test_count;
            }

            speccamera->SpecData_vector.clear();
            speccamera->test_count = 0;
        }
    }
}

//接收线程
RecvThread::RecvThread(QObject *parent) : QThread(parent)
{


}

RecvThread::~RecvThread()
{

}



//通过 pipe 管道 从python端读取数据
void RecvThread::run()
{



    while (1)
    {
        // 读取4字节的数据长度信息
        DWORD bytesRead;
        quint32 dataLength;
        QByteArray lengthBytes(4, 0);



        BOOL success = ReadFile(RPipe, lengthBytes.data(), 4, &bytesRead, NULL);
        if (!success || bytesRead != 4)
        {
            qDebug() << "Failed to read length from pipe:" << GetLastError();
            CloseHandle(RPipe);
            return;
        }


        QDataStream lengthStream(lengthBytes);
        lengthStream.setByteOrder(QDataStream::BigEndian);
        lengthStream >> dataLength;

        // qDebug() << "应该接收到的数据长度:" << dataLength;

        // 根据读取到的数据长度，读取对应长度的数据
        QByteArray data(dataLength, 0);
        success = ReadFile(RPipe, data.data(), dataLength, &bytesRead, NULL);
        if (!success || bytesRead != dataLength) {
            qDebug() << "Failed to read data from pipe:" << GetLastError();
            CloseHandle(RPipe);
            return;
        }

        // qDebug() << "接收到的数据长度:" << bytesRead;

        // 解析数据
        QDataStream dataStream(data);
        dataStream.setByteOrder(QDataStream::BigEndian);

        // 解析命令和其他数据
        QString cmd;
        quint16 brix, diameter, defectNum, height, width;
        quint8 greenPercentage, weight;
        quint32 totalDefectArea;

        char cmdChars[2];
        dataStream.readRawData(cmdChars, 2);
        cmd = QString::fromLatin1(cmdChars, 2).trimmed().toUpper();

        // 判断指令是否为 "RE"
        if (cmd == "RE")
        {
            // qDebug() << "Received 'RE' command. Processing accordingly.";
            // 继续解析其他数据
            dataStream >> brix >> greenPercentage >> diameter >> weight >> defectNum >> totalDefectArea >> height >> width;

            // qDebug() << "命令:" << cmd;
            // qDebug() << "Brix:" << brix << ", Green Percentage:" << greenPercentage << ", Diameter:" << diameter;
            // qDebug() << "Weight:" << weight << ", Defect Number:" << defectNum << ", Total Defect Area:" << totalDefectArea;
            // qDebug() << "Height:" << height << ", Width:" << width;
            // // 剩余的数据为图像数据
            // qDebug() << "data: " <<data.size() <<"Current position in data stream:" << dataStream.device()->pos();

            QByteArray imageData = data.right(data.size() - dataStream.device()->pos());
            // qDebug() << "imageData size: " << imageData.size();

            QImage imageResult((uchar*)imageData.constData(), width, height, QImage::Format_RGB888);
            if (!imageResult.isNull()) {
                // qDebug() << "Image loaded successfully";
                QImage swappedImage = imageResult.rgbSwapped();
                emit recv_Data(brix,greenPercentage,diameter,weight,defectNum,totalDefectArea,height,width,swappedImage);
                // imageResult.save("D:/WeChat Files/wxid_jjgh0jfum83v12/FileStorage/File/2024-06/1.png");
            }
            else
            {
                qDebug() << "Error: Failed to load image from raw data";
            }
            // qDebug() << "接收并处理完成";

        }
    }
}




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
        qDebug()<<"第"<<countimgTopT<<"次"<<"发送失败时字节数： "<<data.size();
        qDebug() << "Failed to write to pipe or no bytes written. Error:" << dwError;
        return false;
    }
    // qDebug()<<"第"<<countimgTopT<<"次"<<"发送成功字节数： "<<data.size();

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


//通过 pipe 管道 给python端发送数据
void SendThread::run()
{



    if(Yure_Flag &&  (hPipe != INVALID_HANDLE_VALUE))
    {
        // qDebug() << "预热前";

        QByteArray dataToSend;
        QDataStream dataStream(&dataToSend, QIODevice::WriteOnly);
        dataStream.setByteOrder(QDataStream::BigEndian);
        quint32 lenth = 2;
        dataStream << lenth;
        // 添加命令
        QString cmd = "YR";
        dataStream.writeRawData(cmd.toLocal8Bit().constData(), 2);
        // 发送数据
        DWORD yr_Written;
        if (!WriteFile(hPipe, dataToSend, 6, &yr_Written, nullptr) || yr_Written != 6) {
            DWORD dwError = GetLastError();
            qDebug() << "Failed to write to pipe or incorrect number of bytes written. Error:" << dwError;

            return;
        }
        else
        {
            qDebug() << "预热成功" ;
        }


    }

    while(1)
    {
        //读取五个RGB队列的队首数据
        if(cameraL->topImgTQueue.size()>15)
        {
            QElapsedTimer timer;
            timer.start();
            //数据对齐
            if(ImgQueue_Alignment)
            {
                cameraL->topImgMQueue.pop();
                cameraL->topImgMQueue.pop();

                cameraL->topImgBQueue.pop();
                cameraL->topImgBQueue.pop();
                cameraL->topImgBQueue.pop();

                cameraL->topImgTQueue.pop();

                ImgQueue_Alignment = 0;
            }

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
            // temp[3] = cameraL->leftImgQueue.front();
            // cameraL->leftImgQueue.pop();
            // qDebug()<<"左侧相机队列大小："<<cameraL->leftImgQueue.size();
            // temp[4] = cameraL->rightImgQueue.front();
            // cameraL->rightImgQueue.pop();
            // qDebug()<<"右侧相机队列大小："<<cameraL->rightImgQueue.size();
            //循环五次 将 temp[5] 中的所有图像数据转换为字节流 用于 pipe 管道发送

            for(int i=0;i<3;i++)
            {
                // Qt_image_count ++;
                // QString filePath = "C:/Users/succtech/Desktop/Qt_image";
                // QString imagePath = QString("%1/%2.bmp").arg(filePath).arg(Qt_image_count);
                // temp[i].save(imagePath);
                int width = temp[i].width();
                int height = temp[i].height();
                // qDebug()<<"原始图像字节: "<<temp[i].byteCount();
                QByteArray imgData(reinterpret_cast<const char*>(temp[i].bits()), temp[i].byteCount());
                // qDebug()<<"现在图像字节: "<<imgData.size();
                QDataStream stream(&imgData, QIODevice::ReadWrite);
                stream.setByteOrder(QDataStream::BigEndian);


                QByteArray dataToSend;
                QDataStream dataStream(&dataToSend, QIODevice::WriteOnly);
                dataStream.setByteOrder(QDataStream::BigEndian);


                quint32 length = static_cast<quint32>(imgData.size() + 6);
                dataStream << length;

                if(fruit_flag)     //百香果
                {
                    // 添加命令
                    QString cmd = "PF";
                    dataStream.writeRawData(cmd.toLocal8Bit().constData(), 2);
                    // Dimensions
                    dataStream << static_cast<quint16>(height);
                    dataStream << static_cast<quint16>(width);

                    // Image data
                    dataStream.writeRawData(imgData.constData(), imgData.size());

                    if (!sendData(hPipe, dataToSend))
                    {
                        qWarning() << "Failed to send data, attempting to reconnect...";
                        specPipe = reconnect(pipeName2);
                        if (specPipe == INVALID_HANDLE_VALUE) return;
                        if (!sendData(specPipe, dataToSend)) return;
                        return;
                    }


                    //当第五张RGB图发送完之后，发送光谱图
                    if(i==2)
                    {
                        unsigned short* specTemp = speccamera->specImgQueue.front();
                        // qDebug()<<"光谱数据大小:"<<specTemp;
                        speccamera->specImgQueue.pop();
                        // qDebug() << "光谱队列长度: " << speccamera->specImgQueue.size();

                        int specWidth = 30;
                        int specHeight = 25;
                        int specBands = 13;

                        QByteArray specByte,specByte_tosend;
                        specByte.append(reinterpret_cast<const char*>(specTemp), specWidth * specHeight * specBands * sizeof(unsigned short));
                        qDebug()<<"specByte: "<<specByte.size();
                        QDataStream spec_stream(&specByte_tosend, QIODevice::ReadWrite);
                        spec_stream.setByteOrder(QDataStream::BigEndian);

                        quint32 length = static_cast<quint32>(specByte.size() + 8);
                        spec_stream << length;

                        // 添加命令
                        QString cmd = "PF";
                        spec_stream.writeRawData(cmd.toLocal8Bit().constData(), 2);

                        // Dimensions
                        spec_stream << static_cast<quint16>(specHeight);
                        spec_stream << static_cast<quint16>(specWidth);
                        spec_stream << static_cast<quint16>(specBands);
                        specByte_tosend.append(specByte);
                        if (!sendData(specPipe, specByte_tosend))
                        {
                            qWarning() << "Failed to send data, attempting to reconnect...";
                            specPipe = reconnect(pipeName2);
                            if (specPipe == INVALID_HANDLE_VALUE) return;
                            if (!sendData(specPipe, specByte_tosend)) return;
                            return;
                        }
                        // qDebug() << "write to spec_pipe." << specByte_tosend.size();
                        msleep(10);
                        // qDebug() << "发送完成:";

                    }

                }
                else      //番茄
                {
                    // 添加命令
                    QString cmd = "TO";
                    dataStream.writeRawData(cmd.toLocal8Bit().constData(), 2);
                    // Dimensions
                    dataStream << static_cast<quint16>(height);
                    dataStream << static_cast<quint16>(width);

                    // Image data
                    dataStream.writeRawData(imgData.constData(), imgData.size());

                    if (!sendData(hPipe, dataToSend))
                    {
                        qWarning() << "Failed to send data, attempting to reconnect...";
                        specPipe = reconnect(pipeName2);
                        if (specPipe == INVALID_HANDLE_VALUE) return;
                        if (!sendData(specPipe, dataToSend)) return;
                        return;
                    }
                    // if(i == 2)
                    // {
                    //     // qDebug() << "光谱队列长度: " << speccamera->specImgQueue.size();
                    //     speccamera->specImgQueue.pop();
                    //     qDebug() << "发送完成:";

                    // }
                }
                // qint64 elapsed = timer.elapsed();
                // qDebug()<<"间隔时间"<<elapsed;
                // msleep(100);
            }
            // qint64 elapsed = timer.elapsed();
            // qDebug()<<"间隔时间"<<elapsed;
        }
    }
}
