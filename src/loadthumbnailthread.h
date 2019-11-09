#ifndef LOADTHUMBNAILTHREAD_H
#define LOADTHUMBNAILTHREAD_H
#include <QThread>
#include <QDir>
#include <QImage>
#include <QPainter>
#include <QFileInfo>
#include <QImageReader>
#include <QImageWriter>
#include "define.h"
#include"common.h"
//#include <QDebug>
class loadThumbnailThread : public QThread
{
    Q_OBJECT
public:
    QFileInfoList allImgs;
    int selectedNum;
    int  imgNum;//文件总数
    int next;
    int pre;
    int saveAsFlag;
    int breakNum;
    int id;
    QString filename;

    ImageReader *imageReader;

    loadThumbnailThread();

    void setimg(const int &i);
signals:
    void sendThumb(const QImage &, const QImage &, const QString &,
                   /*const int &, */const int &);
protected:
    void run();
};

class Image:public  QImage
{
public:
    Image();
    void painter(QImage img, int x, int y, int w, int h);
};


#endif // LOADTHUMBNAILTHREAD_H
