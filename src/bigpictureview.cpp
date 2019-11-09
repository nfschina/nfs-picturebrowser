#include "bigpictureview.h"
//#include <QGLWidget>
#include "messagebox.h"

extern QString configList[ROWS];

bigPictureView::bigPictureView(QWidget *parent)
    :QGraphicsView(parent),
      m_isPressed(false),
      m_zoomState(Normal),
      m_imgCurrentWidth(0),
      m_imgCurrentgHeigth(0),
      m_imgHeigth(0),
      m_imgWidth(0),
      scene(NULL),
      m_item(NULL)
{
    m_isStructured = false;
    initDirection();

    QMovie *t_movie = new QMovie(":/prefix1/1-images/loading.gif");
    t_movie->setScaledSize(QSize(20,20));
    t_movie->start();

    m_loadingLb = new QLabel;
    m_loadingLb->setMovie(t_movie);

    m_tipLb = new QLabel;
    m_tipLb->setFrameStyle(QFrame::Raised);
    m_tipLb->setStyleSheet("background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgb(200, 200, 200, 100), stop:1 rgb(20, 20, 20, 100));"
                           "color: rgb(255 , 255, 255);"
                           "border-radius:14px;"
                           "font:16px;"
                           "border-width: 2px;"
                           "border-style: outset;"
                           "border-color: rgb(255, 255, 255);");

    QGraphicsDropShadowEffect *shadow_effect = new QGraphicsDropShadowEffect(this);
    shadow_effect->setOffset(2, 2);
    shadow_effect->setColor(Qt::gray);
    shadow_effect->setBlurRadius(8);
    m_tipLb->setGraphicsEffect(shadow_effect);

    m_tipLb->setAlignment(Qt::AlignCenter);
    m_tipLb->setFixedSize(100, 32);

    m_saveTipLb = new QLabel;
    m_saveTipLb->setFrameStyle(QFrame::Raised);
    m_saveTipLb->setStyleSheet("background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgb(200, 200, 200, 100), stop:1 rgb(20, 20, 20, 100));"
                               "color: rgb(255 , 255, 255);"
                               "border-radius:14px;"
                               "font:16px;"
                               "border-width: 2px;"
                               "border-style: outset;"
                               "border-color: rgb(255, 255, 255);");
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setOffset(2, 2);
    shadowEffect->setColor(Qt::gray);
    shadowEffect->setBlurRadius(8);
    m_saveTipLb->setGraphicsEffect(shadowEffect);

    m_saveTipLb->setAlignment(Qt::AlignCenter);
    m_saveTipLb->setFixedSize(220, 32);

    QWidget *t_loadingWidget = new QWidget;
    t_loadingWidget->setAttribute(Qt::WA_TransparentForMouseEvents);//子窗口鼠标交给父窗口处理
    QVBoxLayout *t_1_vl = new QVBoxLayout;
    t_1_vl->addWidget(m_loadingLb, 0, Qt::AlignCenter);
    t_loadingWidget->setLayout(t_1_vl);

    QWidget *t_tipWidget = new QWidget;
    t_tipWidget->setAttribute(Qt::WA_TransparentForMouseEvents);//子窗口鼠标交给父窗口处理
    QVBoxLayout *t_2_vl = new QVBoxLayout;
    t_2_vl->addWidget(m_tipLb, 0, Qt::AlignCenter);
    t_tipWidget->setLayout(t_2_vl);

    QWidget * t_saveTipWidget = new QWidget;
    t_saveTipWidget->setAttribute(Qt::WA_TransparentForMouseEvents);
    QVBoxLayout *saveTipLayout = new QVBoxLayout;
    saveTipLayout->addWidget(m_saveTipLb, 0, Qt::AlignCenter);
    t_saveTipWidget->setLayout(saveTipLayout);

    m_stackedlayout = new QStackedLayout;
    m_stackedlayout->addWidget(t_loadingWidget);
    m_stackedlayout->addWidget(t_tipWidget);
    m_stackedlayout->addWidget(t_saveTipWidget);

    QVBoxLayout *t_vlayout = new QVBoxLayout;
    t_vlayout->addLayout(m_stackedlayout);
    setLayout(t_vlayout);

    m_1_timer = new QTimer(this);
    m_1_timer->setSingleShot(true);
    connect(m_1_timer, SIGNAL(timeout()), this, SLOT(slot_tipLbHide()));

    //setAcceptDrops(true);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_AcceptDrops, true);
    //setMouseTracking(true);

    setTransformationAnchor(AnchorUnderMouse);
    setResizeAnchor(AnchorViewCenter);
    setViewportUpdateMode(FullViewportUpdate);
    setDragMode(ScrollHandDrag); //可托拽
    setRenderHints(/*QPainter::Antialiasing | */QPainter::SmoothPixmapTransform);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setAlignment(Qt::AlignCenter);
    setFocusPolicy(Qt::NoFocus);
    setStyleSheet("background:transparent; border:0px");
    //setCursor(Qt::ArrowCursor);

// An ImageReader combining QImageReader and GraphicsMagic (if not disabled)
//    imageReader = new ImageReader(true);


    pop_menu = new QMenu(this);
    pop_menu->setAttribute(Qt::WA_TranslucentBackground, true);
    //"font:9pt ""宋体"";
    pop_menu->setStyleSheet("QMenu"
                            "{background-color:rgb(50, 50, 50, 150);"
                            "color:rgb(255,255,255);"
                            "border-radius:3px;"
                            "font:9pt;}"
                            "QMenu::item"
                            "{padding:0px 5px 0px 20px;"//上右下左
                            "margin:0px 0px;"
                            "height:22px;}"
                            "QMenu::item:selected"
                            "{background-color:rgb(100, 100, 100);"
                            "border-radius:3px;}");
    QAction *act1 = new QAction(QIcon(":/prefix1/1-images/action_icon_openfile.png"), tr("&打开图片...                       Ctrl+O"), this);
    //act1->setShortcut(QKeySequence::Open);
    QAction *act2 = new QAction(QIcon(":/prefix1/1-images/action_icon_copy.png"), tr("&复制                                  Ctrl+C"), this);
    //act2->setShortcut(tr("Ctrl+C"));
    QAction *act3 = new QAction(QIcon(":/prefix1/1-images/action_icon_remove.png"), tr("&删除                                  Del"), this);
   // act3->setShortcut(tr("Delete"));
    QAction *act4 = new QAction(QIcon(":/prefix2/2-images/saveAs.png"),tr("&另存为...                           Shift+Ctrl+S"), this);
   // act4->setShortcut(QKeySequence::SaveAs);
    QAction *act5 = new QAction(QIcon(":/prefix1/1-images/action_icon_F5.png"), tr("&刷新                                  F5"), this);
    //act5->setShortcut(tr("F5"));
    QAction *act6 = new QAction(QIcon(":/prefix1/1-images/action_icon_wallerpage.png"), tr("&设为壁纸"), this);
    QAction *act7 = new QAction(QIcon(":/prefix1/1-images/action_icon_setup.png"), tr("&设置"), this);
    QAction *act8 = new QAction(QIcon(":/prefix1/1-images/action_icon_printpreview.png"), tr("&打印预览"), this);
    QAction *act9 = new QAction(QIcon(":/prefix1/1-images/action_icon_print.png"), tr("&打印"), this);

//    QAction *act8 = new QAction(QIcon(":/prefix1/1-images/action_icon_save.png"), tr("&保存                                  Ctrl+S"), this);
    //act8->setEnabled(false);

    pop_menu->addAction(act1);
//    pop_menu->addAction(act8);
    pop_menu->addAction(act4);
    pop_menu->addSeparator();
    pop_menu->addAction(act2);
    pop_menu->addAction(act3);
    pop_menu->addAction(act5);
    pop_menu->addAction(act6);
    pop_menu->addSeparator();
    pop_menu->addAction(act7);
    pop_menu->addSeparator();
    pop_menu->addAction(act8);
    pop_menu->addAction(act9);

    connect(act1, SIGNAL(triggered()), this, SIGNAL(sig_triggeredOpenfile()));
//    connect(act8, SIGNAL(triggered()), this, SIGNAL(sig_triggeredSave()));
    connect(act2, SIGNAL(triggered()), this, SIGNAL(sig_triggeredCopy()));
    connect(act3, SIGNAL(triggered()), this, SIGNAL(sig_triggeredDelete()));
    connect(act4, SIGNAL(triggered()), this, SIGNAL(sig_triggeredSaveas()));
    connect(act5, SIGNAL(triggered()), this, SIGNAL(sig_triggeredRefresh()));
    connect(act6, SIGNAL(triggered()), this, SLOT(slot_setAsWallpaper()));
    connect(act7, SIGNAL(triggered()), this, SIGNAL(sig_triggeredSetup()));
    connect(act8, SIGNAL(triggered()), this, SIGNAL(sig_triggeredPrintPreview()));
    connect(act9, SIGNAL(triggered()), this, SIGNAL(sig_triggeredPrint()));

//    connect(this,SIGNAL(sig_setBackGround(int)),this,SLOT(slot_setBackGround(int)));

}

