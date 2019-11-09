#-------------------------------------------------
#
# Project created by QtCreator 2015-12-23T10:23:27
#
#-------------------------------------------------

QT       += core gui
QT += widgets
#QT += opengl
QT += svg
QT += printsupport


greaterThan(QT_MAJOR_VERSION, 4):
#QT += widgets
#QT += webkitwidgets
#QT += gui

TARGET = cdos-picturebrowser
#TARGET = 看图工具
TEMPLATE = app

QMAKE_RPATHDIR += /usr/share/cdos-picturebrowser/application/cdos-picturebrowser/lib
TRANSLATIONS=qt_zh_CN.ts


OBJECTS_DIR = tmp
MOC_DIR = tmp
UI_DIR = tmp
RCC_DIR =tmp


SOURCES += main.cpp\
        widget.cpp \
    titlebarwidget.cpp \
    dragproxy.cpp \
    stackedwidget.cpp \
    mainwidget_1.cpp \
    mainwidget_2.cpp \
    maintaskwidget.cpp \
    showpictutewidget.cpp \
    loadfileobj.cpp \
    bigpictureview.cpp \
    setupdialog.cpp \
    pictureitem.cpp \
    smallpictureview.cpp \
    bottomwidget.cpp \
    loadthumbnailthread.cpp \
    picinfowidget.cpp \
    opacityanimation.cpp \
    slidedialog.cpp \
    messagebox.cpp \
    formats/nfspbPsd.cpp \
    formats/psd2png.cpp \
    formats/nfspbMiniz.c \
    formats/imagereader.cpp \
    fileinfothread.cpp \
    pagewidget.cpp

HEADERS  += widget.h \
    titlebarwidget.h \
    common.h \
    dragproxy.h \
    stackedwidget.h \
    mainwidget_1.h \
    mainwidget_2.h \
    maintaskwidget.h \
    showpictutewidget.h \
    loadfileobj.h \
    bigpictureview.h \
    loadoriimgthread.h \
    setupdialog.h \
    pagewidget.h \
    pictureitem.h \
    smallpictureview.h \
    bottomwidget.h \
    loadthumbnailthread.h \
    picinfowidget.h \
    opacityanimation.h \
    slidedialog.h \
    messagebox.h \
    define.h \
    formats/nfspbPsd.h \
    formats/fileformats.h \
    formats/imagereader.h \
    psd2png.h \
    fileinfothread.h

RESOURCES += \
    source_files.qrc

#LIBS +=-L/usr/local  -losscore -lossextra


INCLUDEPATH += /usr/local/include
INCLUDEPATH += /usr/local/include/GraphicsMagick


#unix: CONFIG += link_pkgconfig
#unix: PKGCONFIG += gtk+-3.0


QMAKE_CXXFLAGS += -std=c++11
