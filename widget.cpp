#include "widget.h"
#include "ui_widget.h"
#include "camera.h"
#include <QDateTime>
#include "QSemaphore"
#include "vector"
extern ProcessImg *processimg;
extern SendThread *sendthread;
extern RecvThread *recvthread;
using namespace std;
extern CameraL *cameraL;

extern SpecCamera *speccamera;

extern QSemaphore emptybuff;

bool is_timeout;
extern tomato tomato;
//起动标志位
bool change_passionFlag = 0;
bool change_tomatoFlag = 0;
int start_flag=1;
int save_flag=0;
extern int camStatusret;
extern int camStatusret1;
extern int camStatusret2;
extern int img_count;
extern int img_count1;

extern int img_count2;


int pipe_counter=1;
modbus_t* CZ;
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    processimg = new ProcessImg();
    sendthread = new SendThread();
    recvthread = new RecvThread();


    // connectModbus_CZ();
    initparam();

    /**连接相机*/


    cameraL = new CameraL();
    bool ret = cameraL->initCameraL();


    if(camStatusret==0){
        ui->RGBstatus->setText("已连接");
        ui->RGBstatus->setStyleSheet("QLabel{color:rgb(0,255,0);}");
    }
    if(camStatusret1==0){
        ui->RGBstatus_1->setText("已连接");
        ui->RGBstatus_1->setStyleSheet("QLabel{color:rgb(0,255,0);}");

    }
    if(camStatusret2==0){
        ui->RGBstatus_2->setText("已连接");
        ui->RGBstatus_2->setStyleSheet("QLabel{color:rgb(0,255,0);}");
    }
    speccamera = new SpecCamera();

    bool ret3 = speccamera->init_SpecCamera();


    if(ret3==true){
        ui->spec_camera_status->setText("已连接");
        ui->spec_camera_status->setStyleSheet("QLabel{color:rgb(0,255,0);}");
    }



}

Widget::~Widget()
{
    delete ui;

    // if(sendthread != nullptr)
    // {
    //     delete sendthread;
    //     sendthread = nullptr;
    // }

    // if(recvthread != nullptr)
    // {
    //     delete recvthread;
    //     recvthread = nullptr;
    // }
    // if(processimg != nullptr)
    // {
    //     delete processimg;
    //     processimg = nullptr;
    // }


}

void Widget::initparam()
{
    camStatusret=-1;
    camStatusret1=-1;
    camStatusret2=-1;
    ui->tabWidget->setCurrentIndex(1);
    setMinimumSize(QSize(1200, 600));
    setWindowState(Qt::WindowMaximized);
    //初始化第一个相机参数
    ui->exSpinBox_left->setRange(0, 9999500.00);
    ui->exSpinBox_left->setSingleStep(0.01);
    ui->GainSpinBox_left->setRange(0, 16.98);
    ui->GainSpinBox_left->setSingleStep(0.01);
    ui->wbSpinBox_left_2->setRange(0, 4095);

    //初始化第二个相机参数
    ui->exspinBox_right->setRange(0, 9999500.00);
    ui->exspinBox_right->setSingleStep(0.01);
    ui->GainspinBox_right->setRange(0, 16.98);
    ui->GainspinBox_right->setSingleStep(0.01);
    ui->wbspinBox_right->setRange(0, 4095);

    //初始化第三个相机参数


    ui->exspinBox_top->setRange(0, 9999500.00);
    ui->exspinBox_top->setSingleStep(0.01);
    ui->GainspinBox_top->setRange(0, 16.98);
    ui->GainspinBox_top->setSingleStep(0.01);
    ui->wbspinBox_top->setRange(0, 4095);
}

void Widget::connectModbus_CZ()
{
    //称重模块
    int status = -1;
    CZ = modbus_new_rtu("COM7", 9600, 'N', 8, 1);
    modbus_set_slave(CZ, 3); //设置modbus从机地址
    status = modbus_connect(CZ);
    if(status == -1)
    {
        qDebug() << "modbus connect failed";
        ui->lowermachine_status->setText("未连接");
        ui->lowermachine_status->setStyleSheet("QLabel{color:rgb(255,0,0);}");
    }
    else
    {
        qDebug() << "status" << status;
        qDebug() << "connect plc success";
        ui->lowermachine_status->setStyleSheet("QLabel{background-color:rgb(0,255,0);}");
        ui->lowermachine_status->setText("已连接");
    }
    modbus_set_response_timeout(CZ, 0, 1000000);
}





