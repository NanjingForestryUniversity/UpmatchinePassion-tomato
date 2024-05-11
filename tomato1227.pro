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


# rgb camera SDK
#ROOT_DIR = $$PWD
LIBS += -LF:projectNJFU/20240429(END)/sdk -lMvCameraControl
INCLUDEPATH += F:/projectNJFU/20240429(END)/sdk/rgbcamera_sdk/Includes
DEPENDPATH += F:/projectNJFU/20240429(END)/sdk/rgbcamera_sdk/Includes
# LIBS +=-LE:/QT/projects/tomato20240415/tomato04150955 -lMvCameraControl
# INCLUDEPATH += E:/QT/projects/tomato20240415/tomato04150955/rgbcamera_sdk/Includes
# DEPENDPATH += E:/QT/projects/tomato20240415/tomato04150955/rgbcamera_sdk/Includes




# opencv
LIBS += -LF:/projectNJFU/20240429(END)/sdk/opencv -llibopencv_world344.dll
LIBS += -LF:/projectNJFU/20240429(END)/sdk/opencv/Withcontrib -llibopencv_img_hash344.dll
INCLUDEPATH += F:/projectNJFU/20240429(END)/sdk/opencv/Withcontrib/include
INCLUDEPATH += F:/projectNJFU/20240429(END)/sdk/opencv/Withcontrib/include/opencv
INCLUDEPATH += F:/projectNJFU/20240429(END)/sdk/opencv/Withcontrib/include/opencv2

# spec

LIBS += -LF:/projectNJFU/20240429(END)/sdk -lSpectrolDll
INCLUDEPATH += F:/projectNJFU/20240429(END)/sdk
DEPENDPATH += F:/projectNJFU/20240429(END)/sdk
LIBS += -LF:/projectNJFU/20240429(END)/sdk -lSpectrolDll

RESOURCES +=
