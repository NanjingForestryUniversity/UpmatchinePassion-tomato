#include "camera.h"
#include "thread.h"
#include <vector>

#include <QTime>
// 使用std::vector存储所有帧的数据


extern int save_flag;
extern bool spec_flag;
tomato tomato;
void __stdcall onImageDataCallBackFunc(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser);
void __stdcall onOfflineCallBackFunc(unsigned int nMsgType, void* pUser);
void __stdcall onImageDataCallBackFunc1(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser);
void __stdcall onOfflineCallBackFunc1(unsigned int nMsgType, void* pUser);
void __stdcall onImageDataCallBackFunc2(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser);
void __stdcall onOfflineCallBackFunc2(unsigned int nMsgType, void* pUser);

extern void __stdcall RTRGBViewCallback(void* pContext, unsigned char* pRData, unsigned char* pGData, unsigned char* pBData, unsigned long dataLength);
extern void __stdcall RTSpecStreamingCallback(void *pContext, unsigned char *pData, unsigned long dataLength);

int camStatusret;
int camStatusret1;
int camStatusret2;
extern bool change_passionFlag;
extern bool change_tomatoFlag;
CameraL::CameraL(QObject *parent) : QObject(parent)
{

}

bool CameraL::initCameraL()
{
    /*
     * 枚举设备
     * */
    int device_num = enum_device();
    qDebug()<<"找到相机: "<<device_num;
    if(device_num == -1)
    {
        qDebug()<<"enum device failed";
        if(camera_handle != NULL)
        {
            destroy_handle();
        }
        return false;
    }
    if(device_num == 0)
    {
        qDebug()<<"no camera found";
        if(camera_handle != NULL)
        {
            destroy_handle();
        }
        return false;
    }

    //print_device_info();
    bool isOk = select_device();
    if(!isOk)
    {
        qDebug()<<"create handle failed";
        if(camera_handle != NULL)
        {
            destroy_handle();
        }
        if(camera_handle1 != NULL)
        {
            destroy_handle1();
        }
        if(camera_handle2 != NULL)
        {
            destroy_handle2();
        }
        return false;
    }

    /*
     * 开启相机
     * */
    isOk = open_camera();

    if(!isOk)
    {
        qDebug()<<"open camera failed";
        if(camera_handle != NULL)
        {
            destroy_handle();
        }
        return false;
    }else{
        qDebug()<<"打开相机: "<<"isOk: "<<isOk;
    }

    // set_ROI(0, 0, 2448, 2048);

    /*
     * 导入配置文件（相机）
     **/
    isOk = import_config_file();
    //    set_ROI(0, 668, 1000, 2448);
    if(!isOk)
    {
        qDebug()<<"import config file failed";
        if(camera_handle != NULL)
        {
            destroy_handle();
        }
        if(camera_handle1 != NULL)
        {
            destroy_handle1();
        }
        if(camera_handle2 != NULL)
        {
            destroy_handle2();
        }
        return false;;
    }

    isOk = register_image_callback(onImageDataCallBackFunc,onImageDataCallBackFunc1,onImageDataCallBackFunc2);
    qDebug()<<"注册回调成功: "<<"isOk: "<<isOk;
    if(!isOk)
    {
        qDebug()<<"register image callback function failed";
        if(camera_handle != NULL)
        {
            destroy_handle();
        }
        if(camera_handle1 != NULL)
        {
            destroy_handle1();
        }
        if(camera_handle2 != NULL)
        {
            destroy_handle2();
        }
        return false;
    }

    isOk = register_offline_callback(onOfflineCallBackFunc,onOfflineCallBackFunc1,onOfflineCallBackFunc2);
    qDebug()<<"取消注册回调成功: "<<"isOk: "<<isOk;
    if(!isOk)
    {
        qDebug()<<"register offline callback function failed";
        if(camera_handle != NULL)
        {
            destroy_handle();
        }
        if(camera_handle1 != NULL)
        {
            destroy_handle1();
        }
        if(camera_handle2 != NULL)
        {
            destroy_handle2();
        }
        return false;
    }
    isOk = set_acquisition_mode();
    qDebug()<<"设置触发成功: "<<"isOk: "<<isOk;
    if(!isOk)
    {
        qDebug()<<"set acquistion mode failed";
        if(camera_handle != NULL)
        {
            destroy_handle();
        }
        if(camera_handle1 != NULL)
        {
            destroy_handle1();
        }
        if(camera_handle2 != NULL)
        {
            destroy_handle2();
        }
        return false;
    }

    //    isOk = set_ROI(0, 668, 2448, 1000);
    //    if(!isOk)
    //    {
    //        qDebug()<<"SET ROI FAILED";
    //        if(camera_handle != NULL)
    //        {
    //            destroy_handle();
    //        }
    //        return false;
    //    }


    MV_CC_SetBalanceWhiteAuto(camera_handle, 0);
    MV_CC_SetBalanceWhiteAuto(camera_handle1, 0);
    MV_CC_SetBalanceWhiteAuto(camera_handle2, 0);
    MV_CC_SetIntValue(camera_handle, "LineDebouncerTime", 100);
    MV_CC_SetIntValue(camera_handle1, "LineDebouncerTime", 100);
    MV_CC_SetIntValue(camera_handle2, "LineDebouncerTime", 100);

    return true;


}

