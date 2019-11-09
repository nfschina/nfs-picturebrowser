#include "maintaskwidget.h"
#include<qurl.h>
#include<QFile>
//#include <gio/gio.h>

extern QString configList[ROWS];

scrolledButtonWidget::scrolledButtonWidget(Direction LorR, QWidget *parent, Qt::WindowFlags f)
    :QWidget(parent, f)
{
    setFixedSize(80, 54);
    setAutoFillBackground(true);

    if(configList[0].at(2) == QChar('1'))
    {
        m_transparentValue = 1;
        m_isShowed = true;
    }
    else
    {
        m_transparentValue = 0;
        m_isShowed = true;
        hide();
    }

    m_effect = new QGraphicsOpacityEffect(this);
    m_effect->setOpacity(m_transparentValue);
    setGraphicsEffect(m_effect);

    QPalette t_pal;
    t_pal.setColor(QPalette::Window, QColor(255, 255, 255,0));
    this->setPalette(t_pal);

    m_1_timer = new QTimer(this);
    connect(m_1_timer, SIGNAL(timeout()), this, SLOT(slot_reduceTransparency()));

    m_2_timer = new QTimer(this);
    connect(m_2_timer, SIGNAL(timeout()), this, SLOT(slot_increaseTransparency()));

    m_direction = new QPushButton;
    m_direction->setAutoRepeat(true);
    m_direction->setFixedSize(54, 54);
    m_direction->setFocusPolicy(Qt::NoFocus);
    m_1_hl = new QHBoxLayout;
    m_1_hl->setMargin(0);

    switch (LorR) {
    case Left:
        m_1_hl->addStretch(5);
        m_1_hl->addWidget(m_direction);
        m_1_hl->addStretch(2);
        m_direction->setToolTip(tr("上一张"));
        m_direction->setStyleSheet("QPushButton{border-image:url(:/prefix2/2-images/leftPage.png);}"
                                    "QPushButton:hover{border-image:url(:/prefix2/2-images/leftPage_hover.png);}"
                                   "QPushButton:pressed{border-image:url(:/prefix2/2-images/leftPage.png);}");
        break;
    case Right:
        m_1_hl->addStretch(2);
        m_1_hl->addWidget(m_direction);
        m_1_hl->addStretch(5);
        m_direction->setToolTip(tr("下一张"));
        m_direction->setStyleSheet("QPushButton{border-image:url(:/prefix2/2-images/rightPage.png);}"
                         "QPushButton:hover{border-image:url(:/prefix2/2-images/rightPage_hover.png);}"
                                   "QPushButton:pressed{border-image:url(:/prefix2/2-images/rightPage.png);}");
        break;
    default:
        break;
    }

    this->setLayout(m_1_hl);

}

scrolledButtonWidget::~scrolledButtonWidget()
{

}

void scrolledButtonWidget::enterEvent(QEvent *e)
{

}

void scrolledButtonWidget::leaveEvent(QEvent *e)
{

}

void scrolledButtonWidget::slot_increaseTransparency()
{
    if(m_transparentValue >= 0)
    {
        m_transparentValue -= 0.01;
        m_effect->setOpacity(m_transparentValue);
    }
    else
    {
        m_2_timer->stop();
        this->hide();
    }
}

void scrolledButtonWidget::slot_reduceTransparency()
{
    if(m_transparentValue <= 1)
    {
        m_transparentValue += 0.01;
        m_effect->setOpacity(m_transparentValue);

    }
    else
    {
        m_1_timer->stop();
    }
}

void scrolledButtonWidget::slot_showDirectionButton()
{
    if(!m_isShowed)
    {
        sig_showAnotherDirectionButton();
        this->show();
        m_2_timer->stop();
        m_1_timer->start(0.5);
        m_isShowed = true;
    }
}

void scrolledButtonWidget::slot_hideDirectionButton()
{
    if(configList[0].at(2) == QChar('1'))
        return;

    if(m_isShowed)
    {
        sig_hideAnotherDirectionButton();
        m_1_timer->stop();        
        m_2_timer->start(7);
        m_isShowed = false;
    }
}

mainTaskWidget::mainTaskWidget(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);

    setMouseTracking(true);
    setAttribute(Qt::WA_TranslucentBackground, true);
//    this->setFocusPolicy(Qt::NoFocus);

    m_currentNum = -1;
    m_imagesNum = -1;
    m_loadOriImgThread = NULL;
    m_saveAsThread = NULL;
    m_isLoaded = false;
    m_beautifyProc = new QProcess;
    m_box = NULL;

    m_view = new bigPictureView;

    m_PicInfoWidget = new picInfoWidget(this);//图片信息窗口
    m_PicInfoWidget->hide();
    sOhFlag=Hide;
    picInfoWidgetFlag = false;

    m_1_hl = new QHBoxLayout;
    m_1_hl->setMargin(0);
    m_1_hl->setSpacing(0);
    m_1_hl->addWidget(m_PicInfoWidget);
    m_1_hl->addWidget(m_view);

    this->setLayout(m_1_hl);

    m_leftPage = new scrolledButtonWidget(Left, this);
    m_rightPage = new scrolledButtonWidget(Right, this);
//    m_rightPage->m_direction->setDisabled(true);
//    m_leftPage->m_direction->setDisabled(true);

    bottomAreaState = 0;
    sideAreaState = 0;
    normalSet4State=configList[0].at(3) == QChar('1')?true:false;

    //初始化加载缩略图线程内容 此线程在slidedialog中new
    this->slide = new slideDialog(this);
    //退出幻灯片应该显示的图片
    connect(this->slide, SIGNAL(sig_sendCurrentNum(int)), this, SLOT(slot_slideQuit(int)));


    //m_toolbar = new toolBarWidget(this);
    m_bottomWidget = new bottomWidgt(this);
//    m_bottomWidget->m_toolbar->m_left->setDisabled(true);
//    m_bottomWidget->m_toolbar->m_right->setDisabled(true);


