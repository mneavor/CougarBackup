#-------------------------------------------------
#
# Project created by QtCreator 2014-05-22T11:37:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CougarBackup
TEMPLATE = app


SOURCES += main.cpp\
        cougarbackup.cpp \
    rsync.cpp

HEADERS  += cougarbackup.h \
    rsync.h

FORMS    += cougarbackup.ui