bigPictureView::~bigPictureView()
{

}

void bigPictureView::itemSetPixmap(QImage img)
{

}

void bigPictureView::setPixmapOnView(const QString &file, const QPixmap &pix, const QSize &size)
{    
    if(m_loadingLb->isHidden())
        m_loadingLb->show();
    initDirection();
    m_currentFile = file;

    if(!pix)
    {
        updataCurrentImg(pix);
    }

    sig_changeIcon(true);

    resizeEvent(0);
}

void bigPictureView::updataDirection()
{
    for(int i=0; i<4; i++)
    {
        if(!imgDirection[i])
            continue;
        if(i<3)
        {
            imgDirection[i] = false;
            imgDirection[i+1] = true;
            break;
        }
        else
        {
            imgDirection[0] = true;
            imgDirection[3] = false;
            break;
        }
    }
}

void bigPictureView::initDirection()
{
    imgDirection[0] = true;
    imgDirection[1] = false;
    imgDirection[2] = false;
    imgDirection[3] = false;
}

Direction bigPictureView::getDirection()
{
    for(int i = 0; i < 4; i++)
    {
        if(imgDirection[i] == true)
        {
            switch (i) {
            case 0:
                return Up;
            case 1:
                return Right;
            case 2:
                return Down;
            case 3:
                return Left;
            default:
                return Unknown;
            }
        }
    }
}