uint32_t CameraL::enum_device()
{
    int ret = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &device_list);
    if(ret != MV_OK)
    {
        return -1;
    }
    //qDebug()<<"device_list[0]: "<<device_list.pDeviceInfo[0];

    //qDebug()<<"device_list[1]: "<<device_list.pDeviceInfo[1];

    return device_list.nDeviceNum;
}


bool CameraL::select_device()
{
    MV_CC_DEVICE_INFO* pstDeviceInfo;


    for(i=0;i<=2;i++)
    {

        pstDeviceInfo = device_list.pDeviceInfo[i];
        if (NULL == pstDeviceInfo)
        {
            printf("The Pointer of pstMVDevInfo is NULL!\n");
            return false;
        }
        // qDebug()<<"nip1； "<<((pstDeviceInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0xff000000)>>24);
        // qDebug()<<"nip2； "<<((pstDeviceInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x00ff0000)>>16);
        // qDebug()<<"nip3； "<<((pstDeviceInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x0000ff00)>>8);
        // qDebug()<<"nip4； "<<((pstDeviceInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x000000ff));
        ip4 = pstDeviceInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x000000ff;
        switch (ip4) {
        case 100:
            MV_CC_CreateHandle(&camera_handle1, device_list.pDeviceInfo[i]);

            break;
        case 150:
            MV_CC_CreateHandle(&camera_handle, device_list.pDeviceInfo[i]);
            break;
        case 200:
            MV_CC_CreateHandle(&camera_handle2, device_list.pDeviceInfo[i]);
            break;
        default:
            break;
        }
        pstDeviceInfo++;
    }
    return true;
}

bool CameraL::open_camera()
{
    int ret = MV_CC_OpenDevice(camera_handle);
    int ret1 = MV_CC_OpenDevice(camera_handle1);
    int ret2 = MV_CC_OpenDevice(camera_handle2);
    camStatusret2=0;
    camStatusret1=0;
    camStatusret=0;
    if(ret != MV_OK||ret1 != MV_OK||ret2 != MV_OK)
    {
        return false;
        camStatusret2=0;
        camStatusret1=0;
        camStatusret=0;
    }

    return true;
}

bool CameraL::start_capture()
{
    int ret2 = MV_CC_StartGrabbing(camera_handle2);

    int ret1 = MV_CC_StartGrabbing(camera_handle1);

    int ret = MV_CC_StartGrabbing(camera_handle);
    if (ret != MV_OK||ret1 != MV_OK||ret2 != MV_OK)
    {
        return false;
    }
    return true;
}

bool CameraL::register_image_callback(imageCallbackFunc onImageDataCallBackFunc,imageCallbackFunc onImageDataCallBackFunc1,imageCallbackFunc onImageDataCallBackFunc2)
{
    int ret2 = MV_CC_RegisterImageCallBackForBGR(camera_handle1, onImageDataCallBackFunc, NULL);

    int ret1 = MV_CC_RegisterImageCallBackForBGR(camera_handle, onImageDataCallBackFunc2, NULL);

    int ret = MV_CC_RegisterImageCallBackForBGR(camera_handle2, onImageDataCallBackFunc1, NULL);
    if (ret != MV_OK||ret1 != MV_OK||ret2 != MV_OK)
    {
        return false;
    }
    return true;
}

