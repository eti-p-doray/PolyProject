#-------------------------------------------------
#
# Project created by QtCreator 2015-01-25T16:00:01
#
#-------------------------------------------------

QT       += core gui opengl serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ArmController
TEMPLATE = app


SOURCES += main.cpp\
    Devices/xbee.cpp \
    Devices/packet.cpp \
    Devices/framedata.cpp \
    Devices/handDevice.cpp \
    State/OptimalEstimation.cpp \
    State/ArmKnowledge.cpp \
    State/Imu.cpp \
    model.cpp \
    Controller.cpp \
    glview.cpp

HEADERS  += \
    Average.h \
    Controller.h \
    glview.h \
    model.h \
    State/ArmKnowledge.h \
    State/Matrix.h \
    State/OptimalEstimation.h \
    State/Imu.h \
    Devices/xbee.h \
    Devices/handDevice.h \
    buffer.h

FORMS    += window.ui

CONFIG += c++11 qt

mac {
    QMAKE_PRE_LINK += cp /opt/local/lib/LeapSDK/lib/libc++/libLeap.dylib ArmController.app/Contents/MacOs;
    QMAKE_PRE_LINK += cp /opt/local/lib/opencv/libopencv_core.dylib ArmController.app/Contents/MacOs;
}
unix {
    INCLUDEPATH += /opt/local/include/ /usr/local/include/
    LIBS += -L/opt/local/lib/opencv/ -L/usr/local/lib/opencv/ -lopencv_core \
            -L/opt/local/lib/LeapSDK/lib/libc++/ -L/usr/local/lib/LeapSDK/lib/libc++/ -lLeap
}
win32 {
    CONFIG( release ) {
        QMAKE_PRE_LINK += cp D:/Applications/LeapSDK/lib/x86/Leap.dll release/ &
        QMAKE_PRE_LINK += cp D:/Applications/opencv/build/x86/vc12/bin/opencv_core2411.dll release/ &
    }
    else {
        QMAKE_PRE_LINK += cp D:/Applications/LeapSDK/lib/x86/Leap.dll debug/
        QMAKE_PRE_LINK += cp D:/Applications/opencv/build/x86/vc12/bin/opencv_core2411.dll debug/
    }
    INCLUDEPATH += $$PWD/../../../Applications/opencv/build/include/ $$PWD/../../../Applications/LeapSDK/include
    LIBS += -L$$PWD/../../../Applications/opencv/build/x86/vc12/lib/ -lopencv_core2411 \
            -L$$PWD/../../../Applications/LeapSDK/lib/x86 -lLeap
}

isEmpty(TARGET_EXT) {
    win32 {
        TARGET_CUSTOM_EXT = .exe
    }
    mac {
        TARGET_CUSTOM_EXT = .app
    }
} else {
    TARGET_CUSTOM_EXT = $${TARGET_EXT}
}

win32 {
    DEPLOY_COMMAND = windeployqt
}
mac {
    DEPLOY_COMMAND = macdeployqt
}

win32 {
    CONFIG( debug, debug|release ) {
        # debug
        DEPLOY_TARGET = $$shell_quote($$shell_path($${OUT_PWD}/Debug/$${TARGET}$${TARGET_CUSTOM_EXT}))
    } else {
        # release
        DEPLOY_TARGET = $$shell_quote($$shell_path($${OUT_PWD}/Release/$${TARGET}$${TARGET_CUSTOM_EXT}))
    }
}
mac {
    CONFIG( debug, debug|release ) {
        # debug
        DEPLOY_TARGET = $$shell_quote($$shell_path($${OUT_PWD}/$${TARGET}$${TARGET_CUSTOM_EXT}))
    } else {
        # release
        DEPLOY_TARGET = $$shell_quote($$shell_path($${OUT_PWD}/$${TARGET}$${TARGET_CUSTOM_EXT}))
    }
}



#  # Uncomment the following line to help debug the deploy command when running qmake
#  warning($${DEPLOY_COMMAND} $${DEPLOY_TARGET})

QMAKE_POST_LINK = $${DEPLOY_COMMAND} $${DEPLOY_TARGET}