// An ImageReader combining QImageReader and GraphicsMagic (if not disabled)
    imageReader = new ImageReader(true);

    filesInformationThread = new fileInfoThread();
    this->isFileInfoThreadFinshed = false;
    connect(this->filesInformationThread,SIGNAL(finished()),this,SLOT(slot_fileInfoThreadFinshed()));

    connect(m_bottomWidget->m_toolbar->m_picInfo, SIGNAL(clicked()), this, SLOT(slot_showOrHidePicInfoWidget()));
    connect(this,SIGNAL(sig_sendToInfoWidget(QFileInfo,QStringList)),
            m_PicInfoWidget,SLOT(receiveThreadMsg(QFileInfo,QStringList)));


    m_loadOriImgThreadArray = new QVector<loadOriImgThread *>(numberOfThreads);
    for(int i=0; i<numberOfThreads; i++)
    {
        m_loadOriImgThreadArray->replace(i, new loadOriImgThread);

        connect(m_loadOriImgThreadArray->at(i), SIGNAL(finished()), this, SLOT(slot_updataThreadNum()));
        connect(m_loadOriImgThreadArray->at(i), SIGNAL(sig_sendImg(const QImage &, const QString &)),
                m_view, SLOT(slot_receiveOriImg(const QImage &, const QString &)));
        connect(m_loadOriImgThreadArray->at(i),SIGNAL(sig_sendPdi(const QString &, const QString &)),
                this,SLOT(slot_reciveImgInfo(const QString &, const QString &)));
    }

    connect(m_rightPage, SIGNAL(sig_showAnotherDirectionButton()), m_leftPage, SLOT(slot_showDirectionButton()));
    connect(this, SIGNAL(sig_showDirectionKey()), m_rightPage, SLOT(slot_showDirectionButton()));

    connect(m_leftPage, SIGNAL(sig_hideAnotherDirectionButton()), m_rightPage, SLOT(slot_hideDirectionButton()));
    connect(this, SIGNAL(sig_hideLeftAndRight()), m_leftPage, SLOT(slot_hideDirectionButton()));

    connect(m_view, SIGNAL(sig_mapToParentPos(QPoint)), this, SLOT(slot_determineCursorPos(QPoint)));

    connect(this, SIGNAL(sig_showBottomCtl()), m_bottomWidget, SLOT(slot_showAnimation()));
    connect(this, SIGNAL(sig_hideToolBar()), m_bottomWidget, SLOT(slot_hideAnimation()));

    //切换图片信号链接
    connect(m_leftPage->m_direction, SIGNAL(clicked()), this, SLOT(slot_prev()));
    connect(m_rightPage->m_direction, SIGNAL(clicked()), this, SLOT(slot_next()));

    //工具栏信号链接
    //打开缩略图时 设置item的位置
    connect(m_bottomWidget, SIGNAL(sig_needSetItemPos()), this, SLOT(slot_setAllItemPos()));

    //normal bar
    connect(m_bottomWidget->m_toolbar->m_file, SIGNAL(clicked()), this, SLOT(slot_openFile()));
    connect(m_bottomWidget->m_toolbar->m_left, SIGNAL(clicked()), this, SLOT(slot_prev()));
    connect(m_bottomWidget->m_toolbar->m_right, SIGNAL(clicked()), this, SLOT(slot_next()));
    connect(m_bottomWidget->m_toolbar->m_rotating, SIGNAL(clicked()), m_view, SLOT(slot_rotateImage()));
    connect(m_bottomWidget->m_toolbar->m_magnifying, SIGNAL(clicked()), m_view, SLOT(slot_largerImage()));
    connect(m_bottomWidget->m_toolbar->m_reduce, SIGNAL(clicked()), m_view, SLOT(slot_reduceImage()));
    connect(m_bottomWidget->m_toolbar->m_slide, SIGNAL(clicked()), this, SLOT(slot_showSlide()));
    connect(m_bottomWidget->m_toolbar->m_one_one, SIGNAL(clicked()), m_view, SLOT(slot_oneToOne()));
//open editor tool
    connect(m_bottomWidget->m_toolbar->m_beautify, SIGNAL(clicked()), this, SLOT(slot_openSelectbox()));
//for sharing function
    connect(m_bottomWidget->m_toolbar->m_share, SIGNAL(clicked()), this, SLOT(slot_sharePicture()));
//change current picture to onetoone
    connect(m_view, SIGNAL(sig_changeIcon(bool)), m_bottomWidget->m_toolbar, SLOT(slot_changeOnetoOneIcon(bool)));
//delete current picture
    connect(m_bottomWidget->m_toolbar->m_delete, SIGNAL(clicked()), this, SLOT(slot_removeImg()));

    connect(this, SIGNAL(sig_removeImg()), m_bottomWidget->m_thumbnailView, SLOT(slot_updatePos()));
    connect(m_view, SIGNAL(sig_triggeredDelete()), this, SLOT(slot_removeImg()));
    connect(m_view, SIGNAL(sig_triggeredOpenfile()), this, SLOT(slot_openFile()));
    connect(m_view, SIGNAL(sig_triggeredCopy()), this, SLOT(slot_copyImg()));
    connect(m_view, SIGNAL(sig_triggeredSaveas()), this, SLOT(slot_saveAs()));
    connect(m_view, SIGNAL(sig_triggeredSetup()), this, SLOT(slot_opensetUpDialog()));
    connect(m_view, SIGNAL(sig_triggeredRefresh()), this, SLOT(slot_refresh()));
    connect(m_view, SIGNAL(sig_triggeredSave()), this, SLOT(slot_save()));
    connect(m_view, SIGNAL(sig_triggeredPrintPreview()), this, SLOT(slot_printpreview()));
    connect(m_view, SIGNAL(sig_triggeredPrint()), this, SLOT(slot_print()));

    connect(m_bottomWidget->m_thumbnailView,SIGNAL(sig_sendToDisplay(int)),
            this,SLOT(slot_receiveFromView(int)));

    connect(this, SIGNAL(sig_saveDialog()), this, SLOT(slot_saveDialog()));
    connect(this, SIGNAL(sig_saveAsDialog()), this, SLOT(slot_saveAsDialog()));
    connect(this, SIGNAL(sig_picInfoOpen()), this, SLOT(slot_picInfoOpen()));
    connect(this, SIGNAL(sig_picInfoClose()), this, SLOT(slot_picInfoClose()));
}
void mainTaskWidget::slot_printpreview()
{
    QPrinter printer(QPrinter::HighResolution);
        //自定义纸张大小
        printer.setPageSize(QPrinter::Custom);


        QPrintPreviewDialog preview(&printer, this);
        preview.setMinimumSize(1000,600);
        connect(&preview, SIGNAL(paintRequested(QPrinter*)), SLOT(printPreviewSlot(QPrinter*)));
        preview.exec ();
}

void mainTaskWidget::slot_print()
{
    QPrinter printer;       //---构建新对象
    QPrintDialog printDlg(&printer, this);
    QString filename = m_currentfile;
    qDebug()<<"111111111111111111:"<<filename;
    QPixmap pixmap ;
    pixmap.load(filename);
    if (printDlg.exec())            // 用于判断用户是否点击“打印”按钮
    {
        QPainter painter(&printer);
        QSize size = pixmap.size();            //--获取图片的大小
        painter.begin(&printer);
        QRect rect = painter.viewport();
        int x = rect.width() / pixmap.width();
        int y = rect.height() / pixmap.height();
        size.scale(x,y, Qt :: KeepAspectRatio);     //----按照图形比例大小重新设置视口矩形区域
        //---打印图片
        painter.setWindow(pixmap.rect());
        painter.drawPixmap(0, 0, pixmap);
    }

}

