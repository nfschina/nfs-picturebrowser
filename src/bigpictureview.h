#ifndef BIGPICTUREVIEW_H
#define BIGPICTUREVIEW_H

#include "common.h"
#include "define.h"
#include "pictureitem.h"
#include <qmath.h>
#include <QWheelEvent>
#include <QContextMenuEvent>
#include <QAction>
#include <QMenu>
#include <QProcess>
#include <QMessageBox>
#include <QThread>
#include <QMatrix>
#include <QTransform>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QUrl>
#include <QMimeData>
#include "slidedialog.h"
#include "setupdialog.h"
#include"formats/imagereader.h"
#include "formats/fileformats.h"
#include <QWidgetAction>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QtPrintSupport/QPrintDialog>


class bigPictureView : public QGraphicsView
{
    Q_OBJECT
public:

    enum ZoomState
    {
        Mined,
        Normal,
        Maxed
    };

    QStackedLayout *m_stackedlayout;
    QLabel *m_loadingLb;
    QLabel *m_tipLb;
    QLabel *m_saveTipLb;
    QString m_currentFile;    //换壁纸用
    bool m_isPressed;
    bool m_isStructured;
    bigPictureItem *m_item;
    qreal m_imgCurrentWidth;    //当前图宽
    qreal m_imgCurrentgHeigth;
    qreal m_imgWidth;           //原图宽
    qreal m_imgHeigth;
    qreal m_windowRatio;
    QGraphicsScene *scene;
    bool imgDirection[4];

    QMenu *pop_menu;
    ZoomState m_zoomState;      //图片缩放状态

    QTimer *m_1_timer;

    bigPictureView(QWidget *parent = 0);
    ~bigPictureView();

    void fitWindow();
    void scaleToOrg();
    void itemSetPixmap(QImage);
    void setPixmapOnView(const QString &file,const QPixmap &pix, const QSize &size);
    void updataDirection();
    void initDirection();
    Direction getDirection();
    const bool isRotated();
    void swap(qreal &, qreal &);
    bool isNeedChangeWH();
    void setImgZoomState();
    void setTipText(const QString &);
    void setSaveTipText(const QString&);

    void updataCurrentImg(const QPixmap &pix);

    void mouseDoubleClickEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void resizeEvent(QResizeEvent *e);
    void wheelEvent(QWheelEvent *e);
    void contextMenuEvent(QContextMenuEvent *e);
    void paintEvent(QPaintEvent *e);

    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);

    void setBackGround(int value);

private:    
    QMovie *mov;

signals:
    void sig_doubleClick();
    void sig_singleClick();
    void sig_mapToParentPos(QPoint p);
    //void sig_threadOver();
    void sig_changeIcon(bool);    //改变1:1按钮的图片
    void sig_triggeredDelete();
    void sig_triggeredOpenfile();
    void sig_triggeredSaveas();
    void sig_triggeredCopy();
    void sig_triggeredSetup();
    void sig_triggeredRefresh();
    void sig_triggeredSave();
    void sig_triggeredPrintPreview();
    void sig_triggeredPrint();


public slots:
    void slot_rotateImage();
    void slot_largerImage();
    void slot_reduceImage();
    void slot_setAsWallpaper();
    void slot_tipLbHide();
    void slot_receiveOriImg(const QImage &img, const QString &filename);
    void slot_oneToOne();

};

#endif // BIGPICTUREVIEW_H
