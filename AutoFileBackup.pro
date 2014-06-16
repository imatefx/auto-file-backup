#-------------------------------------------------
#
# Project created by QtCreator 2014-04-20T20:02:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AutoFileBackup
TEMPLATE = app

SOURCES += main.cpp\
        autofilebackup.cpp \
    filecopysettings.cpp \
    droptablewidget.cpp \
    projectconfiguration.cpp

HEADERS  += autofilebackup.h \
    filecopysettings.h \
    droptablewidget.h \
    projectconfiguration.h

FORMS    += autofilebackup.ui

RESOURCES += \
    AppResources.qrc

#CONFIG += console

RC_FILE = winicon.rc

OTHER_FILES += \
    winicon.rc
