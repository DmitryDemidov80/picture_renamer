#-------------------------------------------------
#
# Project created by QtCreator 2015-11-08T23:48:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

VERSION = 1.3.7
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

DESTDIR += ../picture_renamer_bin

QMAKE_TARGET_DESCRIPTION = "Picture renamer"
RC_ICONS = icons/main_icon.ico

TARGET = picture_renamer
TEMPLATE = app

CONFIG += c++1z

SOURCES += main.cpp\
    3rdparty/exif/exif.cpp \
    directoryreader.cpp \
    mainwinrenamer.cpp \
    editorwgt.cpp

HEADERS  += mainwinrenamer.h \
    3rdparty/exif/exif.h \
    directoryreader.h \
    editorwgt.h

INCLUDEPATH += 3rdparty/exif \

#Формирование даты и времени сборки (меняется при полной пересборке проекта)
win32:{
BLD_DATE=$$system(date /t)
BLD_TIME=$$system(time /t)
#message(WIN DATE: $$BLD_DATE TIME: $$BLD_TIME)
DEFINES += BUILD_DATE=\\\"$$BLD_DATE\\\"
DEFINES += BUILD_TIME=\\\"$$BLD_TIME\\\"
} else {
#BUILDDATE = $$system( date "+%d.%m.%Y_%H:%M:%S" ) и дата, и время (в линуксе)
BLD_DATE=$$system(date)
BLD_DATE = $$system( date "+%d.%m.%Y" )
BLD_TIME = $$system( date "+%H:%M" )
#BLD_TIME = $$system( date "+%H:%M:%S" )
#message(LINUX DATE: $$BLD_DATE TIME: $$BLD_TIME)
DEFINES += BUILD_DATE=\\\"$$BLD_DATE\\\"
DEFINES += BUILD_TIME=\\\"$$BLD_TIME\\\"
}