void mainTaskWidget::printPreviewSlot(QPrinter *printerPixmap)
{
   // printerPixmap->setOrientation(QPrinter::Landscape);
    //获取界面的图片
    QPixmap pixmap;
    pixmap.load(m_currentfile);
    QPainter painterPixmap(this);
    painterPixmap.begin(printerPixmap);
    QRect rect = painterPixmap.viewport();
    int x = rect.width() / pixmap.width();
    int y = rect.height() / pixmap.height();
    painterPixmap.scale(x, y);
    painterPixmap.drawPixmap(0, 0, pixmap);
    painterPixmap.end();
}

void mainTaskWidget::slot_showOrHidePicInfoWidget()
{
    switch (sOhFlag)
    {
    case Hide:
        m_PicInfoWidget->show();
        this->m_PicInfoWidget->adjustSize();
        sOhFlag=Show;
        emit sig_picInfoOpen();
        break;
    case Show:
        m_PicInfoWidget->hide();
        sOhFlag=Hide;
        emit sig_picInfoClose();
        break;
    default:
        break;
    }
//判断此时是否为全屏 还需要重新设置bottomwidget位置 和大小
    if(parentWidget()->parentWidget()->isFullScreen())
    {}
}



mainTaskWidget::~mainTaskWidget()
{
}

QSize mainTaskWidget::getImageSize(const QString &filename)
{
    QImageReader imgReader(filename);
    return imgReader.size();
}


void mainTaskWidget::slot_updataThreadNum()
{
    if(!m_isLoaded && m_imagesNum > 0)
    {
       startLoadOriImgThread();
    }
}

//开启线程加载原图
void mainTaskWidget::startLoadOriImgThread()
{
    m_isLoaded = false;

    for(int i=0; i<numberOfThreads; i++)
    {
        if(!m_loadOriImgThreadArray->at(i)->isRunning())
        {
            m_loadOriImgThreadArray->at(i)->currentFile = m_currentfile;
            m_isLoaded = true;
            m_loadOriImgThreadArray->at(i)->start();
            break;
        }
    }
}

void mainTaskWidget::_startLoadThumbnaiThread()
{
}


void mainTaskWidget::getAllImagesPath()
{    
    if(m_bottomWidget->m_thumbnailView->ThumbThreadpre->isRunning())
        m_bottomWidget->m_thumbnailView->ThumbThreadpre->breakNum = 1;
    if(m_bottomWidget->m_thumbnailView->ThumbThreadnext->isRunning())
        m_bottomWidget->m_thumbnailView->ThumbThreadnext->breakNum = 1;

    //如果两个线程还在跑等待结束
    while (m_bottomWidget->m_thumbnailView->ThumbThreadnext->isRunning() ||
           m_bottomWidget->m_thumbnailView->ThumbThreadpre->isRunning()  ||
           this->filesInformationThread->isRunning()) {}

    //设置当前线程ID
    if(m_bottomWidget->m_thumbnailView->currentThreadId == 10000)
        m_bottomWidget->m_thumbnailView->currentThreadId = 0;
    m_bottomWidget->m_thumbnailView->currentThreadId++;


    this->filesInformationThread->orignalFile = m_orignalFile;
    this->filesInformationThread->start();
}

void mainTaskWidget::resizeEvent(QResizeEvent *e)
{

    mainTaskWidgetWidth = this->width();
    mainTaskWidgetHeight = this->height();

   int availableHeigth = QApplication::desktop()->availableGeometry().bottomLeft().y();
   //非全屏时
    if(!parentWidget()->parentWidget()->parentWidget()->isFullScreen())
    {
        if(m_bottomWidget->m_thumbnailView->getShowOrHide() == Hide)
        {
            m_bottomWidget->setGeometry(0, mainTaskWidgetHeight-TOOLBARHEIGTH,
                                        mainTaskWidgetWidth, TOOLBARHEIGTH);
        }
        else
        {
            m_bottomWidget->setGeometry(0, mainTaskWidgetHeight-(TOOLBARHEIGTH + THUMBNAILHEIGTH),
                                        mainTaskWidgetWidth, (TOOLBARHEIGTH + THUMBNAILHEIGTH));
        }

//        m_regions[LeftPageRt] = QRect(0, 0, (TOOLBARHEIGTH + THUMBNAILHEIGTH), mainTaskWidgetHeight-(TOOLBARHEIGTH + THUMBNAILHEIGTH));
//        m_regions[RightPageRt] = QRect(mainTaskWidgetWidth-(TOOLBARHEIGTH + THUMBNAILHEIGTH), 0,
//                                       (TOOLBARHEIGTH + THUMBNAILHEIGTH), mainTaskWidgetHeight-(TOOLBARHEIGTH + THUMBNAILHEIGTH));
//        m_regions[BottomCtlRt] = QRect(0, mainTaskWidgetHeight-(TOOLBARHEIGTH + THUMBNAILHEIGTH),
//                                       mainTaskWidgetWidth, (TOOLBARHEIGTH + THUMBNAILHEIGTH));
//        m_regions[InnerRt] = QRect((TOOLBARHEIGTH + THUMBNAILHEIGTH),0,mainTaskWidgetWidth-160,
//                                   mainTaskWidgetHeight-(TOOLBARHEIGTH + THUMBNAILHEIGTH));

        m_regions[LeftPageRt] = QRect(150, 0, (TOOLBARHEIGTH + THUMBNAILHEIGTH)+50, mainTaskWidgetHeight-(TOOLBARHEIGTH + THUMBNAILHEIGTH));
        m_regions[RightPageRt] = QRect(mainTaskWidgetWidth-(TOOLBARHEIGTH + THUMBNAILHEIGTH), 0,
                                       (TOOLBARHEIGTH + THUMBNAILHEIGTH), mainTaskWidgetHeight-(TOOLBARHEIGTH + THUMBNAILHEIGTH));
        m_regions[BottomCtlRt] = QRect(0, mainTaskWidgetHeight-(TOOLBARHEIGTH + THUMBNAILHEIGTH),
                                       mainTaskWidgetWidth, (TOOLBARHEIGTH + THUMBNAILHEIGTH));
        m_regions[InnerRt] = QRect(150+(TOOLBARHEIGTH + THUMBNAILHEIGTH)+50,0,mainTaskWidgetWidth-400,
                                   mainTaskWidgetHeight-(TOOLBARHEIGTH + THUMBNAILHEIGTH));

    }
    else//全屏时
    {
        //没打开缩略图  虽然工具栏已经切换了 但高度依然是TOOLBARHEIGTH
        //如果已经全屏了 点击打开图片信息栏 此时高度为SMALLTOOLBARHEIGTH
        if(m_bottomWidget->height() == TOOLBARHEIGTH || m_bottomWidget->height() == SMALLTOOLBARHEIGTH)
        {
            m_bottomWidget->setGeometry(0, mainTaskWidgetHeight - DISTANCE - SMALLTOOLBARHEIGTH,
                                        mainTaskWidgetWidth, SMALLTOOLBARHEIGTH);

        }
        else//打开缩略图
        {
            m_bottomWidget->setGeometry(0, mainTaskWidgetHeight - DISTANCE - (SMALLTOOLBARHEIGTH + THUMBNAILHEIGTH),
                                    mainTaskWidgetWidth, SMALLTOOLBARHEIGTH + THUMBNAILHEIGTH);
        }

//        m_regions[LeftPageRt] = QRect(0, 0, (SMALLTOOLBARHEIGTH + THUMBNAILHEIGTH),
//                                      mainTaskWidgetHeight - DISTANCE  -(SMALLTOOLBARHEIGTH + THUMBNAILHEIGTH));
//        m_regions[RightPageRt] = QRect(mainTaskWidgetWidth-(SMALLTOOLBARHEIGTH + THUMBNAILHEIGTH), 0,
//                                       (SMALLTOOLBARHEIGTH + THUMBNAILHEIGTH), availableHeigth-(SMALLTOOLBARHEIGTH + THUMBNAILHEIGTH));
//        m_regions[BottomCtlRt] = QRect(0, mainTaskWidgetHeight - DISTANCE -(SMALLTOOLBARHEIGTH + THUMBNAILHEIGTH), mainTaskWidgetWidth,
//                                       DISTANCE +(SMALLTOOLBARHEIGTH + THUMBNAILHEIGTH));
//        m_regions[InnerRt] = QRect((SMALLTOOLBARHEIGTH + THUMBNAILHEIGTH),0,mainTaskWidgetWidth-160,
//                                   mainTaskWidgetHeight - DISTANCE -(SMALLTOOLBARHEIGTH + THUMBNAILHEIGTH));
        m_regions[LeftPageRt] = QRect(150, 0, (SMALLTOOLBARHEIGTH + THUMBNAILHEIGTH)+50,
                                      mainTaskWidgetHeight - DISTANCE  -(SMALLTOOLBARHEIGTH + THUMBNAILHEIGTH));
        m_regions[RightPageRt] = QRect(mainTaskWidgetWidth-(SMALLTOOLBARHEIGTH + THUMBNAILHEIGTH), 0,
                                       (SMALLTOOLBARHEIGTH + THUMBNAILHEIGTH), availableHeigth-(SMALLTOOLBARHEIGTH + THUMBNAILHEIGTH));
        m_regions[BottomCtlRt] = QRect(0, mainTaskWidgetHeight - DISTANCE -(SMALLTOOLBARHEIGTH + THUMBNAILHEIGTH), mainTaskWidgetWidth,
                                       DISTANCE +(SMALLTOOLBARHEIGTH + THUMBNAILHEIGTH));
        m_regions[InnerRt] = QRect(150+(SMALLTOOLBARHEIGTH + THUMBNAILHEIGTH)+50,0,mainTaskWidgetWidth-400,
                                   mainTaskWidgetHeight - DISTANCE -(SMALLTOOLBARHEIGTH + THUMBNAILHEIGTH));
    }

    //如果显示缩略图才设置item位置 点击显示缩略图首先设置item位置
    if(!m_bottomWidget->m_thumbnailView->isHidden())
        slot_setAllItemPos();

    m_leftPage->setGeometry(0, (mainTaskWidgetHeight-54)/2, 80, 54);
    m_rightPage->setGeometry(mainTaskWidgetWidth-80, (mainTaskWidgetHeight-54)/2, 80, 54);
    picInfoWidgetFlag = false;
    setSelectBoxPos();
}


