#include "widget.h"
#include "ui_widget.h"
#include "camera.h"
#include <QDateTime>
#include "QSemaphore"
#include "vector"
#include <QTcpServer>
#include <QTcpSocket>
#define sys_file2 "./sys_daytime2.txt"
//外部引用
extern ProcessImg *processimg;
extern SendThread *sendthread;
extern RecvThread *recvthread;

extern CameraL *cameraL;
extern SpecCamera *speccamera;

extern QSemaphore emptybuff;
extern tomato tomato;

// c++的省略命名申明
using namespace std;
bool is_timeout;

int start_flag=1;
int save_flag=0;

//0为番茄 1为百香果
int fruit_flag;

int resoult_count = 1;

int i = 1;



//外部用用
extern int camStatusret;
extern int camStatusret1;
extern int camStatusret2;

extern int img_count;
extern int img_count1;
extern int img_count2;
//定义管道名称
extern HANDLE hPipe;
extern HANDLE specPipe;
extern HANDLE RPipe;
extern QString pipeName;
extern QString pipeName2;
extern QString rePipe;

int pipe_counter=1;


//这是 Widge 的构造函数 在你点击运行之后 会执行
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //直接导入本地时间（方法二）

    QFile daytime_file(sys_file2);
    QDateTime lastDateTime;
    if (daytime_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString line;
        QTextStream in(&daytime_file);
        while (!in.atEnd()) {
            line = in.readLine();
        }
        QStringList parts = line.split(" - ");
        if (parts.size() > 0) {
            lastDateTime = QDateTime::fromString(parts.first(), "yyyy-MM-dd HH:mm:ss");
        }
        daytime_file.close();
    }
    if (lastDateTime.isValid() && lastDateTime > QDateTime::currentDateTime()) {
        QMessageBox::about(this, "错误", "时间校验不通过");
        throw ExpiredException();
    }
    QDateTime expiryDate = QDateTime::fromString("2024-08-01 14:00:00", "yyyy-MM-dd HH:mm:ss");
    QDateTime currentDate = QDateTime::currentDateTime();
    if (currentDate >= expiryDate) {

        throw ExpiredException();
    }

    //处理光谱数据的线程
    processimg = new ProcessImg();
    //发送给python端线程
    sendthread = new SendThread();
    //接收python端线程
    recvthread = new RecvThread();
    //new 代表给三个线程开辟空间

    initparam();

    /**连接相机*/
    cameraL = new CameraL();
    //初始化RGB相机
    bool RGB_ret = cameraL->initCameraL();

    if(camStatusret ==0 && RGB_ret)
    {
        ui->RGBstatus->setText("已连接");
        ui->RGBstatus->setStyleSheet("QLabel{color:rgb(0,255,0);}");
    }
    if(camStatusret1 ==0 && RGB_ret)
    {
        ui->RGBstatus_1->setText("已连接");
        ui->RGBstatus_1->setStyleSheet("QLabel{color:rgb(0,255,0);}");

    }
    if(camStatusret2 ==0 && RGB_ret)
    {
        ui->RGBstatus_2->setText("已连接");
        ui->RGBstatus_2->setStyleSheet("QLabel{color:rgb(0,255,0);}");
    }

    //初始化光谱相机
    speccamera = new SpecCamera();
    bool spec_ret = speccamera->init_SpecCamera();

    if(spec_ret == true)
    {
        ui->spec_camera_status->setText("已连接");
        ui->spec_camera_status->setStyleSheet("QLabel{color:rgb(0,255,0);}");
    }

    //初始化称重模块
    connectModbus_CZ();

    //初始化机械手
    connect_socket_Jxs();

    //管道建立连接
    connectPipe();
}

Widget::~Widget()
{
    delete ui;
}

//初始话参数 并非实际相机的参数 而是 ui 界面上的 tab3 上的控件参数
void Widget::initparam()
{
    camStatusret=-1;
    camStatusret1=-1;
    camStatusret2=-1;
    ui->tabWidget->setCurrentIndex(2);
    setMinimumSize(QSize(1200, 600));
    setWindowState(Qt::WindowMaximized);
}

