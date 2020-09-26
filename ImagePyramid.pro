QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$(OPENCV_SDK_DIR)\include

LIBS += -L$$(OPENCV_SDK_DIR)\x64\mingw\lib \
        -lopencv_core440        \
        -lopencv_highgui440     \
        -lopencv_imgcodecs440   \
        -lopencv_imgproc440     \
        -lopencv_features2d440  \
        -lopencv_calib3d440

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