void mainTaskWidget::slot_picInfoOpen()
{
    m_leftPage->setGeometry(200, (mainTaskWidgetHeight-54)/2, 80, 54);
    m_rightPage->setGeometry(mainTaskWidgetWidth-80, (mainTaskWidgetHeight-54)/2, 80, 54);
    picInfoWidgetFlag = true;
}

void mainTaskWidget::slot_picInfoClose()
{
    m_leftPage->setGeometry(0, (mainTaskWidgetHeight-54)/2, 80, 54);
    m_rightPage->setGeometry(mainTaskWidgetWidth-80, (mainTaskWidgetHeight-54)/2, 80, 54);
    picInfoWidgetFlag = false;
}

void mainTaskWidget::leaveEvent(QEvent *e)
{
    if(!this->sideAreaState)
    {
        sig_hideLeftAndRight();
    }
    if(!this->bottomAreaState)
    {
        sig_hideToolBar();
    }
}

//鼠标在 view 上移动将坐标 过来判断在哪个区域 选择发什么信号
void mainTaskWidget::slot_determineCursorPos(const QPoint &posOfCursor)
{

    if(m_regions[InnerRt].contains(posOfCursor) /*|| m_regions[BottomCtlRt].contains(posOfCursor)*/)
    {
        if(!this->sideAreaState)
        {
            sig_hideLeftAndRight();
        }
        if(!this->bottomAreaState)
        {
            sig_hideToolBar();
        }
        return;
    }
    else if(m_regions[BottomCtlRt].contains(posOfCursor))
    {
        if(!this->sideAreaState)
        {
            sig_hideLeftAndRight();
        }
        sig_showBottomCtl();
        return;
    }
    else if(m_regions[RightPageRt].contains(posOfCursor))
    {
        if(!this->bottomAreaState)
        {
            sig_hideToolBar();
        }
        sig_showDirectionKey();
        return;
    }
    else if(m_regions[LeftPageRt].contains(posOfCursor))
    {
        if(!this->bottomAreaState)
        {
            sig_hideToolBar();
        }
        sig_showDirectionKey();
        return;
    }
}


//mainwidget_1 发来的图片名称
void mainTaskWidget::slot_receivePicFile(const QString &filename)
{
    m_orignalFile = m_currentfile=filename;

    QPixmap img;
    m_view->setPixmapOnView(m_currentfile, /*QPixmap::fromImage(img)*/img, getImageSize(m_currentfile));

    /*QPixmap::fromImage(m_bottomWidget->m_thumbnailView->allThumbImg.at(m_currentNum))*/
    startLoadOriImgThread();   //开启线程加载原图片

    getAllImagesPath();        //得到所有的图片文件信息
}

void mainTaskWidget::slot_fileInfoThreadFinshed()
{           
    this->m_bottomWidget->m_thumbnailView->allImgs = this->m_allImagesInfo = this->filesInformationThread->allImagesInfo;
    this->m_bottomWidget->m_thumbnailView->allImgsName = this->m_allImagesName = this->filesInformationThread->allImagesName;
    this->m_bottomWidget->m_thumbnailView->imgNum = this->m_imagesNum = this->filesInformationThread->imagesNum;

    this->m_bottomWidget->m_thumbnailView->widwidth = this->m_bottomWidget->width();
    this->m_bottomWidget->m_thumbnailView->widheight = this->m_bottomWidget->height();
    this->m_bottomWidget->m_thumbnailView->selectedNum = this->m_currentNum = this->filesInformationThread->currentNum;
    this->m_bottomWidget->m_thumbnailView->oldSelectNum = this->m_currentNum;
    this->m_bottomWidget->m_thumbnailView->setItem();

//enable the pre&next button to protect software crash

    isFileInfoThreadFinshed = true;
//    this->m_rightPage->m_direction->setDisabled(false);
//    this->m_leftPage->m_direction->setDisabled(false);
//    this->m_bottomWidget->m_toolbar->m_left->setDisabled(false);
//    this->m_bottomWidget->m_toolbar->m_right->setDisabled(false);


//    this->filesInformationThread->exit();
}

