#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include "MvCameraControl.h"
#include "SpectrolDll.h"
#include "QDebug"
#include "opencv2/opencv.hpp"
#include "QImage"
typedef void(*imageCallbackFunc)(unsigned char * , MV_FRAME_OUT_INFO_EX* , void* );
typedef void(*exceptionCallbackFunc)(unsigned int , void* );
#define CAMERApassion_left_config "E:/qt_tomato_passion/new/20240421/left.mfs"
#define CAMERApassion_right_config "E:/qt_tomato_passion/new/20240421/right.mfs"
#define CAMERAtoppassion_top2_config "E:/qt_tomato_passion/new/20240421/top2.mfs"


#define CAMERA_CONFIG_PATH "E:/qt_tomato_passion/new/20240421/tomatoLeft.mfs"
#define CAMERA2_CONFIG_PATH "E:/qt_tomato_passion/new/20240421/tomatoRight.mfs"
#define CAMERAtop_CONFIG_PATH "E:/qt_tomato_passion/new/20240421/tomatoTop.mfs"

class tomato
{
public:
    unsigned int count=0;
    QImage img1;
    QImage img2;
    QImage img3;
    QImage img4;
    QImage img5;


};
struct Camera_param       //相机参数在此添加，在构造函数中给出默认值，设置参数需要刷新,左侧
{
    float exposure_time;
    float gain;
    int white_balance_ratio;
    //... other params
};
struct Camera_paramR      //相机参数在此添加，在构造函数中给出默认值，设置参数需要刷新，右侧
{
    float exposure_time;
    float gain;
    int white_balance_ratio;
    //... other params
};
struct Camera_paramTop      //相机参数在此添加，在构造函数中给出默认值，设置参数需要刷新，顶部
{
    float exposure_time;
    float gain;
    int white_balance_ratio;
    //... other params
};
class CameraL : public QObject
{
    Q_OBJECT
private:
    void* camera_handle;       //设备句柄
    void* camera_handle1;
    void* camera_handle2;
    MV_CC_DEVICE_INFO_LIST device_list;    //设备列表
    Camera_param camera_param;
    unsigned int ip4;                      //用于保存ipv4地址的第4位，即192.168.0.100中的100
    int i;
public:
    explicit CameraL(QObject *parent = nullptr);
    bool initCameraL();
    uint32_t enum_device();
    bool print_device_info();
    bool select_device();


    bool destroy_handle();
    bool destroy_handle1();
    bool destroy_handle2();



    bool open_camera();
    bool start_capture();
    bool close_camera();
    bool set_acquisition_mode();
    bool set_test_acquisition_mode();
    bool set_ROI(int offset_x, int offset_y, int width, int height);
    bool stop_capture();
    bool register_image_callback(imageCallbackFunc onImageDataCallBackFunc,imageCallbackFunc onImageDataCallBackFunc1,imageCallbackFunc onImageDataCallBackFunc2);
    bool register_offline_callback(exceptionCallbackFunc onOfflineCallBackFunc,exceptionCallbackFunc onOfflineCallBackFunc1,exceptionCallbackFunc onOfflineCallBackFunc2);

    bool import_config_file();
    bool save_config_file();
    bool set_param(Camera_param value);
    Camera_param get_param();
    std::queue<QImage> leftImgQueue;
    std::queue<QImage> rightImgQueue;
    std::queue<QImage> topImgQueue;
    std::queue<QImage> topImgTQueue;
    std::queue<QImage> topImgMQueue;
    std::queue<QImage> topImgBQueue;
    std::queue<tomato> tomatoQueue;


signals:
//    void send_RgbimageL(cv::Mat img,unsigned int count);
    void send_Rgbimage(QImage& img);
    void send_Rgbimage1(QImage& img);

    void send_Rgbimage2(QImage& img,QImage& img1,QImage& img2);

    void send_camerastatus(int ret,int ret1,int ret2);




public slots:
};


class SpecCamera : public QObject
{
    Q_OBJECT
public:
    explicit SpecCamera(QObject *parent = nullptr);
    bool init_SpecCamera();
    bool cal(unsigned char* pData);
    bool stop_capture();
    bool trigger_open();
    bool trigger_close();
    int m_iWidth;				//相机采集数据宽度
    int m_iHeight;				//相机采集数据高度
    unsigned short* specfullImage;
    const int totalFrames = 100;   // 假设需要拼接50帧来形成一张图
    // std::vector<unsigned short*> frameData;
    int vector_flag;
    std::vector<unsigned short*> frameData;
    std::vector<unsigned short*> frameData_copy;
    int test_count = 0;
    //CameraSetting
    float m_fWaveLengthMin;		//起点光谱波长
    float m_fWaveLengthMax;		//终点光谱波长
    float m_fExpTime;			//曝光时间
    float m_fGain;				//增益

    float m_iFrameRate;			//帧速

    //GenralSetting
    int m_cslRed;				//红色波长序号
    int m_cslGreen;				//绿色波长序号
    int m_cslBlue;				//蓝色波长序号
    unsigned int m_uiDepth;

    unsigned short* m_pusSaveBuffer;
    unsigned char* m_pRefreshBuffer;    //RGB

    unsigned char* m_specBuffer;   //光谱

    long m_iDrawCount = 0;


    bool start_capture();
    std::queue<unsigned short*> specImgQueue;


private:


signals:
    void show_SpecImg(QImage);
    void process_spec(unsigned short*);


};
#endif // CAMERA_H