bool CameraL::register_offline_callback(exceptionCallbackFunc onOfflineCallBackFunc, exceptionCallbackFunc onOfflineCallBackFunc1, exceptionCallbackFunc onOfflineCallBackFunc2)
{
    int nRet1 = MV_CC_RegisterExceptionCallBack(camera_handle, onOfflineCallBackFunc2, NULL);

    int nRet = MV_CC_RegisterExceptionCallBack(camera_handle2, onOfflineCallBackFunc1, NULL);
    int nRet2 = MV_CC_RegisterExceptionCallBack(camera_handle1, onOfflineCallBackFunc, NULL);
    if (nRet != MV_OK||nRet1 != MV_OK||nRet2 != MV_OK)
    {
        return false;
    }
    return true;
}



bool CameraL::destroy_handle()
{
    int ret = MV_CC_DestroyHandle(camera_handle);
    if (MV_OK != ret)
    {
        return false;
    }

    return true;
}
bool CameraL::destroy_handle1()
{
    int ret = MV_CC_DestroyHandle(camera_handle1);
    if (MV_OK != ret)
    {
        return false;
    }

    return true;
}bool CameraL::destroy_handle2()
{
    int ret = MV_CC_DestroyHandle(camera_handle2);
    if (MV_OK != ret)
    {
        return false;
    }

    return true;
}
bool CameraL::import_config_file()
{
    if(change_passionFlag&&(!change_tomatoFlag))
    {
        int ret = MV_CC_FeatureLoad(camera_handle, CAMERA_CONFIG_PATH);
        if(ret != MV_OK)
        {
            return false;
        }
        int ret2 = MV_CC_FeatureLoad(camera_handle2, CAMERA2_CONFIG_PATH);
        if(ret2 != MV_OK)
        {
            return false;
        }
        int ret1 = MV_CC_FeatureLoad(camera_handle1, CAMERAtop_CONFIG_PATH);
        if(ret1 != MV_OK)
        {
            return false;
        }
    }
    if((!change_passionFlag)&&change_tomatoFlag)
    {
        int ret = MV_CC_FeatureLoad(camera_handle, CAMERApassion_left_config);
        if(ret != MV_OK)
        {
            return false;
        }
        int ret2 = MV_CC_FeatureLoad(camera_handle2, CAMERApassion_right_config);
        if(ret2 != MV_OK)
        {
            return false;
        }
        int ret1 = MV_CC_FeatureLoad(camera_handle1, CAMERAtoppassion_top2_config);
        if(ret1 != MV_OK)
        {
            return false;
        }
    }
    return true;

}

bool CameraL::save_config_file()
{
    int ret = MV_CC_FeatureSave(camera_handle, CAMERA_CONFIG_PATH);
    if(ret != MV_OK)
    {
        return false;
    }
    return true;
}

bool CameraL::set_param(Camera_param param_struct)
{
    camera_param = param_struct;
    int ret = MV_CC_SetIntValue(camera_handle, "BalanceRatio", camera_param.white_balance_ratio);
    if (ret != MV_OK)
    {
        qDebug()<<"white balance ration set failed";
        return false;
    }

    ret = MV_CC_SetFloatValue(camera_handle, "ExposureTime", camera_param.exposure_time);
    if(ret != MV_OK)
    {
        qDebug()<<"exposure time set failed";
        return false;
    }

    ret = MV_CC_SetFloatValue(camera_handle, "Gain", camera_param.gain);
    if(ret != MV_OK)
    {
        qDebug()<<"gain set failed";
        return false;
    }

    return true;
}

Camera_param CameraL::get_param()
{
    MVCC_INTVALUE BalanceRatio = {0};
    int ret = MV_CC_GetIntValue(camera_handle, "BalanceRatio", &BalanceRatio);
    if (ret == MV_OK)
    {
        camera_param.white_balance_ratio = BalanceRatio.nCurValue;
    }
    else
    {
        qDebug()<<"get white balance ratio failed";
    }


    MVCC_FLOATVALUE ExposureTime = {0};
    ret = MV_CC_GetFloatValue(camera_handle, "ExposureTime", &ExposureTime);
    if(ret == MV_OK)
    {
        camera_param.exposure_time = ExposureTime.fCurValue;
    }
    else
    {
        qDebug()<<"get exposure time failed";
    }

    MVCC_FLOATVALUE Gain = {0};
    ret = MV_CC_GetFloatValue(camera_handle, "Gain", &Gain);
    if(ret == MV_OK)
    {
        camera_param.gain = Gain.fCurValue;
    }
    else
    {
        qDebug()<<"get gain failed";
    }

#if 1
    qDebug()<< camera_param.white_balance_ratio;
    qDebug()<<camera_param.exposure_time;
    qDebug()<<camera_param.gain;

#endif

    return camera_param;
}
/*
 * TriggerMode  0  OFF
 *              1  ON
 *
 * TriggerSource Line0  0
 *               Line1  1
 *               Line2  2
 *               Line3  3
 *               Counter0 4
 *               Software  7
 *               FrequencyConverter  8
 *TriggerActivation  0:RisingEdge
                     1:FallingEdge
                     2:LevelHigh
                     3:LevelLow
   AcquisitionMode   0:SingleFrame
                     1:MultiFrame
                     2:Continuous
 * */
