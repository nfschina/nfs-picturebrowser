#ifndef COMMON_H
#define COMMON_H


#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QLabel>
#include <QPalette>
#include <QPaintEvent>
#include <QFile>
#include <QResizeEvent>
#include <QDebug>
#include <QDesktopWidget>
#include <QApplication>
#include <QRect>
#include <QMouseEvent>
#include <QFileDialog>
#include <QPainter>
#include <QEvent>
#include <QRect>
#include <QTimer>
#include <QtGlobal>
#include <QImageReader>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <math.h>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QTimeLine>
#include <QGraphicsItemAnimation>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QImage>

#include <QDir>
#include <QTextItem>
#include <QTextDocument>
#include <QRadialGradient>
#include <QFileInfo>

#include <QClipboard>
#include <QDataStream>
#include <QProcess>
#include <QMessageBox>
#include <QMap>
#include <QStringList>
#include <iostream>
#include <QPropertyAnimation>
#include <QButtonGroup>
#include <QSettings>
#include <QMovie>


//local head file
#include"formats/imagereader.h"

enum showOrhide
{
    Show,
    Hide
};

enum Direction
{
    Up,
    Right,
    Down,
    Left,
    Unknown
};

enum Checked
{
    Ok,
    Cancel,
    Cover,
};


#endif // COMMON_H