void Widget::on_btn_start_clicked()
{


    if(start_flag){

        processimg->start();
        // sendthread->start();
        // recvthread->start();

        speccamera->start_capture();
        connect(cameraL,SIGNAL(send_Rgbimage(QImage&)),this,SLOT(showimg(QImage&)),Qt::BlockingQueuedConnection);
        connect(cameraL,SIGNAL(send_Rgbimage1(QImage&)),this,SLOT(showimg1(QImage&)),Qt::BlockingQueuedConnection);
        connect(cameraL,SIGNAL(send_Rgbimage2(QImage&,QImage&,QImage&)),this,SLOT(showimg2(QImage&,QImage&,QImage&)),Qt::BlockingQueuedConnection);
        // connect(speccamera,SIGNAL(show_SpecImg(QImage)),this,SLOT(show_specImgSlots(QImage)));
        connect(recvthread,SIGNAL(recv_Data(quint16,quint16,quint16,quint32,QImage)),this,SLOT(receiveDataSlots(quint16,quint16,quint16,quint32,QImage)));

        cameraL->set_acquisition_mode();
        cameraL->start_capture();
        ui->btn_start->setText("停止采集");
        start_flag=0;
    }else{
        cameraL->stop_capture();
        disconnect(cameraL,SIGNAL(send_Rgbimage(QImage&)),this,SLOT(showimg(QImage&)));
        disconnect(cameraL,SIGNAL(send_Rgbimage1(QImage&)),this,SLOT(showimg1(QImage&)));
        disconnect(cameraL,SIGNAL(send_Rgbimage2(QImage&,QImage&,QImage&)),this,SLOT(showimg2(QImage&,QImage&,QImage&)));
        speccamera->stop_capture();
        /*
         * 清除图片显示
         * */
        ui->showimg_left->clear();
        ui->showimg_right->clear();
        ui->label_topL->clear();
        ui->label_topM->clear();
        ui->label_topR->clear();


        qDebug() << "Stop Success!";


        ui->btn_start->setText("开始采集");
        start_flag=1;
    }

}

//右侧
void Widget::showimg(QImage& img)
{

    pix = QPixmap::fromImage(img.scaled(ui->showimg_left->width(),ui->showimg_left->height(),Qt::KeepAspectRatio));
    ui->showimg_left->setScaledContents(true);
    ui->showimg_left->setPixmap(pix);
    ui->showimg_left->show();
}


//左侧
void Widget::showimg1(QImage& img)
{

    pix = QPixmap::fromImage(img.scaled(ui->showimg_right->width(),ui->showimg_right->height(),Qt::KeepAspectRatio));
    ui->showimg_right->setScaledContents(true);
    ui->showimg_right->setPixmap(pix);
    ui->showimg_right->show();
}

/*
 * 顶部相机槽函数
 * */
void Widget::showimg2(QImage& img,QImage& img1,QImage& img2)
{
    // pipe_counter++;
    // qDebug()<<"pipe_counter: "<<pipe_counter;
    pix = QPixmap::fromImage(img.scaled(ui->label_topL->width(),ui->label_topL->height(),Qt::KeepAspectRatio));
    ui->label_topL->setScaledContents(true);
    ui->label_topL->setPixmap(pix);
    ui->label_topL->show();

    pix1 = QPixmap::fromImage(img1.scaled(ui->label_topM->width(),ui->label_topM->height(),Qt::KeepAspectRatio));
    ui->label_topM->setScaledContents(true);
    ui->label_topM->setPixmap(pix1);
    ui->label_topM->show();

    pix2 = QPixmap::fromImage(img2.scaled(ui->label_topR->width(),ui->label_topR->height(),Qt::KeepAspectRatio));
    ui->label_topR->setScaledContents(true);
    ui->label_topR->setPixmap(pix2);
    ui->label_topR->show();
}

void Widget::on_save_img_clicked()
{
    save_flag=1;
}


void Widget::on_btn_PWenter_clicked()
{
    QString password = "110";
    QString input = ui->lineEdit_password->text();
    if(input==password&&ui->lineEdit_user->text()!=""){
        ui->tabWidget->setCurrentIndex(1);
    }else if(input==password&&ui->lineEdit_user->text()==""){
        QMessageBox::information(this,"提示","请输入操作人员！");
    }else{
        QMessageBox::information(this,"提示","密码错误！");
        input.clear();
        ui->lineEdit_password->text();
    }
}


void Widget::on_btn_quit_clicked()
{
    qApp->quit();
}


void Widget::on_btn_quit2_clicked()
{
    qApp->quit();
}


void Widget::on_btn_save_clicked()
{
    if(!save_flag){
        ui->btn_save->setText("不保存图片");
        save_flag=1;
    }else{
        ui->btn_save->setText("保存图片");
        save_flag=0;
    }
}


void Widget::on_btn_setparam_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}