const bool bigPictureView::isRotated()
{
    Direction t_direction = getDirection();

    switch (t_direction) {
    case Up:
        return false;

    case Right:
    case Down:
    case Left:
        return true;
    default:
        return true;
    }
}

void bigPictureView::swap(qreal &a, qreal &b)
{
    a = a + b;
    b = a - b;
    a = a - b;
}

bool bigPictureView::isNeedChangeWH()
{
    Direction t_direction = getDirection();

    switch (t_direction) {
    case Up:
        return false;
    case Right:
        return true;
    case Down:
        return false;
    case Left:
        return true;
    case Unknown:
        return false;
    default:
        return false;
    }
}

void bigPictureView::setImgZoomState()
{
    qreal ratio = m_imgCurrentWidth/m_imgWidth;

    if(ratio <= 0.019999 || m_imgCurrentWidth < 10)
        m_zoomState = Mined;
    else if(ratio >= 20.000)
        m_zoomState = Maxed;
    else
        m_zoomState = Normal;

    //qDebug() << ratio;
}

void bigPictureView::setTipText(const QString &text)
{
    m_1_timer->stop();
    m_tipLb->setText(text);
    m_stackedlayout->setCurrentIndex(1);
//    m_1_timer->start(700);
    m_1_timer->start(1500);

}

void bigPictureView::setSaveTipText(const QString &text)
{
    m_1_timer->stop();
    m_saveTipLb->setText(text);
    m_stackedlayout->setCurrentIndex(2);
    m_1_timer->start(2200);
}