//打开幻灯片对话框并设置初始图片
void mainTaskWidget::slot_showSlide()
{
    if(!isFileInfoThreadFinshed)
    {
        this->m_view->setTipText(tr("图片读取中..."));
    }
    else
    {
        slide->m_loadSlideThread->m_allImagesInfo = m_allImagesInfo;
        slide->m_loadSlideThread->m_StartingPoint = m_currentNum;  //设置起点
        slide->m_loadSlideThread->breakFlag = true;

        slide->m_currentNum = m_currentNum;
        slide->isRunning = true; //将播放状态设为 true
        slide->m_total = m_allImagesInfo.length();//
        slide->m_currentPix = m_view->m_item->getPixmap();  //得到当前显示的原图
        slide->setCurrentPixmap(m_currentfile);
        slide->showFullScreen();
    }
}

void mainTaskWidget::slot_opensetUpDialog()
{
    t_setUpDialog= new setUpDialog(this);
   // this->t_setUpDialog->m_normalWidget->m_2_cb->setChecked(this->bottomAreaState);
    this->t_setUpDialog->m_normalWidget->m_3_cb->setChecked(this->sideAreaState);
    this->t_setUpDialog->m_normalWidget->m_4_cb->setChecked(this->normalSet4State);

//    this->t_setUpDialog->supWgt->currentFile = this->m_currentfile;
    connect(t_setUpDialog->m_normalWidget->m_7_cb,SIGNAL(clicked()),this,SLOT(slot_radioBtnCheck()));
    connect(t_setUpDialog->m_normalWidget->m_8_cb,SIGNAL(clicked()),this,SLOT(slot_radioBtnCheck()));

    connect(this->t_setUpDialog->m_normalWidget,SIGNAL(sig_normalSetting1(bool)),this,SIGNAL(sig_normalSetting1(bool)));
    connect(this->t_setUpDialog->m_normalWidget,SIGNAL(sig_normalSetting3(bool)),this,SLOT(slot_normalSetting3(bool)));
    connect(this->t_setUpDialog->m_normalWidget,SIGNAL(sig_normalSetting2(bool)),this,SLOT(slot_normalSetting2(bool)));
   connect(this->t_setUpDialog->m_normalWidget,SIGNAL(sig_normalSetting4(bool)),this,SLOT(slot_normalSetting4(bool)));
   connect(this->t_setUpDialog->m_normalWidget,SIGNAL(sig_normalSetting5(bool)),this,SLOT(slot_normalSetting5(bool)));

    connect(this->t_setUpDialog,SIGNAL(sig_normalSetting1(bool)),this,SIGNAL(sig_normalSetting1(bool)));
    connect(this->t_setUpDialog,SIGNAL(sig_normalSetting3(bool)),this,SLOT(slot_normalSetting3(bool)));
    connect(this->t_setUpDialog,SIGNAL(sig_normalSetting2(bool)),this,SLOT(slot_normalSetting2(bool)));
    connect(this->t_setUpDialog,SIGNAL(sig_normalSetting4(bool)),this,SLOT(slot_normalSetting4(bool)));
    connect(this->t_setUpDialog,SIGNAL(sig_normalSetting5(bool)),this,SLOT(slot_normalSetting5(bool)));


    connect(this->t_setUpDialog,SIGNAL(sig_sendSlideSetInfo(QString)),this,SLOT(slot_recvSlideSetInfo(QString)));
    t_setUpDialog->show();
}


void mainTaskWidget::slot_recvSlideSetInfo(QString setInfo)
{
//    qDebug()<<"setInfo:"<<setInfo;
    if(setInfo.length() == 4)
    {
//set play interval
        this->slide->m_currentInterval = setInfo.at(1).toLatin1() - '0';
//set play effect
        this->slide->m_currentPlayEffectIndex = setInfo.at(2).toLatin1() - '0';
//set slide background
        if(setInfo.at(3) == QChar('0'))
        {
            this->slide->backgroundColor.setColor(QPalette::Window, QColor(0, 0, 0));
        }
        else
        {
            this->slide->backgroundColor.setBrush(QPalette::Window, QBrush(QPixmap(":/prefix3/3-images/wave.png")));
        }
        this->slide->setPalette(this->slide->backgroundColor);
    }
}

void mainTaskWidget::slot_normalSetting4(bool ischecked)
{
    this->normalSet4State = ischecked;
}
void mainTaskWidget::slot_normalSetting5(bool ischecked)
{
   qDebug()<<"slot_normalSetting5";
    if(ischecked)
    {
        if(!this->isFullScreen())
        {
            m_bottomWidget->setGeometry(0, parentWidget()->height() - (TOOLBARHEIGTH + THUMBNAILHEIGTH) - 0,
                     parentWidget()->width(), (TOOLBARHEIGTH+ THUMBNAILHEIGTH));
        }
        else
        {
            m_bottomWidget->setGeometry(0, parentWidget()->height() - (SMALLTOOLBARHEIGTH + THUMBNAILHEIGTH) - DISTANCE,
                     parentWidget()->width(), (SMALLTOOLBARHEIGTH+ THUMBNAILHEIGTH));
        }
       m_bottomWidget->m_thumbnailView->show();
       m_bottomWidget->m_thumbnailView->setShowOrHide(Show);
    }
    else
    {
        m_bottomWidget->m_thumbnailView->hide();
        m_bottomWidget->m_thumbnailView->setShowOrHide(Hide);
    }


}

void mainTaskWidget::slot_normalSetting2(bool ischecked)
{
    bottomAreaState = ischecked;
    if(ischecked)
    {
        sig_showBottomCtl();
    }
    else
    {
        sig_hideToolBar();
    }
}
void mainTaskWidget::slot_normalSetting3(bool ischecked)
{
    sideAreaState = ischecked;
    if(ischecked)
    {
        sig_showDirectionKey();
    }
    else
    {
        sig_hideLeftAndRight();
    }
}

void mainTaskWidget::slot_radioBtnCheck()
{

    switch(t_setUpDialog->m_normalWidget->mCbBtnGroup->checkedId())
    {
        case 0:
              m_view->setBackGround(0);
            break;
        case 1:
            m_view->setBackGround(1);
            break;
    }
}


void mainTaskWidget::slot_slideQuit(int index)
{
    slot_receiveFromView(index);

    //设置缩略图显示序号 和 移动缩略图
    m_bottomWidget->m_thumbnailView->selectedNum = index;
    //移动缩略图
    m_bottomWidget->m_thumbnailView->changeImg(200);
}

