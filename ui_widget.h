/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QGridLayout *gridLayout_10;
    QTabWidget *tabWidget;
    QWidget *tab_3;
    QGridLayout *gridLayout_12;
    QLabel *label;
    QLabel *label_23;
    QWidget *tab;
    QGroupBox *groupBox_9;
    QGridLayout *gridLayout_11;
    QGroupBox *groupBox;
    QLabel *label_14;
    QWidget *layoutWidget_2;
    QHBoxLayout *horizontalLayout_14;
    QLabel *label_RGB;
    QLabel *RGBstatus;
    QLabel *label_RGB_3;
    QLabel *RGBstatus_1;
    QLabel *label_RGB_2;
    QLabel *RGBstatus_2;
    QLabel *label_9;
    QLabel *spec_camera_status;
    QLabel *label_11;
    QLabel *plc_status;
    QLabel *label_17;
    QLabel *jxs_status;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_5;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_8;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_26;
    QPushButton *passionBtn;
    QPushButton *tomatoBtn;
    QPushButton *btn_start;
    QPushButton *show_resoult;
    QPushButton *btn_setparam;
    QPushButton *btn_save;
    QPushButton *view_results;
    QPushButton *btn_quit;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
    QLabel *label_topL;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_3;
    QLabel *label_topM;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_4;
    QLabel *label_topR;
    QGroupBox *groupBox_6;
    QGridLayout *gridLayout_5;
    QLabel *showimg_right;
    QGroupBox *groupBox_7;
    QGridLayout *gridLayout_6;
    QLabel *showimg_left;
    QGroupBox *groupBox_8;
    QGridLayout *gridLayout_7;
    QLabel *spec_camera_show;
    QWidget *tab_2;
    QGroupBox *groupBox_11;
    QGridLayout *gridLayout_13;
    QLabel *label_28;
    QSpinBox *exSpinBox_left;
    QLabel *label_29;
    QSpinBox *wbSpinBox_left_2;
    QLabel *label_30;
    QSpinBox *GainSpinBox_left;
    QGroupBox *groupBox_13;
    QGridLayout *gridLayout_16;
    QLabel *label_38;
    QSpinBox *exspinBox_top;
    QLabel *label_39;
    QSpinBox *wbspinBox_top;
    QLabel *label_40;
    QSpinBox *GainspinBox_top;
    QGroupBox *groupBox_15;
    QGridLayout *gridLayout_18;
    QLabel *label_46;
    QSpinBox *exspinBox_right;
    QLabel *label_47;
    QSpinBox *wbspinBox_right;
    QLabel *label_48;
    QSpinBox *GainspinBox_right;
    QPushButton *btn_enterparam;
    QPushButton *tab3_ReturnToMain;
    QPushButton *choose_to;
    QPushButton *choose_pa;
    QLineEdit *InputNum1;
    QLineEdit *InputNum5;
    QLabel *label_5;
    QLineEdit *InputNum4;
    QPushButton *set_threshoid;
    QLabel *label_32;
    QLineEdit *InputNum3;
    QLabel *label_33;
    QLabel *label_10;
    QLabel *label_18;
    QLineEdit *InputNum2;
    QLabel *label_12;
    QLabel *label_21;
    QWidget *tab_4;
    QLabel *show_resultsImg;
    QLabel *label_3;
    QPushButton *tab4_ReturnToMain;
    QPlainTextEdit *resoult_info_label;
    QPushButton *choose_pa_2;
    QLabel *label_13;
    QPushButton *choose_to_2;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(2000, 1038);
        gridLayout_10 = new QGridLayout(Widget);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        tabWidget = new QTabWidget(Widget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setStyleSheet(QString::fromUtf8(""));
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        gridLayout_12 = new QGridLayout(tab_3);
        gridLayout_12->setObjectName(QString::fromUtf8("gridLayout_12"));
        label = new QLabel(tab_3);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(36);
        font.setBold(false);
        label->setFont(font);
        label->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_12->addWidget(label, 1, 1, 2, 2);

        label_23 = new QLabel(tab_3);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setPixmap(QPixmap(QString::fromUtf8("H:/\351\241\271\347\233\256\344\273\243\347\240\201/tobacco/uppermachine-tobacco/image/njfu.jpg")));

        gridLayout_12->addWidget(label_23, 0, 0, 1, 1);

        tabWidget->addTab(tab_3, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        groupBox_9 = new QGroupBox(tab);
        groupBox_9->setObjectName(QString::fromUtf8("groupBox_9"));
        groupBox_9->setGeometry(QRect(9, 9, 20, 20));
        gridLayout_11 = new QGridLayout(groupBox_9);
        gridLayout_11->setObjectName(QString::fromUtf8("gridLayout_11"));
        groupBox = new QGroupBox(tab);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, -20, 1961, 161));
        QFont font1;
        font1.setPointSize(12);
        groupBox->setFont(font1);
        label_14 = new QLabel(groupBox);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(330, 40, 511, 51));
        QFont font2;
        font2.setFamily(QString::fromUtf8("\345\215\216\346\226\207\346\245\267\344\275\223"));
        font2.setPointSize(36);
        font2.setBold(false);
        font2.setItalic(false);
        label_14->setFont(font2);
        label_14->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"font: 36pt \"\345\215\216\346\226\207\346\245\267\344\275\223\";"));
        label_14->setTextFormat(Qt::PlainText);
        layoutWidget_2 = new QWidget(groupBox);
        layoutWidget_2->setObjectName(QString::fromUtf8("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(2, 90, 841, 61));
        horizontalLayout_14 = new QHBoxLayout(layoutWidget_2);
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        horizontalLayout_14->setContentsMargins(0, 0, 0, 0);
        label_RGB = new QLabel(layoutWidget_2);
        label_RGB->setObjectName(QString::fromUtf8("label_RGB"));
        QFont font3;
        font3.setPointSize(12);
        font3.setBold(true);
        label_RGB->setFont(font3);
        label_RGB->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));

        horizontalLayout_14->addWidget(label_RGB);

        RGBstatus = new QLabel(layoutWidget_2);
        RGBstatus->setObjectName(QString::fromUtf8("RGBstatus"));
        RGBstatus->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));

        horizontalLayout_14->addWidget(RGBstatus);

        label_RGB_3 = new QLabel(layoutWidget_2);
        label_RGB_3->setObjectName(QString::fromUtf8("label_RGB_3"));
        label_RGB_3->setFont(font3);
        label_RGB_3->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));

        horizontalLayout_14->addWidget(label_RGB_3);

        RGBstatus_1 = new QLabel(layoutWidget_2);
        RGBstatus_1->setObjectName(QString::fromUtf8("RGBstatus_1"));
        RGBstatus_1->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));

        horizontalLayout_14->addWidget(RGBstatus_1);

        label_RGB_2 = new QLabel(layoutWidget_2);
        label_RGB_2->setObjectName(QString::fromUtf8("label_RGB_2"));
        label_RGB_2->setFont(font3);
        label_RGB_2->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));

        horizontalLayout_14->addWidget(label_RGB_2);

        RGBstatus_2 = new QLabel(layoutWidget_2);
        RGBstatus_2->setObjectName(QString::fromUtf8("RGBstatus_2"));
        RGBstatus_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));

        horizontalLayout_14->addWidget(RGBstatus_2);

        label_9 = new QLabel(layoutWidget_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setFont(font3);
        label_9->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));

        horizontalLayout_14->addWidget(label_9);

        spec_camera_status = new QLabel(layoutWidget_2);
        spec_camera_status->setObjectName(QString::fromUtf8("spec_camera_status"));
        spec_camera_status->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));

        horizontalLayout_14->addWidget(spec_camera_status);

        label_11 = new QLabel(layoutWidget_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setFont(font3);
        label_11->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));

        horizontalLayout_14->addWidget(label_11);

        plc_status = new QLabel(layoutWidget_2);
        plc_status->setObjectName(QString::fromUtf8("plc_status"));
        plc_status->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));

        horizontalLayout_14->addWidget(plc_status);

        label_17 = new QLabel(layoutWidget_2);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setFont(font3);
        label_17->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));

        horizontalLayout_14->addWidget(label_17);

        jxs_status = new QLabel(layoutWidget_2);
        jxs_status->setObjectName(QString::fromUtf8("jxs_status"));
        jxs_status->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));

        horizontalLayout_14->addWidget(jxs_status);

        layoutWidget = new QWidget(groupBox);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(790, 220, 574, 356));
        verticalLayout_5 = new QVBoxLayout(layoutWidget);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        layoutWidget1 = new QWidget(tab);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 140, 1951, 791));
        horizontalLayout_8 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_26 = new QLabel(layoutWidget1);
        label_26->setObjectName(QString::fromUtf8("label_26"));
        label_26->setFont(font3);
        label_26->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));

        horizontalLayout->addWidget(label_26);

        passionBtn = new QPushButton(layoutWidget1);
        passionBtn->setObjectName(QString::fromUtf8("passionBtn"));
        passionBtn->setFont(font3);
        passionBtn->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));

        horizontalLayout->addWidget(passionBtn);

        tomatoBtn = new QPushButton(layoutWidget1);
        tomatoBtn->setObjectName(QString::fromUtf8("tomatoBtn"));
        tomatoBtn->setFont(font3);
        tomatoBtn->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));

        horizontalLayout->addWidget(tomatoBtn);


        verticalLayout->addLayout(horizontalLayout);

        btn_start = new QPushButton(layoutWidget1);
        btn_start->setObjectName(QString::fromUtf8("btn_start"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btn_start->sizePolicy().hasHeightForWidth());
        btn_start->setSizePolicy(sizePolicy);
        QFont font4;
        font4.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font4.setPointSize(30);
        font4.setBold(false);
        font4.setItalic(false);
        btn_start->setFont(font4);
        btn_start->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"font: 30pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(btn_start);

        show_resoult = new QPushButton(layoutWidget1);
        show_resoult->setObjectName(QString::fromUtf8("show_resoult"));
        sizePolicy.setHeightForWidth(show_resoult->sizePolicy().hasHeightForWidth());
        show_resoult->setSizePolicy(sizePolicy);
        show_resoult->setFont(font4);
        show_resoult->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"font: 30pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(show_resoult);

        btn_setparam = new QPushButton(layoutWidget1);
        btn_setparam->setObjectName(QString::fromUtf8("btn_setparam"));
        sizePolicy.setHeightForWidth(btn_setparam->sizePolicy().hasHeightForWidth());
        btn_setparam->setSizePolicy(sizePolicy);
        btn_setparam->setFont(font4);
        btn_setparam->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"font: 30pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(btn_setparam);

        btn_save = new QPushButton(layoutWidget1);
        btn_save->setObjectName(QString::fromUtf8("btn_save"));
        sizePolicy.setHeightForWidth(btn_save->sizePolicy().hasHeightForWidth());
        btn_save->setSizePolicy(sizePolicy);
        btn_save->setFont(font4);
        btn_save->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"font: 30pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(btn_save);

        view_results = new QPushButton(layoutWidget1);
        view_results->setObjectName(QString::fromUtf8("view_results"));
        sizePolicy.setHeightForWidth(view_results->sizePolicy().hasHeightForWidth());
        view_results->setSizePolicy(sizePolicy);
        view_results->setFont(font4);
        view_results->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"font: 30pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(view_results);

        btn_quit = new QPushButton(layoutWidget1);
        btn_quit->setObjectName(QString::fromUtf8("btn_quit"));
        sizePolicy.setHeightForWidth(btn_quit->sizePolicy().hasHeightForWidth());
        btn_quit->setSizePolicy(sizePolicy);
        btn_quit->setFont(font4);
        btn_quit->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"font: 30pt \"\345\256\213\344\275\223\";"));

        verticalLayout->addWidget(btn_quit);


        horizontalLayout_8->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        groupBox_4 = new QGroupBox(layoutWidget1);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setFont(font3);
        groupBox_4->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        gridLayout_2 = new QGridLayout(groupBox_4);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        groupBox_2 = new QGroupBox(groupBox_4);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setFont(font3);
        gridLayout = new QGridLayout(groupBox_2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_topL = new QLabel(groupBox_2);
        label_topL->setObjectName(QString::fromUtf8("label_topL"));
        label_topL->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 170, 255);"));

        gridLayout->addWidget(label_topL, 0, 0, 1, 1);


        gridLayout_2->addWidget(groupBox_2, 0, 0, 1, 1);

        groupBox_3 = new QGroupBox(groupBox_4);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setFont(font3);
        gridLayout_3 = new QGridLayout(groupBox_3);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_topM = new QLabel(groupBox_3);
        label_topM->setObjectName(QString::fromUtf8("label_topM"));
        label_topM->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 170, 255);"));

        gridLayout_3->addWidget(label_topM, 0, 1, 1, 1);


        gridLayout_2->addWidget(groupBox_3, 0, 1, 1, 1);

        groupBox_5 = new QGroupBox(groupBox_4);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        groupBox_5->setFont(font3);
        gridLayout_4 = new QGridLayout(groupBox_5);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        label_topR = new QLabel(groupBox_5);
        label_topR->setObjectName(QString::fromUtf8("label_topR"));
        label_topR->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 170, 255);"));

        gridLayout_4->addWidget(label_topR, 0, 0, 1, 1);


        gridLayout_2->addWidget(groupBox_5, 0, 2, 1, 1);

        groupBox_6 = new QGroupBox(groupBox_4);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        groupBox_6->setFont(font3);
        gridLayout_5 = new QGridLayout(groupBox_6);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        showimg_right = new QLabel(groupBox_6);
        showimg_right->setObjectName(QString::fromUtf8("showimg_right"));
        showimg_right->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 170, 255);"));

        gridLayout_5->addWidget(showimg_right, 0, 0, 1, 1);


        gridLayout_2->addWidget(groupBox_6, 1, 0, 1, 1);

        groupBox_7 = new QGroupBox(groupBox_4);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        groupBox_7->setFont(font3);
        gridLayout_6 = new QGridLayout(groupBox_7);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        showimg_left = new QLabel(groupBox_7);
        showimg_left->setObjectName(QString::fromUtf8("showimg_left"));
        showimg_left->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 170, 255);"));

        gridLayout_6->addWidget(showimg_left, 0, 0, 1, 1);


        gridLayout_2->addWidget(groupBox_7, 1, 1, 1, 1);

        groupBox_8 = new QGroupBox(groupBox_4);
        groupBox_8->setObjectName(QString::fromUtf8("groupBox_8"));
        groupBox_8->setFont(font3);
        gridLayout_7 = new QGridLayout(groupBox_8);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        spec_camera_show = new QLabel(groupBox_8);
        spec_camera_show->setObjectName(QString::fromUtf8("spec_camera_show"));
        spec_camera_show->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 170, 255);"));

        gridLayout_7->addWidget(spec_camera_show, 0, 0, 1, 1);


        gridLayout_2->addWidget(groupBox_8, 1, 2, 1, 1);


        verticalLayout_2->addWidget(groupBox_4);


        horizontalLayout_8->addLayout(verticalLayout_2);

        horizontalLayout_8->setStretch(1, 1);
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        groupBox_11 = new QGroupBox(tab_2);
        groupBox_11->setObjectName(QString::fromUtf8("groupBox_11"));
        groupBox_11->setGeometry(QRect(40, 50, 531, 231));
        QFont font5;
        font5.setPointSize(16);
        font5.setBold(true);
        groupBox_11->setFont(font5);
        groupBox_11->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        gridLayout_13 = new QGridLayout(groupBox_11);
        gridLayout_13->setObjectName(QString::fromUtf8("gridLayout_13"));
        label_28 = new QLabel(groupBox_11);
        label_28->setObjectName(QString::fromUtf8("label_28"));
        label_28->setFont(font5);
        label_28->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));

        gridLayout_13->addWidget(label_28, 0, 0, 1, 1);

        exSpinBox_left = new QSpinBox(groupBox_11);
        exSpinBox_left->setObjectName(QString::fromUtf8("exSpinBox_left"));
        QFont font6;
        font6.setBold(true);
        exSpinBox_left->setFont(font6);
        exSpinBox_left->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));

        gridLayout_13->addWidget(exSpinBox_left, 0, 1, 1, 1);

        label_29 = new QLabel(groupBox_11);
        label_29->setObjectName(QString::fromUtf8("label_29"));
        label_29->setFont(font5);
        label_29->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));

        gridLayout_13->addWidget(label_29, 1, 0, 1, 1);

        wbSpinBox_left_2 = new QSpinBox(groupBox_11);
        wbSpinBox_left_2->setObjectName(QString::fromUtf8("wbSpinBox_left_2"));
        wbSpinBox_left_2->setFont(font6);
        wbSpinBox_left_2->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));

        gridLayout_13->addWidget(wbSpinBox_left_2, 1, 1, 1, 1);

        label_30 = new QLabel(groupBox_11);
        label_30->setObjectName(QString::fromUtf8("label_30"));
        label_30->setFont(font5);
        label_30->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));

        gridLayout_13->addWidget(label_30, 2, 0, 1, 1);

        GainSpinBox_left = new QSpinBox(groupBox_11);
        GainSpinBox_left->setObjectName(QString::fromUtf8("GainSpinBox_left"));
        GainSpinBox_left->setFont(font6);
        GainSpinBox_left->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));

        gridLayout_13->addWidget(GainSpinBox_left, 2, 1, 1, 1);

        groupBox_13 = new QGroupBox(tab_2);
        groupBox_13->setObjectName(QString::fromUtf8("groupBox_13"));
        groupBox_13->setGeometry(QRect(40, 350, 531, 231));
        groupBox_13->setFont(font5);
        groupBox_13->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        gridLayout_16 = new QGridLayout(groupBox_13);
        gridLayout_16->setObjectName(QString::fromUtf8("gridLayout_16"));
        label_38 = new QLabel(groupBox_13);
        label_38->setObjectName(QString::fromUtf8("label_38"));
        label_38->setFont(font5);
        label_38->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));

        gridLayout_16->addWidget(label_38, 0, 0, 1, 1);

        exspinBox_top = new QSpinBox(groupBox_13);
        exspinBox_top->setObjectName(QString::fromUtf8("exspinBox_top"));
        exspinBox_top->setFont(font6);
        exspinBox_top->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));

        gridLayout_16->addWidget(exspinBox_top, 0, 1, 1, 1);

        label_39 = new QLabel(groupBox_13);
        label_39->setObjectName(QString::fromUtf8("label_39"));
        label_39->setFont(font5);
        label_39->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));

        gridLayout_16->addWidget(label_39, 1, 0, 1, 1);

        wbspinBox_top = new QSpinBox(groupBox_13);
        wbspinBox_top->setObjectName(QString::fromUtf8("wbspinBox_top"));
        wbspinBox_top->setFont(font6);
        wbspinBox_top->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));

        gridLayout_16->addWidget(wbspinBox_top, 1, 1, 1, 1);

        label_40 = new QLabel(groupBox_13);
        label_40->setObjectName(QString::fromUtf8("label_40"));
        label_40->setFont(font5);
        label_40->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));

        gridLayout_16->addWidget(label_40, 2, 0, 1, 1);

        GainspinBox_top = new QSpinBox(groupBox_13);
        GainspinBox_top->setObjectName(QString::fromUtf8("GainspinBox_top"));
        GainspinBox_top->setFont(font6);
        GainspinBox_top->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));

        gridLayout_16->addWidget(GainspinBox_top, 2, 1, 1, 1);

        groupBox_15 = new QGroupBox(tab_2);
        groupBox_15->setObjectName(QString::fromUtf8("groupBox_15"));
        groupBox_15->setGeometry(QRect(700, 60, 531, 231));
        groupBox_15->setFont(font5);
        groupBox_15->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        gridLayout_18 = new QGridLayout(groupBox_15);
        gridLayout_18->setObjectName(QString::fromUtf8("gridLayout_18"));
        label_46 = new QLabel(groupBox_15);
        label_46->setObjectName(QString::fromUtf8("label_46"));
        label_46->setFont(font5);
        label_46->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));

        gridLayout_18->addWidget(label_46, 0, 0, 1, 1);

        exspinBox_right = new QSpinBox(groupBox_15);
        exspinBox_right->setObjectName(QString::fromUtf8("exspinBox_right"));
        exspinBox_right->setFont(font6);
        exspinBox_right->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));

        gridLayout_18->addWidget(exspinBox_right, 0, 1, 1, 1);

        label_47 = new QLabel(groupBox_15);
        label_47->setObjectName(QString::fromUtf8("label_47"));
        label_47->setFont(font5);
        label_47->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));

        gridLayout_18->addWidget(label_47, 1, 0, 1, 1);

        wbspinBox_right = new QSpinBox(groupBox_15);
        wbspinBox_right->setObjectName(QString::fromUtf8("wbspinBox_right"));
        wbspinBox_right->setFont(font6);
        wbspinBox_right->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));

        gridLayout_18->addWidget(wbspinBox_right, 1, 1, 1, 1);

        label_48 = new QLabel(groupBox_15);
        label_48->setObjectName(QString::fromUtf8("label_48"));
        label_48->setFont(font5);
        label_48->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));

        gridLayout_18->addWidget(label_48, 2, 0, 1, 1);

        GainspinBox_right = new QSpinBox(groupBox_15);
        GainspinBox_right->setObjectName(QString::fromUtf8("GainspinBox_right"));
        GainspinBox_right->setFont(font6);
        GainspinBox_right->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));

        gridLayout_18->addWidget(GainspinBox_right, 2, 1, 1, 1);

        btn_enterparam = new QPushButton(tab_2);
        btn_enterparam->setObjectName(QString::fromUtf8("btn_enterparam"));
        btn_enterparam->setGeometry(QRect(710, 380, 161, 91));
        btn_enterparam->setFont(font5);
        btn_enterparam->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        tab3_ReturnToMain = new QPushButton(tab_2);
        tab3_ReturnToMain->setObjectName(QString::fromUtf8("tab3_ReturnToMain"));
        tab3_ReturnToMain->setGeometry(QRect(1300, 820, 241, 91));
        tab3_ReturnToMain->setFont(font5);
        tab3_ReturnToMain->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        choose_to = new QPushButton(tab_2);
        choose_to->setObjectName(QString::fromUtf8("choose_to"));
        choose_to->setGeometry(QRect(340, 710, 241, 141));
        choose_to->setFont(font5);
        choose_to->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        choose_pa = new QPushButton(tab_2);
        choose_pa->setObjectName(QString::fromUtf8("choose_pa"));
        choose_pa->setGeometry(QRect(40, 710, 231, 141));
        choose_pa->setFont(font5);
        choose_pa->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        InputNum1 = new QLineEdit(tab_2);
        InputNum1->setObjectName(QString::fromUtf8("InputNum1"));
        InputNum1->setGeometry(QRect(774, 720, 81, 30));
        QFont font7;
        font7.setPointSize(16);
        font7.setBold(false);
        InputNum1->setFont(font7);
        InputNum5 = new QLineEdit(tab_2);
        InputNum5->setObjectName(QString::fromUtf8("InputNum5"));
        InputNum5->setGeometry(QRect(1050, 780, 94, 30));
        InputNum5->setFont(font7);
        label_5 = new QLabel(tab_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(720, 630, 108, 21));
        label_5->setFont(font5);
        InputNum4 = new QLineEdit(tab_2);
        InputNum4->setObjectName(QString::fromUtf8("InputNum4"));
        InputNum4->setGeometry(QRect(820, 780, 81, 30));
        InputNum4->setFont(font7);
        set_threshoid = new QPushButton(tab_2);
        set_threshoid->setObjectName(QString::fromUtf8("set_threshoid"));
        set_threshoid->setGeometry(QRect(720, 830, 261, 91));
        set_threshoid->setStyleSheet(QString::fromUtf8("font: 30pt \"\345\256\213\344\275\223\";"));
        label_32 = new QLabel(tab_2);
        label_32->setObjectName(QString::fromUtf8("label_32"));
        label_32->setGeometry(QRect(720, 780, 86, 31));
        label_32->setFont(font5);
        InputNum3 = new QLineEdit(tab_2);
        InputNum3->setObjectName(QString::fromUtf8("InputNum3"));
        InputNum3->setGeometry(QRect(1140, 720, 91, 28));
        InputNum3->setFont(font7);
        label_33 = new QLabel(tab_2);
        label_33->setObjectName(QString::fromUtf8("label_33"));
        label_33->setGeometry(QRect(940, 780, 86, 31));
        label_33->setFont(font5);
        label_10 = new QLabel(tab_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(717, 720, 43, 31));
        label_10->setFont(font5);
        label_18 = new QLabel(tab_2);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(870, 720, 43, 31));
        label_18->setFont(font5);
        InputNum2 = new QLineEdit(tab_2);
        InputNum2->setObjectName(QString::fromUtf8("InputNum2"));
        InputNum2->setGeometry(QRect(930, 720, 81, 30));
        InputNum2->setFont(font7);
        label_12 = new QLabel(tab_2);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(40, 630, 141, 21));
        label_12->setFont(font5);
        label_21 = new QLabel(tab_2);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setGeometry(QRect(1030, 720, 91, 31));
        label_21->setFont(font5);
        tabWidget->addTab(tab_2, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        show_resultsImg = new QLabel(tab_4);
        show_resultsImg->setObjectName(QString::fromUtf8("show_resultsImg"));
        show_resultsImg->setGeometry(QRect(10, 60, 816, 683));
        show_resultsImg->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        label_3 = new QLabel(tab_4);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, -10, 181, 81));
        label_3->setFont(font5);
        tab4_ReturnToMain = new QPushButton(tab_4);
        tab4_ReturnToMain->setObjectName(QString::fromUtf8("tab4_ReturnToMain"));
        tab4_ReturnToMain->setGeometry(QRect(1530, 830, 291, 101));
        tab4_ReturnToMain->setStyleSheet(QString::fromUtf8("font: 30pt \"\345\256\213\344\275\223\";"));
        resoult_info_label = new QPlainTextEdit(tab_4);
        resoult_info_label->setObjectName(QString::fromUtf8("resoult_info_label"));
        resoult_info_label->setGeometry(QRect(950, 60, 871, 681));
        resoult_info_label->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        choose_pa_2 = new QPushButton(tab_4);
        choose_pa_2->setObjectName(QString::fromUtf8("choose_pa_2"));
        choose_pa_2->setGeometry(QRect(10, 830, 161, 91));
        choose_pa_2->setFont(font5);
        choose_pa_2->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        label_13 = new QLabel(tab_4);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(10, 750, 141, 21));
        label_13->setFont(font5);
        choose_to_2 = new QPushButton(tab_4);
        choose_to_2->setObjectName(QString::fromUtf8("choose_to_2"));
        choose_to_2->setGeometry(QRect(210, 830, 161, 91));
        choose_to_2->setFont(font5);
        choose_to_2->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        tabWidget->addTab(tab_4, QString());

        gridLayout_10->addWidget(tabWidget, 0, 0, 1, 1);


        retranslateUi(Widget);

        tabWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        label->setText(QCoreApplication::translate("Widget", "\346\236\234\350\224\254\345\210\206\351\200\211\347\240\224\345\217\221\345\233\242\351\230\237", nullptr));
        label_23->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("Widget", "Tab1", nullptr));
        groupBox_9->setTitle(QString());
        groupBox->setTitle(QString());
        label_14->setText(QCoreApplication::translate("Widget", "\346\236\234\350\224\254\345\210\206\351\200\211\347\240\224\345\217\221\345\233\242\351\230\237", nullptr));
        label_RGB->setText(QCoreApplication::translate("Widget", "\345\267\246\344\276\247\347\233\270\346\234\272", nullptr));
        RGBstatus->setText(QCoreApplication::translate("Widget", "\346\234\252\350\277\236\346\216\245", nullptr));
        label_RGB_3->setText(QCoreApplication::translate("Widget", "\345\217\263\344\276\247\347\233\270\346\234\272", nullptr));
        RGBstatus_1->setText(QCoreApplication::translate("Widget", "\346\234\252\350\277\236\346\216\245", nullptr));
        label_RGB_2->setText(QCoreApplication::translate("Widget", "\351\241\266\351\203\250\347\233\270\346\234\272", nullptr));
        RGBstatus_2->setText(QCoreApplication::translate("Widget", "\346\234\252\350\277\236\346\216\245", nullptr));
        label_9->setText(QCoreApplication::translate("Widget", "\345\205\211\350\260\261\347\233\270\346\234\272", nullptr));
        spec_camera_status->setText(QCoreApplication::translate("Widget", "\346\234\252\350\277\236\346\216\245", nullptr));
        label_11->setText(QCoreApplication::translate("Widget", "PLC", nullptr));
        plc_status->setText(QCoreApplication::translate("Widget", "\346\234\252\350\277\236\346\216\245", nullptr));
        label_17->setText(QCoreApplication::translate("Widget", " \346\234\272\346\242\260\346\211\213", nullptr));
        jxs_status->setText(QCoreApplication::translate("Widget", "\346\234\252\350\277\236\346\216\245", nullptr));
        label_26->setText(QCoreApplication::translate("Widget", "\345\275\223\345\211\215\345\210\206\351\200\211\346\236\234\350\224\254\357\274\232", nullptr));
        passionBtn->setText(QCoreApplication::translate("Widget", "\347\231\276\351\246\231\346\236\234", nullptr));
        tomatoBtn->setText(QCoreApplication::translate("Widget", "\347\225\252\350\214\204", nullptr));
        btn_start->setText(QCoreApplication::translate("Widget", "\345\274\200\345\247\213\351\207\207\351\233\206", nullptr));
        show_resoult->setText(QCoreApplication::translate("Widget", "\345\210\206\351\200\211\347\225\214\351\235\242", nullptr));
        btn_setparam->setText(QCoreApplication::translate("Widget", "\350\256\276\347\275\256\345\217\202\346\225\260", nullptr));
        btn_save->setText(QCoreApplication::translate("Widget", "\344\277\235\345\255\230\345\233\276\347\211\207", nullptr));
        view_results->setText(QCoreApplication::translate("Widget", "\346\237\245\347\234\213\347\273\223\346\236\234", nullptr));
        btn_quit->setText(QCoreApplication::translate("Widget", "\351\200\200\345\207\272\347\263\273\347\273\237", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("Widget", "\347\233\270\346\234\272", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("Widget", "\351\241\266\351\203\250\344\270\213", nullptr));
        label_topL->setText(QString());
        groupBox_3->setTitle(QCoreApplication::translate("Widget", "\351\241\266\351\203\250\344\270\255", nullptr));
        label_topM->setText(QString());
        groupBox_5->setTitle(QCoreApplication::translate("Widget", "\351\241\266\351\203\250\344\270\212", nullptr));
        label_topR->setText(QString());
        groupBox_6->setTitle(QCoreApplication::translate("Widget", "\345\267\246\344\276\247", nullptr));
        showimg_right->setText(QString());
        groupBox_7->setTitle(QCoreApplication::translate("Widget", "\345\217\263\344\276\247", nullptr));
        showimg_left->setText(QString());
        groupBox_8->setTitle(QCoreApplication::translate("Widget", "\345\205\211\350\260\261\347\233\270\346\234\272", nullptr));
        spec_camera_show->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("Widget", "Tab2", nullptr));
        groupBox_11->setTitle(QCoreApplication::translate("Widget", "\345\267\246\344\276\247\347\233\270\346\234\272", nullptr));
        label_28->setText(QCoreApplication::translate("Widget", "\346\233\235\345\205\211\346\227\266\351\227\264\357\274\232", nullptr));
        label_29->setText(QCoreApplication::translate("Widget", "\347\231\275\345\271\263\350\241\241\357\274\232", nullptr));
        label_30->setText(QCoreApplication::translate("Widget", "\345\242\236\347\233\212\357\274\232", nullptr));
        groupBox_13->setTitle(QCoreApplication::translate("Widget", "\351\241\266\344\276\247\347\233\270\346\234\272", nullptr));
        label_38->setText(QCoreApplication::translate("Widget", "\346\233\235\345\205\211\346\227\266\351\227\264\357\274\232", nullptr));
        label_39->setText(QCoreApplication::translate("Widget", "\347\231\275\345\271\263\350\241\241\357\274\232", nullptr));
        label_40->setText(QCoreApplication::translate("Widget", "\345\242\236\347\233\212\357\274\232", nullptr));
        groupBox_15->setTitle(QCoreApplication::translate("Widget", "\345\217\263\344\276\247\347\233\270\346\234\272", nullptr));
        label_46->setText(QCoreApplication::translate("Widget", "\346\233\235\345\205\211\346\227\266\351\227\264\357\274\232", nullptr));
        label_47->setText(QCoreApplication::translate("Widget", "\347\231\275\345\271\263\350\241\241\357\274\232", nullptr));
        label_48->setText(QCoreApplication::translate("Widget", "\345\242\236\347\233\212\357\274\232", nullptr));
        btn_enterparam->setText(QCoreApplication::translate("Widget", "\350\256\276\347\275\256\347\233\270\346\234\272\345\217\202\346\225\260", nullptr));
        tab3_ReturnToMain->setText(QCoreApplication::translate("Widget", " \350\277\224\345\233\236\344\270\273\347\225\214\351\235\242", nullptr));
        choose_to->setText(QCoreApplication::translate("Widget", "\347\225\252\350\214\204", nullptr));
        choose_pa->setText(QCoreApplication::translate("Widget", " \347\231\276\351\246\231\346\236\234", nullptr));
        InputNum1->setText(QString());
        InputNum5->setText(QString());
        label_5->setText(QCoreApplication::translate("Widget", "\351\230\210\345\200\274\346\240\207\345\207\206\357\274\232", nullptr));
        InputNum4->setText(QString());
        set_threshoid->setText(QCoreApplication::translate("Widget", " \350\256\276\345\256\232\351\230\210\345\200\274", nullptr));
        label_32->setText(QCoreApplication::translate("Widget", "\347\274\272\351\231\267\346\225\260\351\207\217", nullptr));
        label_33->setText(QCoreApplication::translate("Widget", "\347\274\272\351\231\267\351\235\242\347\247\257", nullptr));
        label_10->setText(QCoreApplication::translate("Widget", "\347\263\226\345\272\246", nullptr));
        label_18->setText(QCoreApplication::translate("Widget", "\347\233\264\345\276\204", nullptr));
        InputNum2->setText(QString());
        label_12->setText(QCoreApplication::translate("Widget", "\350\257\267\351\200\211\346\213\251\346\236\234\350\224\254\357\274\232", nullptr));
        label_21->setText(QCoreApplication::translate("Widget", "\351\242\234\350\211\262\345\215\240\346\257\224", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("Widget", "Tab3", nullptr));
        show_resultsImg->setText(QString());
        label_3->setText(QCoreApplication::translate("Widget", "\345\244\204\347\220\206\347\273\223\346\236\234\345\233\276\357\274\232", nullptr));
        tab4_ReturnToMain->setText(QCoreApplication::translate("Widget", "\350\277\224\345\233\236\344\270\273\347\225\214\351\235\242", nullptr));
        choose_pa_2->setText(QCoreApplication::translate("Widget", " \347\231\276\351\246\231\346\236\234", nullptr));
        label_13->setText(QCoreApplication::translate("Widget", "\345\275\223\345\211\215\345\210\206\347\261\273\346\236\234\350\224\254\357\274\232", nullptr));
        choose_to_2->setText(QCoreApplication::translate("Widget", "\347\225\252\350\214\204", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QCoreApplication::translate("Widget", "Tab4", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
