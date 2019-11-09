#ifndef BIGPICTUREITEM_H
#define BIGPICTUREITEM_H

#include <QGraphicsPixmapItem>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneDragDropEvent>
#include <QRectF>
#include "common.h"
#include <QGraphicsDropShadowEffect>


class smallPictureItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    smallPictureItem(QGraphicsItem *parent = 0):QGraphicsPixmapItem(parent)
    {
    }
    ~smallPictureItem()
    {

    }

    //QSize m_imgSize;
    QImage m_thumbnail;

};

class bigPictureItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:

    bool isZoomed;
    bool isFitWindow;
    QSize imgSize;

    bigPictureItem(const int &imgWidth,
                   const int &imgHeigth,
                   const QPixmap &pixmap,
                   QGraphicsItem *parent = 0)
        :QGraphicsItem(parent),
          isZoomed(false),
          imgSize(-1,-1),
          isFitWindow(true)
    {
        setAcceptDrops(false);
        setZValue(0);
        m_imgWidth = imgWidth;
        m_imgHeigth = imgHeigth;
        pix = pixmap;

        //设置图片阴影
//        QGraphicsDropShadowEffect *shadow_effect = new QGraphicsDropShadowEffect(this);
//        shadow_effect->setOffset(1, 1);
//        shadow_effect->setColor(Qt::gray);
//        shadow_effect->setBlurRadius(8);
//        this->setGraphicsEffect(shadow_effect);
    }

    ~bigPictureItem()
    {}

    const QPixmap &getPixmap()
    {
        return pix;
    }

    QRectF boundingRect() const
    {
         return QRectF(QRectF(-m_imgWidth/2, -m_imgHeigth/2, m_imgWidth, m_imgHeigth));
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget)
    {
        Q_UNUSED(painter);
        Q_UNUSED(option);
        Q_UNUSED(widget);
        painter->drawPixmap(-m_imgWidth/2, -m_imgHeigth/2, m_imgWidth, m_imgHeigth, pix);
    }

    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent * e)
    {
        e->acceptProposedAction();
    }

    virtual void dropEvent(QGraphicsSceneDragDropEvent * e)
    {
    }

private:
    int m_imgWidth;
    int m_imgHeigth;
    QPixmap pix;
};

#endif // BIGPICTUREITEM_H
