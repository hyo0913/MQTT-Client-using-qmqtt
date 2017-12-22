QT       += core gui network mqtt

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MQTT_CLIENT
TEMPLATE = app

HEADERS  += mainwindow.h \
        ConnectOptionDialog.h \
        SubscribePrintForm.h

SOURCES += main.cpp\
        mainwindow.cpp \
        ConnectOptionDialog.cpp \
        SubscribePrintForm.cpp

FORMS    += mainwindow.ui \
        ConnectOptionDialog.ui \
        SubscribePrintForm.ui