void Widget::connectModbus_CZ()
{
    //称重模块
    CZ = modbus_new_tcp("192.168.101.110",2000);
    modbus_set_slave(CZ, 1); //设置modbus从机地址
    int status = modbus_connect(CZ);
    if(status == -1)
    {
        qDebug() << "modbus connect failed";
    }
    else
    {
        ui->plc_status->setStyleSheet("QLabel{color:rgb(0,255,0);}");
        ui->plc_status->setText("已连接");
    }
    modbus_set_response_timeout(CZ, 0, 1000000);
}

void Widget::connect_socket_Jxs()
{
    server = new QTcpServer();
    server->listen(QHostAddress::Any, 21123);

    bool is_timeout;
    server->waitForNewConnection(5000, &is_timeout);

    if(is_timeout == true)
    {
        qDebug() << "Jxs connect failed";
        return;
    }

    socket = server->nextPendingConnection();

    ui->jxs_status->setStyleSheet("QLabel{color:rgb(0,255,0);}");
    ui->jxs_status->setText("已连接");
}

void Widget::connectPipe()
{

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


    // 创建并连接到命名管道
    RPipe = CreateFile(
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

}


//点击 ui 界面的开始按钮
void Widget::on_btn_start_clicked()
{
    //start_flag 为 1
    if(start_flag)
    {
        QFile file(sys_file2);
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);
            out << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") << " - "  << "\n";
            file.close();
        }
        ui->btn_start->setStyleSheet("background-color: red;");

        cameraL->open_camera2();
        cameraL->set_acquisition_mode();
        cameraL->start_capture();
        speccamera->start_capture();

        //开启线程 也就是执行一次对应线程的 run() 函数 在 thread 里面有  即执行三个线程的 while（1） 死循环 一直监听
        processimg->start();
        sendthread->start();
        recvthread->start();

        //连接信号跟槽函数
        connect(cameraL,SIGNAL(send_Rgbimage(QImage&)),this,SLOT(showimg(QImage&)),Qt::BlockingQueuedConnection);
        connect(cameraL,SIGNAL(send_Rgbimage1(QImage&)),this,SLOT(showimg1(QImage&)),Qt::BlockingQueuedConnection);
        connect(cameraL,SIGNAL(send_Rgbimage2(QImage&,QImage&,QImage&)),this,SLOT(showimg2(QImage&,QImage&,QImage&)),Qt::BlockingQueuedConnection);
        connect(speccamera,SIGNAL(show_SpecImg(QImage)),this,SLOT(show_specImgSlots(QImage)));
        connect(recvthread,SIGNAL(recv_Data(quint16,quint8,quint16,quint8,quint16,quint32,quint16,quint16,QImage)),this,SLOT(receiveDataSlots(quint16,quint8,quint16,quint8,quint16,quint32,quint16,quint16,QImage)));



        start_flag=0;

        ui->tabWidget->setCurrentIndex(3);

        // qDebug()<<"Start Success!";
    }

    //start_flag 为 0
    else
    {
        QFile file(sys_file2);
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);
            out << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") << " - "  << "\n";
            file.close();
        }
        ui->btn_start->setStyleSheet("background-color: white;");
        cameraL->stop_capture();
        speccamera->stop_capture();

        disconnect(cameraL,SIGNAL(send_Rgbimage(QImage&)),this,SLOT(showimg(QImage&)));
        disconnect(cameraL,SIGNAL(send_Rgbimage1(QImage&)),this,SLOT(showimg1(QImage&)));
        disconnect(cameraL,SIGNAL(send_Rgbimage2(QImage&,QImage&,QImage&)),this,SLOT(showimg2(QImage&,QImage&,QImage&)));       
        disconnect(speccamera,SIGNAL(show_SpecImg(QImage)),this,SLOT(show_specImgSlots(QImage)));
        disconnect(recvthread,SIGNAL(recv_Data(quint16,quint8,quint16,quint8,quint16,quint32,quint16,quint16,QImage)),this,SLOT(receiveDataSlots(quint16,quint8,quint16,quint8,quint16,quint32,quint16,quint16,QImage)));
        ui->showimg_left->clear();
        ui->showimg_right->clear();
        ui->label_topL->clear();
        ui->label_topM->clear();
        ui->label_topR->clear();

        start_flag=1;

        // qDebug() << "Stop Success!";
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

