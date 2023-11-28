QT       += core gui serialport charts serialbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += \
    activewidget.cpp \
    initwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    serial.cpp \
    startwidget.cpp
    style.astylerc \

HEADERS += \
    activewidget.h \
    initwidget.h \
    mainwindow.h \
    serial.h \
    startwidget.h \
    types.h

FORMS += \
    activewidget.ui \
    initwidget.ui \
    mainwindow.ui \
    startwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    img.qrc
