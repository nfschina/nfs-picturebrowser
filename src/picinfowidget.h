#ifndef PICINFOWIDGET_H
#define PICINFOWIDGET_H

#include <QWidget>
#include <QStyleOption>
#include <QPainter>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QImageReader>
#include <QFileInfo>
#include <QFile>
#include <QFileDialog>
#include <QDateTime>
#include <QPropertyAnimation>
#include <QMap>
#include <QVariant>
#include <QDesktopServices>
#include <QUrl>
#include <QImage>
#include <QFrame>
//#include <QDebug>
#include <QImageReader>
#include <QScrollArea>
#include <QScrollBar>
#include"define.h"
#include <QSize>
#include <QDesktopWidget>
#include<QDebug>

class picInfoWidget : public QScrollArea
{
    Q_OBJECT
public:
    explicit picInfoWidget(QWidget *parent = 0);
    int infonum;
    int moveFlag ;

    QLabel*picName;
    QLabel*picType;
    QLabel*picSize;
    QLabel*picResolving;

    QLabel*picNameDetail;
    QLabel*picTypeDetail;
    QLabel*picSizeDetail;
    QLabel*picResolvingDetail;

    QLabel*filePath;
    QLabel*fileCreateTime;
    QLabel*fileModifyTime;
    QLabel*fileSize;
    QLabel*fileOwner;

    QLabel*filePathDetail;
    QLabel*fileCreateTimeDetail;
    QLabel*fileModifyTimeDetail;
    QLabel*fileSizeDetail;
    QLabel*fileOwnerDetail;

    QLabel *lb_PhotoInfo;
    QLabel *lb_FileInfo;
    void adjustSize();

    void mouseDoubleClickEvent(QMouseEvent *e);
signals:

public slots:
    void receiveThreadMsg(QFileInfo imgInfo, QStringList sizeAndDpi);
};

#endif // PICINFOWIDGET_H
