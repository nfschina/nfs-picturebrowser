#ifndef MAINTASKWIDGET_H
#define MAINTASKWIDGET_H

#include <QWidget>
#include "common.h"
#include "define.h"
#include "bottomwidget.h"
#include "pictureitem.h"
#include "bigpictureview.h"
#include "loadoriimgthread.h"
#include "loadfileobj.h"
#include "messagebox.h"
#include "slidedialog.h"
#include <QBuffer>
#include "picinfowidget.h"
#include<QtWebKitWidgets/QWebView>
#include <QDesktopServices>
#include "fileinfothread.h"
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QtPrintSupport/QPrintDialog>


#if 0
/*about OSS SDK*/
#include <oss_c_sdk/aos_log.h>
#include "oss_c_sdk/aos_util.h"
#include "oss_c_sdk/aos_string.h"
#include "oss_c_sdk/aos_status.h"
#include "oss_c_sdk/oss_auth.h"
#include <oss_c_sdk/oss_util.h>
#include <oss_c_sdk/oss_api.h>
//#include "oss_c_sdk/oss_config.h"
//#include "oss_c_sdk/oss_sample_util.h"
#include <oss_c_sdk/aos_http_io.h>
#include <oss_c_sdk/aos_string.h>
#include <oss_c_sdk/aos_transport.h>
#include <oss_c_sdk/oss_define.h>

#endif if


class ImageReader;
class saveAsThread : public QThread {

    Q_OBJECT

public:

    QString m_filename;
    QPixmap m_pixmap;
    bool stateFlag;

    saveAsThread(const QString &filename, const QPixmap &pix, QObject *parent = 0)
    {
        Q_UNUSED(parent);
        m_filename = filename;
        m_pixmap = pix;
    }

protected:
    void run()
    {
        //将QString类型转化为 char *
        QString  str;
        char* ch;
        QByteArray ba = m_filename.toLatin1();
        ch = ba.data();

        //得到图片的格式
        char *format = strrchr(ch, '.');

        //将文件首先写放入缓冲区
        QByteArray bytes;
        QBuffer buffer(&bytes);
        if(buffer.open(QIODevice::WriteOnly))
        {
            m_pixmap.save(&buffer, format+1);
        }
        else
        {
            std::cout << "saveAsThread open QBuffer failed" << std::endl;
            return;
        }

        //将缓冲区的文件写入磁盘
        QFile t_file(m_filename);//创建一个文件对象，存储用户选择的文件
        if (t_file.open(QIODevice::ReadWrite))
        {
            t_file.write(bytes);
            t_file.close();
            stateFlag = true;
        }
        else
        {
            std::cout << "saveAsThread create QFile failed" << std::endl;
            stateFlag = false;
        }

    }
};

class scrolledButtonWidget : public QWidget
{
    Q_OBJECT

public:

    bool m_isShowed;
    QGraphicsOpacityEffect *m_effect;
    qreal m_transparentValue;
    QTimer *m_1_timer;
    QTimer *m_2_timer;
    QPushButton *m_direction;
    QHBoxLayout *m_1_hl;

    scrolledButtonWidget(Direction LorR, QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~scrolledButtonWidget();


    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);

signals:

    void sig_showAnotherDirectionButton();
    void sig_hideAnotherDirectionButton();

public slots:

    void slot_increaseTransparency();
    void slot_reduceTransparency();
    void slot_showDirectionButton();
    void slot_hideDirectionButton();
};


class mainTaskWidget : public QWidget
{
    Q_OBJECT
public:

    enum cursorPos
    {
        LeftPageRt,
        RightPageRt,
        BottomCtlRt,
        InnerRt,
        openPicInfo_InnerRt,
        openPicInfo_BottomCtlRt,
        openPicInfo_LeftPageRt,
        openPicInfo_RightPageRt,
        Unknown
    };

    bool m_isLoaded;                                  //当前图片是否加载过了
    bool picInfoWidgetFlag;

    QHBoxLayout* m_1_hl;
    QString m_currentfile;                          //当前显示的图片的绝对路径
    QString m_orignalFile;
    QStringList m_allImagesName;                    //所有图片的名字
    QFileInfoList m_allImagesInfo;                  //所有图片文件信息

    int m_imagesNum;       //图片总个数
    int m_currentNum;       //但前显示图片在列表中的编号

