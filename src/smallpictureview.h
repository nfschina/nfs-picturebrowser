#ifndef SMALLPICTUREVIEW_H
#define SMALLPICTUREVIEW_H

#include <QGraphicsView>
#include <QFileInfo>
#include "common.h"
#include "loadthumbnailthread.h"
#include "pictureitem.h"
#include "define.h"
#include <QTimeLine>

class smallPictureView : public QGraphicsView
{
    Q_OBJECT
public:

    showOrhide getShowOrHide();
    void setShowOrHide(showOrhide);

    smallPictureView(QWidget *parent = 0);
    ~smallPictureView();

private:
    showOrhide m_flag;
public:
    int wheelFlag;
    int imgNum;//当前文件夹图片个数
    int widwidth;
    int widheight;
    int currentThreadId;

    QString currentfile ;         //当前选中的图片文件
    QFileInfoList allImgs;        //图片文件对象链表//存储当前文件夹所有图片的信息
    QList<QString> allImgsName;   //判断线程发来的图片用 //存储当前文件夹所有图片名称

    QGraphicsScene scene;         //缩略图的场景

    smallPictureItem *midPixItemFlag;//中间大图片的item中间变量

    QList<smallPictureItem *> midPixItems; //所有大图的item链表
    QList<smallPictureItem *> allPixItem;  //缩略图位移变量
    QList<QGraphicsItemAnimation *> allAnimation;

    loadThumbnailThread * ThumbThreadpre;//线程从当前位置往前加载缩略图
    loadThumbnailThread * ThumbThreadnext;//线程从当前位置往后加载缩略图

    loadThumbnailThread * saveAsThread;//图片另存为的线程

    int selectedNum;//当前显示的图片编号
    int oldSelectNum;

    void setItem();//读取当前文件所有图片
    void setItemPos(int i);
    void movItemPos(QTimeLine *, int);
    void changeImg(int time);
    QTimeLine *timeLine;

public slots:
    void slot_itemAddImgpre(const QImage &, const QImage &,
                      const QString &, /*const int &, */const int &);
    void slot_itemAddImgnext(const QImage &, const QImage &,
                        const QString &, /*const int &, */const int &);

    void timelineFinished();

protected:
    void mousePressEvent(QMouseEvent *);//鼠标点击事件
    void wheelEvent(QWheelEvent *e);//滑轮事件
    void mouseDoubleClickEvent(QMouseEvent *e);

signals:
    void sig_sendToDisplay(int num);

public slots:
    void slot_updatePos();
};

#endif // SMALLPICTUREVIEW_H