void mainTaskWidget::slot_finishSave()
{
//    m_view->setTipText(QString("已保存"));

    foreach (saveAsThread *t_thread, m_saveAsThreadlist)
    {
//        if(t_thread->isFinished())
        if(t_thread->stateFlag)
        {
            m_view->setTipText(QString(tr("已保存")));
            delete t_thread;
            m_saveAsThreadlist.removeAt(m_saveAsThreadlist.indexOf(t_thread));
            break;
        }
        else
            m_view->setSaveTipText(QString(tr("文件名输入不正确,请重新输入")));
    }
}

void mainTaskWidget::slot_sharePicture()
{
        QMessageBox::information(this,tr("提示"),tr("此功能正在开发中..."));
}
void mainTaskWidget::slot_setAllItemPos()
{
    m_bottomWidget->m_thumbnailView->widwidth = this->width();
    m_bottomWidget->m_thumbnailView->widheight = (TOOLBARHEIGTH + THUMBNAILHEIGTH);

    if(m_bottomWidget->m_thumbnailView->allPixItem.length() == m_imagesNum)
        for(int i=0; i<m_imagesNum; i++)
            m_bottomWidget->m_thumbnailView->setItemPos(i);
}

//打开美图工具
void mainTaskWidget::slot_openSelectbox()
{
    m_box = new SelectBox(tr("编辑工具  "), this);
    m_box->addBoxContent(QPixmap(":/prefix1/1-images/gimp.png"), "gimp");
#ifdef NFS-PICTURE-EDITOR
    m_box->addBoxContent(QPixmap(":/prefix1/1-images/gimp.png"),tr("图片编辑"));
#endif

    m_box->setFixedWidth(m_box->m_boxWidth);         //固定选项框的宽
    connect(m_box, SIGNAL(sig_ret(int)), this, SLOT(slot_OpenPictureEditingTool(int)));

    setSelectBoxPos();

    m_box->show();
//    mBoxIsOpen=true;
    m_box->m_closeBtn->hide();
}
void mainTaskWidget::loadWeiboShare()
{
//        QRect rect(0,0,1440,900);
//        shareWeb=new QWebView(this);
//        shareWeb->setGeometry(rect);
//        QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled,true);
//        shareWeb->settings()->setAttribute(QWebSettings::PluginsEnabled,true);
//        shareWeb->load(QUrl("http://www.jiathis.com/send/?webid=tsina&url=http://www.urlurl.com&title=自定义内容"));
//        shareWeb->show();


//    QDesktopServices::openUrl(QUrl(QLatin1String("http://www.jiathis.com/send/?webid=tsina&url=http://www.urlurl.com&title=自定义内容")));


#if 1

  /*added bi huj 20160512*/
//    const QUrl url("http://www.jiathis.com/send/?webid=tsina&url=http://www.nfs-china.com/index.html&title=来自方德的图片分享！&pic=http://o71ylzgrr.bkt.clouddn.com/5df72237c81f217863e09cf1a284c528.gif");
    const QUrl url("http://www.jiathis.com/send/?webid=tsina&url=http://www.nfs-china.com/index.html&title=来自方德的图片分享！&pic=http://nfs-picture-browser.oss-cn-beijing.aliyuncs.com/test01.gif");
//    const QUrl url("http://www.baidu.com");
    QDesktopServices::openUrl(url);
/*end added*/
#endif

#if 0
     const char *access_id = "bIS0GdtnOGTCPfyK";   //设置用户帐号
     const char *access_key = "YdNhu4VNRhRj2oPGZ06hI2JoS3M5Lo";  //设置用户密码
     const char *endpoint = "oss-cn-shanghai.aliyuncs.com";    //设置hostname
     const char*bucket_name="nfs-picture-editor";
    oss_client_t*client=client_initialize_with_endpoint(access_id,access_key,endpoint);
   const char*filename="124981977249437952.jpg";

   time_t t=time(NULL);
   t+=60;
   char  expiration[20];
    sprintf(expiration, "%ld", t);
     printf("expiration = %s\n", expiration);
     char*urlStr=client_generate_presigned_url_with_method(client,bucket_name,filename,expiration,"GET");
//     qDebug<<urlStr;
     if(urlStr != NULL) {
             free(urlStr);
             urlStr = NULL;
         }
#endif

#if 0
     if (aos_http_io_initialize(NULL, 0) != AOSE_OK) {
          exit(1);
      }

     aos_pool_t *p = NULL;
     aos_string_t bucket;
     aos_string_t object;
     int is_cname = 0;
     aos_table_t *headers = NULL;
     aos_table_t *resp_headers = NULL;
     oss_request_options_t *options = NULL;
     char *filename = "/home/jackey/test01.gif";
     aos_status_t *s = NULL;
     aos_string_t file;

     aos_pool_create(&p, NULL);
     options = oss_request_options_create(p);
     init_sample_request_options(options, is_cname);
     headers = aos_table_make(options->pool, 1);
     apr_table_set(headers, OSS_CONTENT_TYPE, "image/jpeg");
     aos_str_set(&bucket, BUCKET_NAME);
     aos_str_set(&object, OBJECT_NAME);
     aos_str_set(&file, filename);

     s = oss_put_object_from_file(options, &bucket, &object, &file,
                                  headers, &resp_headers);

     if (aos_status_is_ok(s)) {
         printf("put object from file succeeded\n");
     } else {
     printf("put object from file failed\n");
     }

     aos_pool_destroy(p);


      aos_http_io_deinitialize();
#endif

}

void mainTaskWidget::loadGimpTool()
{
    QFile file("/usr/bin/gimp");
    if(file.exists())
    {
        m_beautifyProc->start("gimp " + m_currentfile);
    }
    else
    {
        QMessageBox::critical(this,tr("警告             "),tr("请先安装GIMP图片编辑工具","确定"));
    }
}
void mainTaskWidget::slot_OpenPictureEditingTool(int id)
{
    m_box = NULL;
    switch (id) {
    case 0:
        loadGimpTool();
        break;
    case 1:
        // m_beautifyProc->start("pictureEditor " + m_currentfile);
        printf("hello picture browser!");        
        loadWeiboShare();
        break;
    default:
        break;
    }
}

void mainTaskWidget::slot_receiveFromView(const int &num)
{
    m_currentNum = num;

//    m_orignalFile = m_allImagesInfo.at(m_currentNum).absoluteFilePath();

//    imageReader->readImage(m_orignalFile);
//    m_currentfile = imageReader->fileChanged;


    m_currentfile = m_allImagesInfo.at(m_currentNum).absoluteFilePath();

    if(m_saveAsThreadlist.length()>0)
        m_view->m_stackedlayout->setCurrentIndex(0);

    QPixmap img;
    m_view->setPixmapOnView(m_currentfile, img/*QPixmap::fromImage(m_bottomWidget->m_thumbnailView->midPixItems.at(m_currentNum)->m_thumbnail)*/,
                                   getImageSize(m_currentfile));
    //开启线程加载原图片
    startLoadOriImgThread();
}

