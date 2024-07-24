# 果蔬智能装备及产业化

UpmatchinePassion-tomato
 文件夹：
├─***CameraConfig***
├─debug
│  ├─bearer
│  ├─CameraConfig
│  ├─debug
│  ├─iconengines
│  ├─imageformats
│  ├─Log
│  ├─platforms
│  ├─release
│  ├─***Settings***
│  │  └─401d
│  ├─styles
│  └─translations
├─dll
├─***image***
│  ├─L
│  ├─R
│  ├─specImg
│  └─T
│      ├─bottom
│      ├─middle
│      └─top
├─***opencv***
│  ├─include
│  │  ├─CMakeFiles
│  │  ├─opencv
│  │  └─opencv2
│  │     
│  └─Withcontrib
│      └─include
│          ├─opencv
│          └─opencv2
├─release
├─***rgbcamera_sdk***
│  ├─Includes
│  └─Libraries
├─***RT_SDK***
│  ├─include
│  ├─lib
└─**src**

## 0 引言

此代码涉及海康威视面阵相机**MV-CS050-10GC**

光谱相机为无锡谱世界生产的RT10，**settings文件夹一定要放在debug目录下**。

与机械手通讯采用tcp-ip协议，主要发送分级结果给机械手。

与称重模块通讯以及光谱相机筛选谱段以及拼接。




**注意：依赖文件在release里面，是v3.0版本，记得下载**

## 1 细节

福建泉州番茄-百香果 Qt 项目代码，应该是 Qt 人的入门级代码。

代码部分一共有三部分，camera^ thread和widget, camera主要内容是相机，负责相 机的初始化和参数设置‘thread主要内容是线程，负责各个线程的工作和启停，widget主要内容是界面按钮和信号的处理。

下面进行各个分部分介绍(完整代码都有注释，所以此文档重点介绍一些难理解的点)

### 1.1 Camera

此次RGB相机选用的是海康的面阵相机，相机型号为MV-CS050-10GC,关于相机的 用户手册可以去海康官网下载。此次一共用到三个相机。

![1721802095232](C:\Users\PC\AppData\Roaming\Typora\typora-user-images\1721802095232.png)

一共三个相机，分别为图中标的 14、15 和 17，以传送带前进方向为正方向，17 为右 侧相机，14 为左侧相机，15 为顶部相机。因为三个相机，就得确定三个相机的连接顺序， 所以选择根据相机第四位ip来确定顺序，100为右侧相机，150为左侧相机，200为顶部相 机，以下为代码具体实现。

```c++
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
```



其余代码可参考海康相机调试软件MVS自带的开发指南，里面有完整的抓图程序以及 各个函数接口定义。路径为D:\MVS\Development\Documentations，具体图片如如所示。

![1721802263676](C:\Users\PC\AppData\Roaming\Typora\typora-user-images\1721802263676.png)

光谱相机代码就当图一乐。如果有兴趣， 可以搜索.bil编排格式的光谱数据，即可理解数据筛选。

### 1.2 Thread

线程代码部分开头为三个RGB相机的回调函数，主要功能是相机传输图像的数据，代 码层面接受图像数据并处理，具体内容看海康相机的用户指南，里面有关于回调的详细解释。

此次项目一共开设了三个线程，分别为ProcessImg (处理光谱相机数据线程)、 RecvThread (接受python端返回的数据线程)和SendThread (给python端发送数据线程)， 所有线程相关的代码都在各个线程类的Run函数内，因为线程在启动后，会执行一次他对 应Run函数，每个线程都写了 while (1)死循环，为的是让线程一直处于监听状态，一旦满 足处理要求就立马进行处理。线程开启代码在widget.cpp内，具体代码如下。

//开启线程也就是执行一次对应线程的**run()**函数在**thread**里面有 即执行三个线程的**while** **(1)***死循环*一直监听

```c++
processimg->start();

sendthread->start()； 
recvthread->start()；
```



发送和接收线程内，对于数据有对应的协议位，具体协议内容在python端，其中发送 线程内，对于发送的判断条件为什么是： topImgTQueue>15，因为从RGB相机第 一次触发到称重模块之间的果托数量正好是16，所以要再第16次触发后开始发送，这样 读取到的python返回值和读取到的称重数据就能对应上。发送线程为什么要数据对齐，因 为三个RGB相机在空间上是有错位的，而三个相机又是同时触发，所以相机的图片需要进 行数据对齐，既删除掉一些多余的图像。

其余代码都是常规代码，问ChatGPT就可以。

### 1.3 Widget

Widget主要是负责一些界面上的信号和函数，这部分重点应该是信号和槽函数的 connect函数的理解，可以自行写一些信号和槽函数的代码，这样可以更深入了解信号和 槽函数对应的关系。

Widget内涉及到两个协议，分别是跟机械手通讯的协议和跟PLC通讯的协议。

与机械手通讯采用Socket TCP/IP协议，Qt端做服务器，机械手做客户机，端口号为 21123，IP任意，Qt会监听本地指定端口号下的任意IP,通讯报文格式为 [X:0;Y:0;A:0;ATTR:0;ID:0;]，ATTR后面是等级，ID后面是果类0是番茄1是百香果。

与PLC通讯采用Modbus TCP协议，端口号为2000，IP为192.168.101.110,从机地 址为1, Qt端做主站，PLC做从站，分为收跟发，Qt端会给PLC发送等级信息和编号信 息，Qt端会从PLC读取称重数据。跟PLC商量称重数据放在寄存器40001 上, PLC负责 写，Qt负责读取；编号放在寄存器40002，等级信息放在40003,4XXXX寄存器在PLC端是 16位的数据寄存器，所以所有读取格式都采用uint16_t,但代码中读、写的地址是40000 和40001,这是因为Modbus TCP协议读、写似乎是从指定寄存器地址的下一位地址开始 操作，验证过结果是对的，能正确读、写数据，但具体理论是否如我们假设的一样没去深 究。