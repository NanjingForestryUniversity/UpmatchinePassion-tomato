#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include "QMessageBox"
#include "thread.h"
#include <unistd.h>
#include "QMessageBox"
#include "src/modbus.h"

#include <QImage>
#define SAVE_IMAGE_PATH "F:/tomato/tomato1227/image/left"
QT_BEGIN_NAMESPACE



//ui界面的类 窗口的各种控件（按钮、文字输入行、label等）
namespace Ui { class Widget; }
QT_END_NAMESPACE
class ExpiredException : public std::exception {
public:
    const char* what() const throw() {
        return "程序试用期已过，请联系供应商！";
    }
};
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    int Tangdu;
    int Zhijing = 7;
    float Yansezhanbi = 0.3;
    int Quexianshuliang = 0;
    float Quexianmianji = 0.5;

    void initparam();



    void connectModbus_CZ();

    void connect_socket_Jxs();
    void connectPipe();

    QTcpServer *server;
    QTcpSocket *socket;

    modbus_t* CZ;

private slots:
    void on_btn_start_clicked();

    void showimg1(QImage&);
    void showimg(QImage&);
    void showimg2(QImage&,QImage&,QImage&);


    void on_save_img_clicked();

    void on_btn_quit_clicked();

    void on_btn_save_clicked();

    void on_btn_setparam_clicked();

    void on_btn_enterparam_clicked();

    void receiveDataSlots(quint16,quint8,quint16,quint8,quint16,quint32,quint16,quint16,QImage);

    void show_specImgSlots(QImage);

    void on_view_results_clicked();

    void on_tab4_ReturnToMain_clicked();

    void on_choose_pa_clicked();

    void on_choose_to_clicked();

    void on_tab3_ReturnToMain_clicked();

    void on_show_resoult_clicked();

    void on_set_threshoid_clicked();

private:
    Ui::Widget *ui;
    //画板 负责接收QImage并显示 不能直接显示QImage变量
    QPixmap pix;
    QPixmap pix1;
    QPixmap pix2;

    int m_index;



    //存图标志位
    int save_img=0;

    //tcp变量

    QTcpServer* server_to_lowermachine = nullptr;
    QTcpSocket* lower_machine = nullptr;
    volatile bool is_running = false;



};
#endif // WIDGET_H