//顶部
void Widget::showimg2(QImage& img,QImage& img1,QImage& img2)
{
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


//退出
void Widget::on_btn_quit_clicked()
{
    qApp->quit();
}


//保存图片按钮
void Widget::on_btn_save_clicked()
{
    if(!save_flag)
    {
        ui->btn_save->setStyleSheet("background-color: red;");
        save_flag=1;
    }
    else
    {
        ui->btn_save->setStyleSheet("background-color: white;");
        save_flag=0;
    }
}


void Widget::on_btn_setparam_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}

//导入参数 实际没用 因为相机参数会预先写死 并不会在运行途中更换
void Widget::on_btn_enterparam_clicked()
{

}



//接受线程收到数据 长径、短径等等 在 ui 界面显示
void Widget::receiveDataSlots(quint16 brix,quint8 greenPercentage,quint16 diameter,quint8 weight,quint16 defectNum,quint32 totalDefectArea,quint16 height,quint16 width,QImage img)
{
    img = img.rgbSwapped();

    pix = QPixmap::fromImage(img.scaled(ui->show_resultsImg->width(),ui->show_resultsImg->height(),Qt::KeepAspectRatio));
    ui->show_resultsImg->setScaledContents(true);
    ui->show_resultsImg->setPixmap(pix);
    ui->show_resultsImg->show();

    float brix_float = brix;
    float totalDefectArea_float = totalDefectArea;
    float greenPercentage_float = greenPercentage;
    float diameter_float = diameter;
    // ui->Tdangdu_line->setText(QString::number(brix_float/1000)); //
    // ui->Zhijing_line->setText(QString::number(diameter_float/100)); //cm
    // ui->Shuliang_line->setText(QString::number(defectNum));
    // ui->Mianji_line->setText(QString::number(totalDefectArea_float/1000)); //cm^2
    // ui->Zhanbi_line->setText(QString::number(greenPercentage_float/100)); //

    //重量是g
    //读取称重数据
    uint16_t read = 0;
    modbus_read_registers(CZ,0x40000,1,&read);
    qDebug()<<"称重数据"<<read;

    int level = 5;
    //百香果
    if(fruit_flag)
    {
        if(diameter = 0)
        {
            level = 4;
        }
        else
        {
            if(defectNum = 0)
            {
                level = 4;
            }
            else
            {
                if(totalDefectArea_float/1000 > 2)
                {
                    level = 4;
                }
                else
                {
                    if(read > 90)
                    {
                        level = 1;
                    }
                    else if(read < 65)
                    {
                        level = 3;
                    }
                    else
                    {
                        level = 2;
                    }
                }
            }
        }

    }
    //番茄
    else
    {
        if(diameter == 0)
        {
            level = 5;
        }
        else
        {
            if(defectNum != 0)
            {
                level = 5;
            }
            else
            {
                if(totalDefectArea_float/1000 >= Quexianmianji)
                {
                    level = 5;
                }
                else
                {
                    if(greenPercentage_float/100 >= Yansezhanbi)
                    {
                        if(diameter_float/100 >= Zhijing -2)
                        {
                            level = 3;
                        }
                        else
                        {
                            level = 4;
                        }
                    }
                    else
                    {
                        if(diameter_float/100 >= Zhijing)
                        {
                            level = 1;
                        }
                        else
                        {
                            level = 2;
                        }
                    }
                }
            }
        }
    }

    // ui->resoult_line->setText(QString::number(level));
    qDebug()<<"果子等级为:"<<level;

    //窗口显示

    if(resoult_count%25 == 0)
    {
        ui->resoult_info_label->clear();
    }

    QString resout_info;
    QString fruit_resoult_flag;
    //百香果
    if(fruit_flag)
    {
        if(defectNum == 0)
        {
            fruit_resoult_flag =  "有";
        }
        else
        {
            fruit_resoult_flag =  "无";
        }
        resout_info = QString("当前百香果编号为：%1 糖度值为： %2 直径为： %3  %4皱缩  缺陷面积为： %5  最终识别结果为：%6 ").arg(resoult_count).arg(brix_float/1000).arg(diameter_float/100).arg(fruit_resoult_flag).arg(totalDefectArea_float/1000).arg(level);
        ui->resoult_info_label->appendPlainText(resout_info);
    }
    //番茄
    else
    {
        if(defectNum == 0)
        {
            fruit_resoult_flag = "无";
        }
        else
        {
            fruit_resoult_flag =  "有";
        }
        resout_info = QString("当前番茄编号为：%1 直径为： %2  %3裂口  缺陷面积为： %4 绿色占比为： %5 最终识别结果为：%6 ").arg(resoult_count).arg(diameter_float/100).arg(fruit_resoult_flag).arg(totalDefectArea_float/1000).arg(greenPercentage_float/100).arg(level);
        ui->resoult_info_label->appendPlainText(resout_info);
    }

    resoult_count++ ;
    QFile file("./image/output.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        QTextStream out(&file);
        out << resout_info << "\n";
        file.close();
    }
    i++;

    // 给机械手发送信息
    QString txt = QString("[X:0;Y:0;A:0;ATTR:%1;ID:%2]").arg(level).arg(fruit_flag);
    QByteArray byteArray = txt.toUtf8();
    socket->write(byteArray);
    // qDebug()<<"发送机械手信息等级为:"<<level;

    //给plc发送信息
    uint16_t write[2];
    write[0] = resoult_count;
    write[1] = level;
    modbus_write_registers(CZ,0x40001,2,write);
    // qDebug()<<"发送plc信息等级为:"<<level;



    //称重数据在 read 里面



}