void mainTaskWidget::slot_reciveImgInfo(const QString &dpix, const QString &dpiy)
{
    if(m_imagesNum < 1)
         return ;

    QStringList info;
    imageReader->readImage(m_currentfile);
    QSize t_size = getImageSize(imageReader->fileChanged);

    info.append(QString::number(t_size.width()));
    info.append(QString::number(t_size.height()));
    info.append(dpix);
    info.append(dpiy);

    sig_sendToInfoWidget(m_allImagesInfo.at(m_currentNum), info);
}

void mainTaskWidget::slot_prev()
{
    if(!isFileInfoThreadFinshed)
    {
        this->m_view->setTipText(tr("图片读取中..."));
    }
    else
    {
        if(m_currentNum > 0 )
        {
            m_currentNum -= 1;
        }
        else
        {
            m_currentNum = m_imagesNum-1;
        }

        changePicure();
    }
}

void mainTaskWidget::slot_next()
{
    if(!isFileInfoThreadFinshed)
    {
        this->m_view->setTipText(tr("图片读取中..."));
    }
    else
    {
        if(m_currentNum < m_imagesNum-1)
        {
            m_currentNum += 1;
        }
        else
        {
            m_currentNum = 0;
        }
        changePicure();
    }
}

void mainTaskWidget::slot_openFile()
{
    QString dirPath = m_allImagesInfo.at(m_currentNum).absoluteDir().absolutePath();
    QString getFileName = openFileDialog(this, dirPath);

    if (!getFileName.isEmpty())
    {
        m_orignalFile=getFileName;
        m_currentfile = getFileName;

        //空图片
        QPixmap t_img;

        if(m_saveAsThreadlist.length()>0)
            m_view->m_stackedlayout->setCurrentIndex(0);
        m_view->setPixmapOnView(m_currentfile, t_img, getImageSize(m_currentfile));

        //开启线程加载原图片
        startLoadOriImgThread();
//obtain all files information;
        getAllImagesPath();

//hide the thumbnail
//        if(this->m_bottomWidget->m_thumbnailView->getShowOrHide() == Show)
  //      {
     //       this->m_bottomWidget->slot_smallPictureViewShowOrHide();
       // }
    }
}


void mainTaskWidget::removeImg()
{
    QString filename = m_currentfile;
    QString t_filename = m_allImagesInfo.at(m_currentNum).fileName();
    QString fileNewName = QDir::homePath() + "/.local/share/Trash/files/" + t_filename;
    QFile::copy(filename, fileNewName);
    QFile::remove(filename);

    //写.trashinfo文件，在回收站还原时使用
    QString trashFilePath = QDir::homePath() + "/.local/share/Trash/info/" + t_filename + ".trashinfo";
    QFile trashFile(trashFilePath);
    trashFile.open(QFile::WriteOnly);
    QTextStream trashOutput(&trashFile);
    trashOutput << "[Trash Info]" << endl;
    trashOutput << "Path=" + filename <<endl;
    trashFile.close();

    m_allImagesName.removeAt(m_currentNum);
    m_allImagesInfo.removeAt(m_currentNum);

    if(m_currentNum == m_imagesNum-1)
    {
        m_currentNum = 0;
    }
    else
    {
        m_imagesNum -= 1;
    }
    //当删一张图片时 m_currentNum = 0; m_imagesNum = 0;

    sig_removeImg();  //发送信号 删除相应的缩略图

    m_view->setTipText(QString(tr("已删除")));

    m_imagesNum = m_imagesNum -1 ;

    if(!m_imagesNum) //如果图片为 0 张
    {
        m_currentNum = -1;
        m_currentfile = "";
        sig_changePage();
    }
    else
    {
        m_currentfile = m_allImagesInfo.at(m_currentNum).absoluteFilePath();

        QPixmap img;
        m_view->setPixmapOnView(m_currentfile, img/*QPixmap::fromImage(m_bottomWidget->m_thumbnailView->midPixItems.at(m_currentNum)->m_thumbnail)*/,
                                getImageSize(m_currentfile));
        //开启线程加载原图片
        startLoadOriImgThread();
    }
}

void mainTaskWidget::slot_removeImg()
{
    if(!isFileInfoThreadFinshed)
    {
        this->m_view->setTipText(tr("图片读取中..."));
        return;
    }

    if(!m_allImagesInfo.length())
    {
        return;
    }

    Checked ret;

    if(normalSet4State)
    {
        QString t_filename = m_allImagesInfo.at(m_currentNum).fileName();
        QString text = tr("确认要删除文件 “") + t_filename + tr("” 吗?");
        myMessageBox t_msgBox(QString(tr("取消")), QString(tr("确定")),
                              QString(tr("覆盖图片")), false,
                              QPixmap(":/prefix1/1-images/question.png"),
                              text, this);
        t_msgBox.exec();

        ret = t_msgBox.getChecked();
    }
    else
    {
        ret = Ok;
    }

    switch (ret)
    {
      case Ok:
          removeImg();
          break;
      case Cancel:
          break;
      default:
          break;
    }
}

void mainTaskWidget::slot_copyImg()
{
     QClipboard *clipboard = QApplication::clipboard();
     QByteArray localFile;
     localFile.append(QString("copy\nfile://")+ m_allImagesInfo.at(m_currentNum).absoluteFilePath());
     QMimeData *src=new QMimeData;
     src->setData("x-special/gnome-copied-files", localFile);
     clipboard->setMimeData(src);
}

void mainTaskWidget::slot_saveAs()
{
    QString slcStr;
    QString t_fileName;
    QString t_string = m_currentfile;

    int index = m_currentfile.lastIndexOf('.');
    if(index == -1)
    {
        t_string = t_string + "-看图工具";
    }
    else
    {
        t_string.insert(index, "-看图工具");
    }
    t_fileName = QFileDialog::getSaveFileName(this, tr("保存图片"), t_string);
                                          //"Images (*.bmp *.gif *.tif *.jpg *.jpeg *.jpeg2000 *.png *.pbm *.pgm *.ppm *.xbm *.xpm *.tiff)", &slcStr);

    QString userName = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    userName = userName.section("/", -1, -1);
    QFileInfo fileInfo = QFileInfo(m_currentfile);
    QString fileOwner = fileInfo.owner();
    if(userName == fileOwner)   // the current path == current user path
    {
        if (!t_fileName.isNull())
        {
            //处理文件名
            t_fileName = fixFilename(t_fileName);
            m_view->m_stackedlayout->setCurrentIndex(1);
            m_view->m_tipLb->setText(QString(tr("正在保存...")));
            saveAsThread *t_saveAsThread;
            if(m_view->isRotated())
            {
                Direction t_direction = m_view->getDirection();
                QPixmap t_rotatedPix = getRotatedOrgPix(t_direction);
                t_saveAsThread = new saveAsThread(t_fileName, t_rotatedPix);
            }
        else
        {
            //开启线程保存图片
            t_saveAsThread = new saveAsThread(t_fileName, m_view->m_item->getPixmap());
        }
            m_saveAsThreadlist.append(t_saveAsThread);
            t_saveAsThread->start();
            connect(m_saveAsThreadlist.at(m_saveAsThreadlist.length()-1),
                    SIGNAL(finished()), this, SLOT(slot_finishSave()));
        }
        else
        {
            std::cout << "Failed to get file name" << std::endl;
        }
    }
    else
        emit sig_saveAsDialog();
}

