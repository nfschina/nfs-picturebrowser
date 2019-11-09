#ifndef IMAGEREADER
#define IMAGEREADER



#include <iostream>
#include <QObject>
#include <QImageReader>
#include <QFileInfo>
#include <QDir>
#include <QTextDocument>
#include <QPixmap>
#include <QPainter>
#include <QtSvg/QtSvg>

#include "formats/fileformats.h"
//#include <GraphicsMagick/Magick++/Image.h>
#include "psd2png.h"

class ImageReader : public QObject {

    Q_OBJECT

public:
    ImageReader(bool v);

    QString qtfiles;
    QString gmfiles;
    QString extrasfiles;

    QString picSize;

    float scaleImg1;
    float scaleImg2;

    bool animatedImg;
    QString fileformat;
    QSize origSize;

    QImage readImage(QString filename);
//    void readImage(QString filename);
    QString fileChanged;

private:
    bool verbose;
    FileFormats *fileFormats;


    QImage readImage_QT(QString filename);
    QImage readImage_GM(QString filename);
    QImage readImage_XCF(QString filename);

//    bool doIUseMagick(QString filename);
    QString whatDoIUse(QString filename);

};



#endif // IMAGEREADER

