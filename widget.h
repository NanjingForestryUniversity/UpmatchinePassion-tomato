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
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void initparam();
    void connectModbus_CZ();


private slots:
    void on_btn_start_clicked();

    void showimg1(QImage&);
    void showimg(QImage&);
    void showimg2(QImage&,QImage&,QImage&);


    void on_save_img_clicked();

    void on_btn_PWenter_clicked();

    void on_btn_quit_clicked();

    void on_btn_quit2_clicked();

    void on_btn_save_clicked();

    void on_btn_setparam_clicked();

    void on_btn_enterparam_clicked();

    void on_btn_return_clicked();

    void on_passionBtn_clicked();

    void on_tomatoBtn_clicked();


    void receiveDataSlots(quint16 longD,quint16 shortD,quint16 QNum,quint32 ,QImage img);

    void show_specImgSlots(QImage);

private:
    Ui::Widget *ui;
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