void mainTaskWidget::slot_saveAsDialog()
{
    m_view->setSaveTipText(QString(tr("您没有权限对当前文件进行更改")));
}

void mainTaskWidget::slot_refresh()
{    
    slot_receivePicFile(m_currentfile);
}


void mainTaskWidget::saveAsRotatedImg(const QPixmap &pix)
{
    QString slcStr;
    QString t_fileName;
    QString t_string = m_currentfile;

    int index = m_currentfile.lastIndexOf('.');

    if(index == -1)
    {
        t_string = t_string + "-看图工具";
    }
    else
    {
        t_string.insert(index, "-看图工具");
    }

    t_fileName = QFileDialog::getSaveFileName(this, tr("保存图片"), t_string);
                                          //"Images (*.bmp *.gif *.tif *.jpg *.jpeg *.jpeg2000 *.png *.pbm *.pgm *.ppm *.xbm *.xpm *.tiff)", &slcStr);

    if (!t_fileName.isNull())
    {
        //处理文件名
        t_fileName = fixFilename(t_fileName);
        //开启线程保存图片
        m_view->m_stackedlayout->setCurrentIndex(1);
        m_view->m_tipLb->setText(QString(tr("正在保存...")));
        saveAsThread *t_saveAsThread = new saveAsThread(t_fileName, pix);
        m_saveAsThreadlist.append(t_saveAsThread);
        t_saveAsThread->start();
        connect(m_saveAsThreadlist.at(m_saveAsThreadlist.length()-1),
                SIGNAL(finished()), this, SLOT(slot_finishSave()));
    }
    else
    {
        std::cout << "Failed to get file name" << std::endl;
    }
}

void mainTaskWidget::changePicure()
{
    //设置缩略图显示序号 和 移动缩略图
    m_bottomWidget->m_thumbnailView->selectedNum = m_currentNum;
    //移动缩略图
    m_bottomWidget->m_thumbnailView->changeImg(150);

    slot_receiveFromView(m_currentNum);
}

QPixmap mainTaskWidget::getRotatedOrgPix(Direction direction)
{
    QPixmap t_pix = m_view->m_item->getPixmap();
    QMatrix t_matrix;
    switch (direction) {
    case Right:
        t_matrix.rotate(90);
        break;
    case Down:
        t_matrix.rotate(180);
        break;
    case Left:
        t_matrix.rotate(270);
        break;
    default:
        break;
    }

    QPixmap t_rotatedPix = t_pix.transformed(t_matrix);

    return  t_rotatedPix;
}

void mainTaskWidget::setSelectBoxPos()
{

    if(!m_box)
        return;

    if(parentWidget()->parentWidget()->parentWidget()->isFullScreen())
    {
        QPoint referenceP = m_bottomWidget->m_smallToolBar->mapTo(this, m_bottomWidget->m_smallToolBar->m_beautify->pos());
        m_box->move(QPoint(referenceP.x() - (m_box->m_boxWidth - m_bottomWidget->m_smallToolBar->m_beautify->width())/2,
                 mainTaskWidgetHeight - m_bottomWidget->height() - m_box->height() -DISTANCE));
    }
    else
    {

        QPoint referenceP = m_bottomWidget->m_toolbar->mapTo(this, m_bottomWidget->m_toolbar->m_beautify->pos());
        m_box->move(QPoint(referenceP.x() - (m_box->m_boxWidth - m_bottomWidget->m_toolbar->m_beautify->width())/2,
                 mainTaskWidgetHeight - m_bottomWidget->height() - m_box->height()));
    }
}

void mainTaskWidget::savePicture(Direction direction)
{
    QPixmap t_rotatedPix;
    saveAsThread *t_saveAsThread;

    myMessageBox t_msgBox(QString(tr("不保存")), QString(tr("另存为")),
                          QString(tr("覆盖原图")), true,
                          QPixmap(":/prefix1/1-images/question.png"),
                          QString(tr("图片发生旋转，请选择保存方式?")), this);

    t_msgBox.exec();

    Checked t_checked = t_msgBox.getChecked();

    switch (t_checked) {
    case Ok:
        slot_saveAs();
        break;
    case Cancel:
        break;
    case Cover:
        t_rotatedPix = getRotatedOrgPix(direction);
        m_view->m_stackedlayout->setCurrentIndex(1);
        m_view->m_tipLb->setText(QString(tr("正在保存...")));
        t_saveAsThread = new saveAsThread(m_currentfile, t_rotatedPix);
        m_saveAsThreadlist.append(t_saveAsThread);
        t_saveAsThread->start();
        connect(m_saveAsThreadlist.at(m_saveAsThreadlist.length()-1),
                SIGNAL(finished()), this, SLOT(slot_finishSave()));
        break;
    default:
        break;
    }
}

QString &mainTaskWidget::fixFilename(QString &filename)
{
    int index = filename.lastIndexOf(".");

    if(index == -1)
    {
        filename = filename + ".png";
    }
    else
    {
        QStringList format;
        format << "bmp"
               << "gif"
               << "tif"
               << "jpg"
               << "jpeg"
               << "jpeg2000"
               << "png"
               << "pbm"
               << "pgm"
               << "ppm"
               << "xbm"
               << "xpm"
               << "tiff";

        if(!format.contains(filename.right(filename.length() - index - 1), Qt::CaseSensitive))
        {
            filename = filename + ".png";
        }
    }

    return filename;
}


void mainTaskWidget::slot_save()
{
    QString userName = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    userName = userName.section("/", -1, -1);
    QFileInfo fileInfo = QFileInfo(m_currentfile);
    QString fileOwner = fileInfo.owner();

    QPixmap t_rotatedPix;
    saveAsThread *t_saveAsThread;

    if(fileOwner == userName)
    {
        Direction t_direction = m_view->getDirection();
//        switch (t_direction)
//        {
//            case Up:
//                m_view->setTipText(QString("图片无修改"));
//                std::cout << "The picture didn't rotate." << std::endl;
//                break;

//            case Right:
//                savePicture(Right);
//                break;

//            case Down:
//                savePicture(Down);
//                break;

//            case Left:
//                savePicture(Left);
//                break;

//            case Unknown:
//                std::cout << "The image direction Unkown" << std::endl;
//                break;

//            default:
//            break;
//        }
        t_rotatedPix = getRotatedOrgPix(t_direction);
        t_saveAsThread = new saveAsThread(m_currentfile, t_rotatedPix);
        m_saveAsThreadlist.append(t_saveAsThread);
        t_saveAsThread->start();
    }
    else
        emit sig_saveDialog();
}

void mainTaskWidget::slot_saveDialog()
{
    m_view->setSaveTipText(QString(tr("您没有权限对当前文件进行更改")));
}


