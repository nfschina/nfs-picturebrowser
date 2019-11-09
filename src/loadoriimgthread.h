#ifndef THBTHREAD_H
#define THBTHREAD_H

#include <QThread>
#include <QFileInfo>
//#include <QtDebug>
#include <QDateTime>
#include <QDir>
#include <QImage>
#include "common.h"

class loadOriImgThread : public QThread {

	Q_OBJECT

public:
    //static int m_threadNum;

    QString currentFile;
    ImageReader *imageReader;
    QImage img;
    loadOriImgThread(QThread *parent = 0) : QThread(parent)
    {
        imageReader = new ImageReader(true);
    }

signals:
    void sig_sendImg(QImage, QString);
    void sig_sendPdi(const QString &, const QString &);

protected:
    void run()
    {
        img = imageReader->readImage(currentFile);
        sig_sendImg(img, imageReader->fileChanged);
        sig_sendPdi(QString::number(img.logicalDpiX()),
                    QString::number(img.logicalDpiY()));
	}
};

#endif // THBTHREAD_H