bool CameraL::set_acquisition_mode()
{
    int ret = MV_CC_SetEnumValue(camera_handle, "TriggerMode", 1);
    int ret1 = MV_CC_SetEnumValue(camera_handle1, "TriggerMode",1);
    int ret2 = MV_CC_SetEnumValue(camera_handle2, "TriggerMode", 1);
     if(MV_OK != ret || MV_OK != ret1 ||MV_OK != ret2)
     {
         return false;
     }

    int ret3 = MV_CC_SetEnumValue(camera_handle, "TriggerSource",0);
    int ret4 = MV_CC_SetEnumValue(camera_handle1, "TriggerSource", 0);
    int ret5 = MV_CC_SetEnumValue(camera_handle2, "TriggerSource", 0);
     if(MV_OK != ret3 || MV_OK != ret4 ||MV_OK != ret5)
     {
         return false;
     }

    int ret6 = MV_CC_SetEnumValue(camera_handle, "TriggerActivation", 0);
    int ret7 = MV_CC_SetEnumValue(camera_handle1, "TriggerActivation", 0);
    int ret8 = MV_CC_SetEnumValue(camera_handle2, "TriggerActivation", 0);
     if(MV_OK != ret6 || MV_OK != ret7 ||MV_OK != ret8)
     {
         return false;
     }

    int ret9 = MV_CC_SetEnumValue(camera_handle, "AcquisitionMode", 2);  //采集模式连续
    int ret10 = MV_CC_SetEnumValue(camera_handle1, "AcquisitionMode", 2);
    int ret11 = MV_CC_SetEnumValue(camera_handle2, "AcquisitionMode", 2);
     if(MV_OK != ret9 || MV_OK != ret10 ||MV_OK != ret11)
     {
         return false;
     }

    return true;
}

bool CameraL::stop_capture()
{

    int ret = MV_CC_StopGrabbing(camera_handle);
    int ret1 = MV_CC_StopGrabbing(camera_handle1);
     int ret2 = MV_CC_StopGrabbing(camera_handle2);
     if(MV_OK!=ret){
         destroy_handle();
     }
     if(MV_OK!=ret1){
         destroy_handle1();
     }
     if(MV_OK!=ret2){
         destroy_handle2();
     }
    return true;
}


//spec camera
SpecCamera::SpecCamera(QObject *parent)
{

}

bool SpecCamera::init_SpecCamera()
{
    //打开相机
    int ires = RTOpenCamera();
    qDebug()<<"ires: "<<ires;
    if (ires != 1)
    {
        printf("Open Camera failed!");
        return false;
    }
    //获取当前系统相机参数
    m_uiDepth = RTGetCameraDepth();    //获取相机图像最大值（256/4096/65536）
    RTGetImageProperty(m_iWidth, m_iHeight);  //获取图像宽、高
    m_pusSaveBuffer = new unsigned short[m_iHeight*((m_iWidth * 3 + 3) / 4 * 4)];
    m_pRefreshBuffer = new unsigned char[m_iHeight*((m_iWidth * 3 + 3) / 4 * 4)];


    RTGetCameraGain(m_fGain);
    RTGetExposure(m_fExpTime);
    RTGetCameraFrameRate(m_iFrameRate);
    RTGetMinAndMaxWavelength(m_fWaveLengthMin, m_fWaveLengthMax);

    RTGetRGBselect(m_cslRed, m_cslGreen, m_cslBlue);


    RTSetExposure(m_fExpTime);
    RTSetCameraGain(m_fGain);
    RTSetCameraFrameRate(m_iFrameRate);
    RTApplyCameraParam();
    if (m_cslRed > m_iHeight || m_cslGreen > m_iHeight || m_cslBlue > m_iHeight
        || m_cslRed <= 0 || m_cslGreen <= 0 || m_cslBlue <= 0)
    {
        m_cslRed = m_iHeight * 3 / 4;
        m_cslGreen = m_iHeight / 2;
        m_cslBlue = m_iHeight / 4;
    }
    RTSetRGBselect(m_cslRed, m_cslGreen, m_cslBlue);
    // RTAddRGBCallback(RTRGBViewCallback, nullptr);
    RTAddStreamingCallback(RTSpecStreamingCallback,nullptr);
    return true;
}

