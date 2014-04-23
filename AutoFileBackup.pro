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
    diff_match_patch/diff_match_patch.cpp \
    filecopysettings.cpp

HEADERS  += autofilebackup.h \
    diff_match_patch/diff_match_patch.h \
    filecopysettings.h

FORMS    += autofilebackup.ui