void Widget::on_btn_enterparam_clicked()
{
//    //left
//    Camera_param setL;
//    setL.exposure_time = ui->exSpinBox_left->text().toFloat();
//    setL.gain = ui->GainSpinBox_left->text().toDouble();
//    setL.white_balance_ratio = ui->wbSpinBox_left_2->text().toInt();


//    cameraL->set_param(setL);

//    cameraL->stop_capture();
//    usleep(1000);

//    cameraL->get_param();
//    if(!cameraL->save_config_file())
//    {
//        qDebug()<<"SAVE FAILED";
//    }
//    //right
//    Camera_paramR setsR;
//    setsR.exposure_time = ui->exspinBox_right->text().toFloat();
//    setsR.gain = ui->GainspinBox_right->text().toDouble();
//    setsR.white_balance_ratio = ui->wbspinBox_right->text().toInt();

//    qDebug()<<setsR.exposure_time;
//    cameraR->set_param2(setsR);

//    cameraR->stop_capture2();
//    usleep(1000);

//    cameraR->get_param2();
//    if(!cameraR->save_config_file2())
//    {
//        qDebug()<<"SAVE FAILED";
//    }

//    //top
//    Camera_paramTop settop;
//    settop.exposure_time = ui->exspinBox_top->text().toFloat();
//    settop.gain = ui->GainspinBox_top->text().toDouble();
//    settop.white_balance_ratio = ui->wbspinBox_top->text().toInt();


//    cameratop->set_param(settop);

//    cameratop->stop_capture();
//    usleep(1000);

//    cameratop->get_param();
//    if(!cameratop->save_config_file())
//    {
//        qDebug()<<"SAVE FAILED";
//    }
//    qDebug()<<"设置成功";
}


void Widget::on_btn_return_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}





void Widget::on_passionBtn_clicked()  //切换百香果模型
{
    change_passionFlag = 1;
    change_tomatoFlag = 0;
    uint8_t send_buf_switch[13];
    send_buf_switch[0] = 0xAA;
    send_buf_switch[1] = 0x00;
    send_buf_switch[2] = 0x00;
    send_buf_switch[3] = 0x00;
    send_buf_switch[4] = 0x05;
    send_buf_switch[5] = ' ';
    send_buf_switch[6] = ' ';
    send_buf_switch[7] = 'P';
    send_buf_switch[8] = 'A';
    send_buf_switch[9] = 0xFF;
    send_buf_switch[10] = 0xFF;
    send_buf_switch[11] = 0xFF;
    send_buf_switch[12] = 0xBB;
    // sendthread->send_socket->write((const char*)send_buf_switch,sizeof(send_buf_switch));
}


void Widget::on_tomatoBtn_clicked()  //切换番茄模型
{
    change_tomatoFlag = 1;
    change_passionFlag = 0;
    uint8_t send_buf_switch[13];
    send_buf_switch[0] = 0xAA;
    send_buf_switch[1] = 0x00;
    send_buf_switch[2] = 0x00;
    send_buf_switch[3] = 0x00;
    send_buf_switch[4] = 0x05;
    send_buf_switch[5] = ' ';
    send_buf_switch[6] = ' ';
    send_buf_switch[7] = 'T';
    send_buf_switch[8] = 'O';
    send_buf_switch[9] = 0xFF;
    send_buf_switch[10] = 0xFF;
    send_buf_switch[11] = 0xFF;
    send_buf_switch[12] = 0xBB;
    // sendthread->send_socket->write((const char*)send_buf_switch,sizeof(send_buf_switch));
}



void Widget::receiveDataSlots(quint16 longD, quint16 shortD, quint16 QNum, quint32 QArea,QImage img)
{
    img = img.rgbSwapped();

    pix = QPixmap::fromImage(img.scaled(ui->show_resultsImg->width(),ui->show_resultsImg->height(),Qt::KeepAspectRatio));
    ui->show_resultsImg->setScaledContents(true);
    ui->show_resultsImg->setPixmap(pix);
    ui->show_resultsImg->show();
    ui->longdia_linedit->setText(QString::number(longD));
    ui->shortdia_linedit->setText(QString::number(shortD));
    ui->QueNum_linedit->setText(QString::number(QNum));
    ui->QArea_linedit->setText(QString::number(QArea));

}

void Widget::show_specImgSlots(QImage img)
{
    // qDebug()<<"2";
    QTransform transform;
    transform.rotate(90.0);
    img = img.transformed(transform, Qt::FastTransformation);

    pix = QPixmap::fromImage(img.scaled(ui->spec_camera_show->width(),ui->spec_camera_show->height(),Qt::KeepAspectRatio));
    ui->spec_camera_show->setScaledContents(true);
    ui->spec_camera_show->setPixmap(pix);
    ui->spec_camera_show->show();
}