void bigPictureView::updataCurrentImg(const QPixmap &pix)
{
    resetTransform();
    qreal realWidth;
    qreal realHeigth;

    if(m_item != NULL)
        delete m_item;
    if(scene != NULL)
       delete scene;

    //如果在显示模糊图片时发生旋转 图片的宽高发生改变
    if(!isNeedChangeWH())
    {
        realWidth=m_imgWidth;
        realHeigth=m_imgHeigth;
    }
    else
    {
        realWidth=m_imgHeigth;
        realHeigth=m_imgWidth;
    }

    m_item = new bigPictureItem(realWidth, realHeigth,pix);
    scene = new QGraphicsScene;

    //如果是全屏 背景为黑色
    if(parentWidget()->parentWidget()->parentWidget()->parentWidget()->isFullScreen())
    {
        if(configList[0].at(5) == QChar('0'))
            scene->setBackgroundBrush(QColor(0, 0, 0));
        else
            scene->setBackgroundBrush(QColor(0, 0, 0, 150));
    }
    else
    {
        scene->setBackgroundBrush(QColor(235, 241, 255));
    }

    /*if the current file is GIF image*/

//    scene->addItem(m_item);
        QImageReader imageFormat;
        QByteArray format = imageFormat.imageFormat(m_currentFile);
       if(format == "gif")
        {
            QMovie *mv = new QMovie(m_currentFile);
            QLabel *lbl = new QLabel;

            mv->stop();
            mv->setScaledSize(QSize(realWidth,realHeigth));
            mv->start();

            lbl->setMovie(mv);
            lbl->resize(realWidth,realHeigth);
            scene->addWidget(lbl);
        }
        else
        {
            scene->addItem(m_item);
        }

    setScene(scene);
}

void bigPictureView::mouseDoubleClickEvent(QMouseEvent *e)
{
    sig_doubleClick();
}

void bigPictureView::mouseMoveEvent(QMouseEvent *e)
{
    QGraphicsView::mouseMoveEvent(e);

    if(!m_isPressed)
        sig_mapToParentPos(mapToParent(e->pos()));
}

void bigPictureView::mousePressEvent(QMouseEvent *e)
{
    QGraphicsView::mousePressEvent(e);

    sig_singleClick();
    if(e->button() == Qt::LeftButton)
        m_isPressed = true;
}

void bigPictureView::mouseReleaseEvent(QMouseEvent *e)
{
    QGraphicsView::mouseReleaseEvent(e);
    m_isPressed = false;
}

void bigPictureView::fitWindow()
{
    qreal width = this->width();
    qreal height = this->height();

    m_windowRatio = width/height;

    qreal t_WFactor = width/m_imgCurrentWidth;
    qreal t_HFactor = height/m_imgCurrentgHeigth;

    //缩小窗口
    if(m_imgCurrentWidth > width || m_imgCurrentgHeigth > height)
    {
        if(1/t_WFactor > 1/t_HFactor)
        {
            scale(t_WFactor, t_WFactor);
            m_imgCurrentgHeigth = m_imgCurrentgHeigth * t_WFactor;
            m_imgCurrentWidth = m_imgCurrentWidth * t_WFactor;
            //qDebug() << m_imgCurrentWidth << m_imgCurrentgHeigth;
            //qDebug() << width << height;
        }
        else
        {
            scale(t_HFactor, t_HFactor);
            m_imgCurrentWidth = m_imgCurrentWidth * t_HFactor;
            m_imgCurrentgHeigth = m_imgCurrentgHeigth * t_HFactor;
        }

        setImgZoomState(); //得到图片缩放状态
    }//放大窗口
    else if((width > m_imgCurrentWidth && m_imgCurrentWidth < m_imgWidth) ||
            (height > m_imgCurrentgHeigth && m_imgCurrentgHeigth < m_imgHeigth))
    {
        if(t_WFactor < t_HFactor)
        {
            scale(t_WFactor, t_WFactor);
            m_imgCurrentgHeigth = m_imgCurrentgHeigth * t_WFactor;
            m_imgCurrentWidth = m_imgCurrentWidth * t_WFactor;
        }
        else
        {
            scale(t_HFactor, t_HFactor);
            m_imgCurrentWidth = m_imgCurrentWidth * t_HFactor;
            m_imgCurrentgHeigth = m_imgCurrentgHeigth * t_HFactor;
        }

        //如果显示的长 宽大于实际长宽 缩放到实际长宽
        if(m_imgCurrentWidth > m_imgWidth || m_imgCurrentgHeigth > m_imgHeigth)
        {
            qreal factor = m_imgWidth/m_imgCurrentWidth;
            scale(factor, factor);
            m_imgCurrentWidth = m_imgCurrentWidth * factor;
            m_imgCurrentgHeigth = m_imgCurrentgHeigth * factor;
        }

        setImgZoomState(); //得到图片缩放状态
    }
}

