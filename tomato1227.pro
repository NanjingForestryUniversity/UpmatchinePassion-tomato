QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    camera.cpp \
    main.cpp \
    src/modbus-data.c \
    src/modbus-rtu.c \
    src/modbus-tcp.c \
    src/modbus.c \
    thread.cpp \
    widget.cpp

HEADERS += \
    camera.h \
    src/modbus-private.h \
    src/modbus-rtu-private.h \
    src/modbus-rtu.h \
    src/modbus-tcp-private.h \
    src/modbus-tcp.h \
    src/modbus-version.h \
    src/modbus.h \
    thread.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


DISTFILES += \
            src/modbus.lib
LIBS += -Ldll -lws2_32

#查看所有路径知否正确 RGB opencv 光谱相机
#建议改成绝对路径
# rgb camera SDK
LIBS += -LD:/porject/20240425/rgbcamera_sdk/Libraries/win64 -lMvCameraControl
INCLUDEPATH += D:/porject/20240425/rgbcamera_sdk/Includes
DEPENDPATH += D:/porject/20240425/rgbcamera_sdk/Includes



# spec
INCLUDEPATH += D:/porject/20240425/RT_SDK/SDK/include
DEPENDPATH += D:/porject/20240425/RT_SDK/SDK/include
LIBS += -LD:/porject/20240425/RT_SDK/SDK/lib -lSpectrolDll