//显示光谱伪彩色图 实际效果很差 就取消了
//展示一条折线
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


void Widget::on_view_results_clicked()
{
    QString  resoult_path = "C:/Users/succtech/Desktop/20240425/image";
    // 打开文件选择对话框，并设置默认路径
    QString filePath = QFileDialog::getOpenFileName(this, "查看结果", resoult_path);
}

void Widget::on_tab4_ReturnToMain_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}

void Widget::on_choose_pa_clicked()
{
    fruit_flag = 1;
    //tab2
    ui->passionBtn->setStyleSheet("background-color: red;");
    ui->tomatoBtn->setStyleSheet("background-color: white;");

    //tab3
    ui->choose_pa->setStyleSheet("background-color: red;");
    ui->choose_to->setStyleSheet("background-color: white;");

    //tab4
    ui->choose_pa_2->setStyleSheet("background-color: red;");
    ui->choose_to_2->setStyleSheet("background-color: white;");

    ui->tabWidget->setCurrentIndex(1);
}

void Widget::on_choose_to_clicked()
{
    fruit_flag = 0;
    //tab2
    ui->tomatoBtn->setStyleSheet("background-color: red;");
    ui->passionBtn->setStyleSheet("background-color: white;");

    //tab3
    ui->choose_to->setStyleSheet("background-color: red;");
    ui->choose_pa->setStyleSheet("background-color: white;");

    //tab4
    ui->choose_to_2->setStyleSheet("background-color: red;");
    ui->choose_pa_2->setStyleSheet("background-color: white;");

    ui->tabWidget->setCurrentIndex(1);
}

void Widget::on_tab3_ReturnToMain_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}

void Widget::on_show_resoult_clicked()
{
    ui->tabWidget->setCurrentIndex(3);
}

void Widget::on_set_threshoid_clicked()
{
    Tangdu = ui->InputNum1->text().toInt();
    Zhijing = ui->InputNum2->text().toInt();
    Yansezhanbi = ui->InputNum3->text().toInt();
    Quexianshuliang = ui->InputNum4->text().toInt();
    Quexianmianji = ui->InputNum5->text().toInt();
}