    loadOriImgThread *m_loadOriImgThread;
    QList<loadOriImgThread *> allThread;

    const int numberOfThreads = 1;
    QVector<loadOriImgThread *> *m_loadOriImgThreadArray;
    saveAsThread *m_saveAsThread;
    QList<saveAsThread *> m_saveAsThreadlist;

    fileInfoThread* filesInformationThread;


    cursorPos m_cursorPosF;

    int mainTaskWidgetWidth;
    int mainTaskWidgetHeight;

    qreal m_windowRatio;

    QRect m_regions[6];

    bottomWidgt *m_bottomWidget;
     picInfoWidget *m_PicInfoWidget;
     showOrhide sOhFlag;


    scrolledButtonWidget *m_leftPage;
    scrolledButtonWidget *m_rightPage;

    bigPictureView *m_view;

    QProcess *m_beautifyProc;

    SelectBox *m_box;
    setUpDialog *t_setUpDialog;

// imagereader combining QImageReader and GraphicsMagic
    ImageReader *imageReader;
    slideDialog *slide;

    int xOffset;
    int yOffset;
    bool bottomAreaState;
    bool sideAreaState;
    bool normalSet4State;

    bool isFileInfoThreadFinshed;

    explicit mainTaskWidget(QWidget *parent = 0);
    ~mainTaskWidget();


    //打开线程加载所有原图片
    void startLoadOriImgThread();

    //打开线程加载所有缩略图
    void startLoadThumbnaiThread();
    void _startLoadThumbnaiThread();
    void getAllImagesPath();
    QSize getImageSize(const QString &);
    void removeImg();

    void resizeEvent(QResizeEvent *e);
    void leaveEvent(QEvent *e);

    void savePicture(Direction);
    QString &fixFilename(QString &);
    void saveAsRotatedImg(const QPixmap &);
    void changePicure();
    QPixmap getRotatedOrgPix(Direction direction);
    void setSelectBoxPos();       //窗口拖拉改变大小时 selectbox做相应的移动
    void loadWeiboShare();//load weibo sharing
    QWebView*shareWeb;
    void loadGimpTool();// 加载GIMP图片编辑软件
signals:
    void sig_showDirectionKey();
    void sig_hideLeftAndRight();
    void sig_showBottomCtl();
    void sig_hideToolBar();
    void sig_showBottomWidget();
    void sig_hideBottomWidget();

    void sig_sendToInfoWidget(const QFileInfo &, const QStringList &);

    void sig_removeImg();
    void sig_changePage();


    void sig_saveAsDialog();
    void sig_saveDialog();
    void sig_picInfoOpen();
    void sig_picInfoClose();
    void sig_normalSetting1(bool ischecked);


public slots:
    void slot_determineCursorPos(const QPoint &);
    void slot_receivePicFile(const QString &);
    void slot_prev();
    void slot_next();
    void slot_openFile();
    void slot_removeImg();
    void slot_copyImg();
    void slot_saveAs();
    void slot_saveAsDialog();
    void slot_refresh();
    void slot_save();
    void slot_saveDialog();
    void slot_showOrHidePicInfoWidget();

    void slot_updataThreadNum();
    void slot_receiveFromView(const int &num); //处理缩略图发来的序号 显示图片
    void slot_reciveImgInfo(const QString &, const QString &);
    void slot_showSlide();
    void slot_opensetUpDialog();
    void slot_slideQuit(int); //幻灯片退出
    void slot_finishSave();
    void slot_setAllItemPos();
    void slot_openSelectbox();//打开编辑工具选项栏
    void slot_OpenPictureEditingTool(int); //打开“图片编辑”和“gimp”
    void slot_sharePicture();
    void slot_radioBtnCheck();
    void slot_fileInfoThreadFinshed();

	void slot_picInfoOpen();
    void slot_normalSetting2(bool ischecked);
    void slot_normalSetting3(bool ischecked);
    void slot_normalSetting4(bool ischecked);
    void slot_normalSetting5(bool ischecked);


    void slot_recvSlideSetInfo(QString setInfo);
	void slot_picInfoClose();

    void slot_printpreview();
    void slot_print();
    void  printPreviewSlot(QPrinter *printerPixmap);

};

#endif // MAINTASKWIDGET_H

