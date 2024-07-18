#include "camera.h"
#include "thread.h"
#include <vector>
// 使用std::vector存储所有帧的数据

//外部引用的标志位 save_flag 存图标志位 RGB 光谱是否需要保存图 （不能单独存一个 RGB 或 光谱 ， 只能要存全存 除非再加标志位）
//spec_flag 没用到
extern int save_flag;
extern bool spec_flag;
tomato tomato;

//RGB相机的回调函数
void __stdcall onImageDataCallBackFunc(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser);
void __stdcall onOfflineCallBackFunc(unsigned int nMsgType, void* pUser);
void __stdcall onImageDataCallBackFunc1(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser);
void __stdcall onOfflineCallBackFunc1(unsigned int nMsgType, void* pUser);
void __stdcall onImageDataCallBackFunc2(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser);
void __stdcall onOfflineCallBackFunc2(unsigned int nMsgType, void* pUser);

//光谱相机的回调函数
extern void __stdcall RTRGBViewCallback(void* pContext, unsigned char* pRData, unsigned char* pGData, unsigned char* pBData, unsigned long dataLength);
extern void __stdcall RTSpecStreamingCallback(void *pContext, unsigned char *pData, unsigned long dataLength);

//三个相机是否连接上的标志位 在 initcamera 里面的 opencamera 函数有返回值 负责修改 ui界面的 "是否连接" 标志位
int camStatusret;
int camStatusret1;
int camStatusret2;

//果蔬标志位
extern int fruit_flag;
CameraL::CameraL(QObject *parent) : QObject(parent)
{

}


//初始化相机
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



    return true;
}


//下面是所有 initcamera 函数内调用的函数的具体内容
uint32_t CameraL::enum_device()
{
    int ret = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &device_list);
    if(ret != MV_OK)
    {
        return -1;
    }

    return device_list.nDeviceNum;
}

//这里需要注意 因为是三个相机  且有位置顺序 所以需要根据相机的第四位 ip 来确定 （所有相机都必须分配固定ip）
//比如相机 ip 为 192.168.2.10   10 则为相机的第四位 ip  如果发现顺序不对，使用MVS软件重新分配固定 ip
//200顶部相机   100右侧相机   150左侧相机
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

        ip4 = pstDeviceInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x000000ff;
        //根据第四位 ip 来创建 camera_handle
        //camera_handle 相当于 一个指向相机的指针 这个指针就代表相机
        switch (ip4)
        {
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

bool CameraL::open_camera2()
{
    /*
     * 导入配置文件（相机）
     **/
    bool isOk = import_config_file();
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

    // MV_CC_SetBalanceWhiteAuto(camera_handle, 0);
    // MV_CC_SetBalanceWhiteAuto(camera_handle1, 0);
    // MV_CC_SetBalanceWhiteAuto(camera_handle2, 0);
    // MV_CC_SetIntValue(camera_handle, "LineDebouncerTime", 100);
    // MV_CC_SetIntValue(camera_handle1, "LineDebouncerTime", 100);
    // MV_CC_SetIntValue(camera_handle2, "LineDebouncerTime", 100);
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
    int ret2 = MV_CC_RegisterImageCallBackForBGR(camera_handle1, onImageDataCallBackFunc1, NULL);  //100 right

    int ret1 = MV_CC_RegisterImageCallBackForBGR(camera_handle, onImageDataCallBackFunc2, NULL);   //150  left

    int ret = MV_CC_RegisterImageCallBackForBGR(camera_handle2, onImageDataCallBackFunc, NULL);   //200  top
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
}

bool CameraL::destroy_handle2()
{
    int ret = MV_CC_DestroyHandle(camera_handle2);
    if (MV_OK != ret)
    {
        return false;
    }

    return true;
}

//前面的两个标志位决定导入哪一个相机配置文件
bool CameraL::import_config_file()
{
    if(fruit_flag)   //百香果
    {
        qDebug()<<"import passion camera ini";
        int ret = MV_CC_FeatureLoad(camera_handle, CAMERApassion_left_config);
        if(ret != MV_OK)
        {
            return false;
        }
        int ret2 = MV_CC_FeatureLoad(camera_handle2, CAMERAtoppassion_top2_config);
        if(ret2 != MV_OK)
        {
            return false;
        }
        int ret1 = MV_CC_FeatureLoad(camera_handle1, CAMERApassion_right_config);
        if(ret1 != MV_OK)
        {
            return false;
        }
    }
    else   //西红柿
    {
        qDebug()<<"import tomato camera ini";

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


//光谱相机的所有函数
SpecCamera::SpecCamera(QObject *parent)
{

}

//初始化相机
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
    RTAddRGBCallback(RTRGBViewCallback, nullptr);
    RTAddStreamingCallback(RTSpecStreamingCallback,nullptr);
    return true;
}

//这个 cal 函数 是光谱相机的回调函数内触发的一个槽函数 可以在 thread 里面的光谱相机的回调找到
bool SpecCamera::cal(unsigned char *pData)
{
    test_count++;
    //假设筛选后的ROI 为  30 * 100   宽 * 谱段数
    //按右上角为数据初始点 900像素点 每一行第 900 个像素点 第 73 个谱段



    unsigned short* spec_total_data = new unsigned short[30 * 13]; //宽 * 谱段数

    unsigned short* spec_short_data = (unsigned short *)pData;
    int Band[13]={8,9,10,48,49,50,77,80,103,108,115,143,145};
    for(int k=0;k<13;k++)
    {
        memcpy(spec_total_data + k * 30, spec_short_data + 900 + (Band[k] - 1) * 2048, 30 * sizeof(unsigned short));
    }
    if (test_count < 26)
    {
    SpecData_vector.push_back(spec_total_data);
    }
    else
    {
        delete[] spec_total_data; // 释放内存以防止内存泄漏
    }



    return true;
}


//光谱相机内置函数 不用管
bool SpecCamera::stop_capture()
{
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

bool SpecCamera::start_capture()
{
    trigger_open();
    bool flag = RTStartSingleCapture(false, false, "E:/QT/projects/tomato20240415/tomato04150955/image/newdata.raw");
    if (!flag)
    {
        printf("erro to start capture");
        return 1;
    }
    return true;
}