void bigPictureView::setBackGround(int value)
{
    if(parentWidget()->parentWidget()->parentWidget()->parentWidget()->isFullScreen())
    {
        if(0 == value)
        {
            scene->setBackgroundBrush(QColor(0,0,0));
        }
        else
        {
            scene->setBackgroundBrush(QColor(0,0,0,150));
        }
    }
}

void bigPictureView::resizeEvent(QResizeEvent *e)
{
    QGraphicsView::resizeEvent(e);

    //双击全屏时 判断是否需要透明背景
    if(parentWidget()->parentWidget()->parentWidget()->parentWidget()->isFullScreen())
    {
        if(configList[0].at(5) == QChar('0'))
            scene->setBackgroundBrush(QColor(0, 0, 0));
        else
            scene->setBackgroundBrush(QColor(0, 0, 0, 150));
    }

    if(m_isStructured && m_item->isZoomed)//缩放过不需要适应窗口
        return;

    fitWindow();

    if(!m_isStructured)
        m_isStructured = true;
}

void bigPictureView::wheelEvent(QWheelEvent *e)
{
    if(!itemAt(e->pos()))//判断如果图片不再光标下返回
        return;

    m_1_timer->stop();
    if(m_item->isFitWindow)
        sig_changeIcon(false); //适应窗口图标

    if(!m_item->isZoomed)
        m_item->isZoomed = true;
    if(m_item->isFitWindow)
        m_item->isFitWindow = false;

    qreal factor = qPow(1.12, e->delta() / 240.0); //1.10的0.5次方

    if(e->delta() > 0)
    {
        if(m_zoomState == Normal || m_zoomState == Mined)
        {
            scale(1.058300, 1.058300);
            m_imgCurrentWidth = m_imgCurrentWidth * 1.058300;
            m_imgCurrentgHeigth = m_imgCurrentgHeigth * 1.058300;

            int t_ratio = m_imgCurrentWidth/m_imgWidth*100;
            setTipText(QString::number(t_ratio) + QString("%"));
        }
        else
        {
            setTipText(QString(tr("已最大")));
        }
    }
    else
    {
        if(m_zoomState == Normal || m_zoomState == Maxed)
        {
            if(m_imgCurrentWidth >= 10 || m_imgCurrentgHeigth >= 10)
            {
                scale(0.944911, 0.944911);
                m_imgCurrentWidth = m_imgCurrentWidth * 0.944911;
                m_imgCurrentgHeigth = m_imgCurrentgHeigth * 0.944911;

                int t_ratio = m_imgCurrentWidth/m_imgWidth*100;
                setTipText(QString::number(t_ratio) + QString("%"));
            }
        }
        else
        {
            setTipText(QString(tr("已最小")));

        }
    }

    setImgZoomState(); //得到图片缩放状态
    e->accept();
}

void bigPictureView::contextMenuEvent(QContextMenuEvent *e)
{
    //菜单出现的位置为当前鼠标的位置
   pop_menu->exec(QCursor::pos());
   e->accept();
}

void bigPictureView::paintEvent(QPaintEvent *e)
{
    QGraphicsView::paintEvent(e);
}

void bigPictureView::dragEnterEvent(QDragEnterEvent *e)
{
    e->acceptProposedAction();    
}

void bigPictureView::dropEvent(QDropEvent *e)
{    
}