bool SpecCamera::cal(unsigned char *pData)
{
    // QTime currenttime = QTime::currentTime();
    // qDebug()<<"触发时间："<<currenttime;










    // unsigned short* spec_data = new unsigned short[1024*40]; //2048*224*1
    // unsigned short* spec_data_short = (unsigned short *)pData;
    // memcpy(spec_data, spec_data_short + 1024 * 40 , 1024 * 40 * sizeof(unsigned short));
    // //将处理后的帧数据存储到vector中
    // frameData.push_back(spec_data);
    // // 检查是否已收集到足够的帧
    // if (frameData.size() == totalFrames)
    // {

    //     // 处理这些帧，例如拼接成一张大图
    //     specfullImage = new unsigned short[1024 * 40 * totalFrames];

    //     for (int i = 0; i < totalFrames ; i++)
    //     {
    //         memcpy(specfullImage + i * 1024 * 40, frameData[i] , 1024 * 40 * sizeof(unsigned short));
    //     }
    //     frameData.clear();
    //     if(save_flag)
    //     {
    //         static int file_index = 1;
    //         std::string spec_filename = "E:/qt_tomato_passion/new/20240422/image/specImg/spec" + std::to_string(file_index);
    //         FILE *fp = fopen(spec_filename.c_str(), "wb");
    //         int ret = fwrite(specfullImage,  1, totalFrames * 1024 * 40 * sizeof(unsigned short), fp);
    //         // qDebug()<<"光谱队列长度: "<<frameData.size()<<" ret : "<<ret;

    //         std::fflush(fp);
    //         fclose(fp);
    //         file_index++;
    //         std::cout<<"save img success"<<std::endl;
    //         // qDebug()<<"test_count: "<<test_count;
    //     }


    // }


    // memcpy(specfullImage + test_count * m_iWidth * 224, spec_data + 500 * 224, 1024 * 224 * sizeof(unsigned short));
    // test_count++;
    // if(test_count == totalFrames)
    // {
    //     QTime starttime = QTime::currentTime();
    //     //specImgQueue.push(specfullImage);

    //     test_count = 0;
    //     QTime stoptime = QTime::currentTime();
    //     int functime = starttime.msecsTo(stoptime);
    //     qDebug() << "Time elapsed for image" <<functime;
    // }


    return true;
}



bool SpecCamera::stop_capture()
{
    bool ret = trigger_close();
    if(ret == false)
    {
        qDebug()<<"trigger is not close:"<<ret;
        return false;
    }
    else
    {
        qDebug()<<"trigger is close:"<<ret;
    }
    qDebug()<<"ret: "<<ret;
    if (RTIsCameraWorking())
    {
        RTStopSingleCapture();
    }
    return true;
}

bool SpecCamera::trigger_open()
{
    bool ret = RT_SetBool(L"Camera.TriggerIn", true);       //设置相机外触发

    if(ret==false)
    {
        qDebug()<<"trigger is not open:"<<ret;
        return false;
    }
    else
    {
        qDebug()<<"trigger is open:"<<ret;
    }
    return true;
}

bool SpecCamera::trigger_close()
{
    bool ret = RT_SetBool(L"Camera.TriggerIn", false);

    if(ret==false)
    {
        qDebug()<<"trigger is not close:"<<ret;
        return false;
    }
    else
    {
        qDebug()<<"trigger is close:"<<ret;
    }
    return true;
}

bool SpecCamera::start_capture()
{
    trigger_open();
    bool flag = RTStartSingleCapture(false, false, "E:/qt_tomato_passion/new/20240422/image/specImg/newdata.raw");
    if (!flag)
    {
        printf("erro to start capture");
        return 1;
    }
    return true;
}
