#-------------------------------------------------
#
# Project created by QtCreator 2015-11-08T23:48:46
#
#-------------------------------------------------

QT       += core gui
#CONFIG += qt debug

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

VERSION = 1.3.2
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

TARGET = PictureRenamer
TEMPLATE = app

CONFIG += c++17

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