void bigPictureView::slot_rotateImage()
{
    if(!m_item->isFitWindow)
        sig_changeIcon(true); //1:1 图标

    m_item->isZoomed = false;  //将缩放标志取消 旋转适应窗口
    m_item->isFitWindow = true;

    updataDirection();

    rotate(90.0);

    qreal t_tmp = m_imgCurrentgHeigth;
    m_imgCurrentgHeigth = m_imgCurrentWidth;
    m_imgCurrentWidth = t_tmp;

    qreal height = this->height();
    qreal width = this->width();

    /***************************************************
     * case1 和case2处理窗口包含图片的情况，包括水平或垂直包含
     * case1 第一次旋转（对于只有窗口只能包含图片垂直或水平一种情况 当第二次进入执行case2）
     * 如果图片水平和垂直都能包含与窗口 则只执行case1 从来不执行case2
     * case3 处理图片任何情况都不能包含与窗口中
    *****************************************************/

    //case1
    if(QRect(0, 0, width, height).contains(QRect(0, 0, m_imgWidth, m_imgHeigth)))
    {
        t_tmp = m_imgHeigth;
        m_imgHeigth = m_imgWidth;
        m_imgWidth = t_tmp;

        qreal factor;
        if(m_imgHeigth >= height && m_imgHeigth <= width)
        {
            factor = height/m_imgCurrentgHeigth;
            scale(factor, factor);
        }
        else if(m_imgWidth > width && m_imgWidth < height)
        {
            factor = width/m_imgWidth;
            scale(factor, factor);
        }
        else //图片比窗口小
        {
            factor = m_imgWidth/m_imgCurrentWidth;
            scale(factor, factor);
        }

        m_imgCurrentWidth = m_imgCurrentWidth*factor;
        m_imgCurrentgHeigth = m_imgCurrentgHeigth*factor;

        setImgZoomState(); //得到图片缩放状态
        return ;
    }

    //case2 第二次旋转
    if(QRect(0,0, width, height).contains(QRect(0, 0, m_imgHeigth, m_imgWidth)))
    {
        t_tmp = m_imgHeigth;
        m_imgHeigth = m_imgWidth;
        m_imgWidth = t_tmp;

        qreal factor;
        if(m_imgWidth >= height && m_imgWidth <= width)
        {
            factor = m_imgWidth/m_imgCurrentWidth;
            scale(factor, factor);
        }
        else if(m_imgHeigth>width && m_imgHeigth <height)
        {
            factor = m_imgHeigth/m_imgCurrentgHeigth;
            scale(factor, factor);
        }

        m_imgCurrentWidth = m_imgCurrentWidth*factor;
        m_imgCurrentgHeigth = m_imgCurrentgHeigth*factor;

        setImgZoomState(); //得到图片缩放状态
        return ;
    }

    t_tmp = m_imgHeigth;
    m_imgHeigth = m_imgWidth;
    m_imgWidth = t_tmp;

    //case 3: 窗口矩形不包含图片
    resizeEvent(0);
    emit sig_triggeredSave();
}

//放大图片
void bigPictureView::slot_largerImage()
{
    m_1_timer->stop();
    if(m_item->isFitWindow)
        sig_changeIcon(false);

    if(!m_item->isZoomed)
        m_item->isZoomed = true;
    if(m_item->isFitWindow)
        m_item->isFitWindow = false;

    if(m_zoomState == Normal || m_zoomState == Mined)
    {
        scale(1.058300, 1.058300);
        m_imgCurrentWidth = m_imgCurrentWidth * 1.058300;
        m_imgCurrentgHeigth = m_imgCurrentgHeigth * 1.058300;

        int t_ratio = m_imgCurrentWidth/m_imgWidth*100;
        setTipText(QString::number(t_ratio) + QString("%"));
    }
    else
    {
        setTipText(QString(tr("已最大")));
    }

    setImgZoomState(); //得到图片缩放状态
}

void bigPictureView::slot_reduceImage()
{
    m_1_timer->stop();
    if(m_item->isFitWindow)
        sig_changeIcon(false); //修改图标信号

    if(!m_item->isZoomed)
        m_item->isZoomed = true;
    if(m_item->isFitWindow)
        m_item->isFitWindow = false;

    if(m_zoomState == Normal || m_zoomState == Maxed)
    {
        if(m_imgCurrentWidth >= 10 || m_imgCurrentgHeigth >= 10)
        {
            scale(0.944911, 0.944911);
            m_imgCurrentWidth = m_imgCurrentWidth * 0.944911;
            m_imgCurrentgHeigth = m_imgCurrentgHeigth * 0.944911;

            int t_ratio = m_imgCurrentWidth/m_imgWidth*100;
            setTipText(QString::number(t_ratio) + QString("%"));
        }
    }
    else
    {
        setTipText(QString(tr("已最小")));
    }

    setImgZoomState(); //得到图片缩放状态
}

void bigPictureView::slot_setAsWallpaper()
{
    QProcess *proc = new QProcess;
    QString currentFile = m_currentFile;
    bool succeedFlag = false;
    int haveSpave = 0;
    int len = currentFile.length();
//check if the currentfile contain space or not
    for(int i=0;i<len;i++)
    {
        if(currentFile.at(i).isSpace())
        {
            haveSpave=1;
            break;
        }
    }

//if the current file contain space
    if(haveSpave)
    {
        QDir*pictureBrowser = new QDir;
        bool exist = pictureBrowser->exists(QDir::homePath()+"/"+PICTUREBROWSER);
        if(exist){}
        else
        {
            succeedFlag=pictureBrowser->mkdir(QDir::homePath()+"/"+PICTUREBROWSER);
        }
        currentFile=QDir::homePath()+"/"+PICTUREBROWSER+"/"+"wallpaper";
//need to delete last wallpaper
        QFile::remove(currentFile);
        succeedFlag = QFile::copy(m_currentFile,currentFile);
    }

//need to confirm the desktop is gnome/unity
    QString str = "gsettings set org.cdos.desktop.background picture-uri 'file://"+currentFile+"'";
    proc->start(str);
}

void bigPictureView::slot_tipLbHide()
{
    m_stackedlayout->setCurrentIndex(0);
}

// 接受线程发来的原图片 替换缩略图
void bigPictureView::slot_receiveOriImg(const QImage &img,const QString &filename)
{
//    if(m_currentFile != filename) //view 要显示的图片 和 线程发过来的要相同
//        return;

    bool zoom = m_item->isZoomed;         //保存模糊图片状态
    bool fitWindow = m_item->isFitWindow;

    this->initDirection();
    m_currentFile = filename;
    QImageReader imgReader(filename);
    m_imgCurrentWidth = m_imgWidth  = imgReader.size().width();
    m_imgCurrentgHeigth = m_imgHeigth = imgReader.size().height();

    sig_changeIcon(true);
    resizeEvent(0);

    updataCurrentImg(QPixmap::fromImage(/*currentImg*/img));

    //判断是否发生旋转(如果在显示模糊图片是点击了旋转， 原图也要做相应的旋转)
    Direction t_direction = getDirection();
    qreal factor;

    switch (t_direction) {
    case Up:
        factor = m_imgCurrentWidth/img.width();
        break;

    case Right:
        rotate(90.0);
        factor = m_imgCurrentgHeigth/img.width();
        break;

    case Down:
        rotate(180.0);
        factor = m_imgCurrentWidth/img.width();
        break;

    case Left:
        rotate(270.0);
        factor = m_imgCurrentgHeigth/img.width();
        break;

    case Unknown:
        std::cout << "Direction Unknown" << std::endl;
        break;
    default:
        std::cout << "Direction Error" << std::endl;
        break;
    }

    scale(factor, factor);

    m_item->isZoomed = zoom;
    m_item->isFitWindow = fitWindow;

    m_loadingLb->hide();
}

void bigPictureView::scaleToOrg()
{
    qreal factor = m_imgWidth/m_imgCurrentWidth;
    scale(factor, factor);

    m_imgCurrentWidth = m_imgWidth;
    m_imgCurrentgHeigth = m_imgHeigth;
}

void bigPictureView::slot_oneToOne()
{
    if(m_item->isFitWindow)          //当前是适应窗口的 缩放至原图
    {
        sig_changeIcon(false);

        scaleToOrg();
        setImgZoomState(); //得到图片缩放状态

        m_item->isFitWindow = false;
        m_item->isZoomed = true;
    }
    else                             //当前不是适应窗口的  缩放至适应窗口
    {
        sig_changeIcon(true);
        //如果原图的大小 比窗口小 缩放至原图
        if(m_imgWidth < this->width() && m_imgHeigth < this->height())
        {
            scaleToOrg();
        }
        else              //如果原图比窗口大  缩放之适应窗口
            fitWindow();

        m_item->isFitWindow = true;
        m_item->isZoomed = false;
    }

}




