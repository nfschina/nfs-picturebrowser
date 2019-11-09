#include "slidedialog.h"


menuBar::menuBar(QWidget *parent, Qt::WindowFlags f)
    :QFrame(parent, f)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setFixedSize(410, SLIDETOOLBARHEIGTH);
    setMouseTracking(true);
    m_isEntered = false;

    m_animation = new opacityAnimation(this);

    QFont ft;
    ft.setBold(true);
    ft.setPointSize(10.5);
    ft.setFamily("Waree");


    m_interval = new QLabel(this);
    m_interval->setFont(ft);
    m_interval->setStyleSheet("color:rgb(201,234,255);"
                              "border: 0px;"
                              "font: 14px;"
                              "background-color: rgb(0,0,0,0)");

    m_increase = new menuButton(QSize(16,16), this);
    m_increase->setToolTip(tr("增加间隔时间"));

    m_reduce = new menuButton(QSize(16,16), this);
    m_reduce->setToolTip(tr("减少间隔时间"));

    QWidget *t_widget = new QWidget(this);
    t_widget->setStyleSheet("background-color: rgb(0,0,0,0)");
    QHBoxLayout *t_2_hl = new QHBoxLayout;
    t_2_hl->setSpacing(1);
    t_2_hl->setMargin(0);
    t_2_hl->addStretch();
    t_2_hl->addWidget(m_reduce);
    t_2_hl->addWidget(m_interval);
    t_2_hl->addWidget(m_increase);
    t_2_hl->addStretch();

    t_widget->setLayout(t_2_hl);

    m_counter = new QLabel;
    m_counter->setAlignment(Qt::AlignCenter);
    m_counter->setFixedWidth(80);
    m_counter->setFont(ft);
    m_counter->setStyleSheet("color:rgb(201,234,255);"
                             "border: 0px;"
                             "background-color: rgb(0,0,0,0)");

    m_pause = new menuButton(QSize(34, 34));
    m_pause->setToolTip(tr("暂停/继续幻灯片"));

    m_effect = new menuButton((QSize(76,34)), this);


    m_quit = new menuButton((QSize(76, 34)), this);
    m_quit->setToolTip(tr("退出放映"));

    QFrame* t_line1 = new QFrame(this);
    t_line1->setStyleSheet("border: 1px inset grey;");
    t_line1->setFrameShape(QFrame::VLine);
    t_line1->setFrameShadow(QFrame::Sunken);

    QFrame* t_line2 = new QFrame(this);
    t_line2->setStyleSheet("border: 1px inset grey;");
    t_line2->setFrameShape(QFrame::VLine);
    t_line2->setFrameShadow(QFrame::Sunken);

    QHBoxLayout *t_1_hl = new QHBoxLayout;
    t_1_hl->setContentsMargins(4, 0, 6, 0);
    t_1_hl->setSpacing(20);

    t_1_hl->addWidget(m_counter);
    t_1_hl->addStretch();
    t_line1->setGeometry(85, 4, 1, 34);
    t_line2->setGeometry(width()-85, 4, 1, 34);
    t_widget->setGeometry(85, 0, 105, height());
    t_1_hl->addWidget(m_pause);
    m_effect->move(235, 5);
    t_1_hl->addStretch();
    t_1_hl->addWidget(m_quit);


    setLayout(t_1_hl);

    setStyleSheet("QFrame{background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgb(120, 120, 120, 150), stop:1 rgb(40, 40, 40, 150));"
                          "color: rgb(255 , 255, 255);"
                           "border-radius:20px;"
                           "font:15px;"
                           "border-width: 1px;"
                           "border-style: outset;"
                           "border-color: rgb(200, 200, 200);}");

    meneBarSetStyleSheet();

    hide();
}

menuBar::~menuBar()
{

}

bool menuBar::getIsEntered()
{
    return m_isEntered;
}

void menuBar::meneBarSetStyleSheet()
{
    m_increase->setStyleSheet("QPushButton{border-image:url(:/prefix3/3-images/increase_normal.png);}"
                              "QPushButton:hover{border-image:url(:/prefix3/3-images/increase_hover.png);}"
                              "QPushButton:pressed{border-image:url(:/prefix3/3-images/increase_pressed.png);}");

    m_reduce->setStyleSheet("QPushButton{border-image:url(:/prefix3/3-images/reduce-normal.png);}"
                            "QPushButton:hover{border-image:url(:/prefix3/3-images/reduce_hover.png);}"
                            "QPushButton:pressed{border-image:url(:/prefix3/3-images/reduce_pressed.png);}");

    m_pause->setStyleSheet("QPushButton{border-image:url(:/prefix3/3-images/pause_normal.png);}"
                           "QPushButton:hover{border-image:url(:/prefix3/3-images/pause_hover.png);}"
                           "QPushButton:pressed{border-image:url(:/prefix3/3-images/pause_pressed.png);}");

    m_effect->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix3/3-images/new_effect.png)}"
                            "QPushButton:hover{border-image:url(:/prefix3/3-images/new_hover.png);}"
                            "QPushButton:pressed{border-image:url(:/prefix3/3-images/new_pressed.png);}");

    m_quit->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix3/3-images/new_quit.png)}"
                          "QPushButton:hover{border-image:url(:/prefix3/3-images/new_hover.png);}"
                          "QPushButton:pressed{border-image:url(:/prefix3/3-images/new_pressed.png);}");
}


void menuBar::enterEvent(QEvent *e)
{
    m_isEntered = true;
}

void menuBar::leaveEvent(QEvent *e)
{
    m_isEntered = false;
}



slideDialog::slideDialog(QWidget *parent, Qt::WindowFlags f)
    :QDialog(parent, f)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setAutoFillBackground(true);
    setMouseTracking(true);
    setCursor(Qt::BlankCursor);
     movie = new QMovie();

     bool backgroundState;
     backgroundState = configList[5].at(2) == QChar('0')?true:false;
//set 幻灯片背景

//    if(configList[5].length() == 3)
//        if(configList[5].at(2) == QChar('0'))
       if(backgroundState)
       {
            backgroundColor.setColor(QPalette::Window, QColor(0, 0, 0));
       }
       else
       {
            backgroundColor.setBrush(QPalette::Window, QBrush(QPixmap(":/prefix3/3-images/wave.png")));
       }
//    else
//        if(configList[5].at(3) == QChar('0'))
//            pal.setColor(QPalette::Window, QColor(0, 0, 0));
//        else
//            pal.setBrush(QPalette::Window, QBrush(QPixmap(":/prefix3/3-images/wave.png")));

//    setPalette(pal);
    this->setPalette(this->backgroundColor);

    m_currentInterval = configList[5].at(0).toLatin1() - '0';
    m_loadSlideThread = new loadSlideThread;
    m_currentPix = QPixmap(0,0);
    m_i = 0;
    isRunning = true;
    imageReader = new ImageReader(true);

    /*设置幻灯片的特效*/
    if(configList[5].length() == 3)
        m_currentPlayEffectIndex = configList[5].at(1).toLatin1() - '0';
    else
        m_currentPlayEffectIndex = configList[5].at(2).toLatin1() - '0';

    m_pixLabel = new QLabel(this);
    m_pixLabel->setMouseTracking(true);

    QVBoxLayout *t_vl = new QVBoxLayout;
    t_vl->setMargin(0);
    t_vl->addWidget(m_pixLabel, 0, Qt::AlignCenter);

    this->setLayout(t_vl);

    //控制菜单栏的显隐
    m_1_timer = new QTimer(this);
    m_1_timer->setInterval(3001);
    connect(m_1_timer, SIGNAL(timeout()), this, SLOT(slot_hideMenuBar()));

    //切换图片定时器
    m_2_timer = new QTimer(this);
    connect(m_2_timer, SIGNAL(timeout()), this, SLOT(slot_updatePixmap()));

    m_menuBar = new menuBar(this);
    connect(m_menuBar->m_effect, SIGNAL(clicked()), this, SLOT(slot_showEffectDialog()));
    connect(m_menuBar->m_quit, SIGNAL(clicked()), this, SLOT(slot_quit()));
    connect(m_menuBar->m_increase, SIGNAL(clicked()), this, SLOT(slot_upInterval()));
    connect(m_menuBar->m_reduce, SIGNAL(clicked()), this, SLOT(slot_downInterval()));
    connect(m_menuBar->m_pause, SIGNAL(clicked()), this, SLOT(slot_setPlay()));

    m_effectDialog = new EffectDialog(m_currentPlayEffectIndex, this);
    m_effectDialog->hide();
    connect(m_effectDialog, SIGNAL(sig_sendID(int)), this, SLOT(slot_setCurrentPlayEffectIndex(int)));
}

slideDialog::~slideDialog()
{

}

#if 0
void slideDialog::obtainDefaultSetting()
{
    setUpDialog *setting= new setUpDialog;
    bool ok;
    m_currentPlayOrder = setting->getProperty("slide/playOrder").toInt(&ok,10);
    m_currentInterval = setting->getProperty("slide/playSpeed").toInt(&ok,10);
    m_currentPlayEffectIndex = setting->getProperty("slide/playEffectIndex").toInt(&ok,10);
    delete setting;
}
#endif

QSize slideDialog::getImageSize(const QString &filename)
{
    QImageReader imgReader(filename);
    return imgReader.size();
}
void slideDialog::playEffectSetting(int index)
{
    QPropertyAnimation *animation;
    animation= new QPropertyAnimation(m_pixLabel,"geometry");  


    int appWidth=QApplication::desktop()->width();
    int appHeight= QApplication::desktop()->height();

//    m_currentfile=m_loadSlideThread->m_allImagesInfo.at(m_currentNum).absoluteFilePath();


//    int picWidth;
//    picWidth=getImageSize(m_currentfile).width();
//    int picHeight;
//    picHeight=getImageSize(m_currentfile).height();

    animation->setStartValue(QRect(0, 0, 100, 30));
//    int realX;int realY;int realWidth;int realHeight;

//set the start point and the end point
    animation->setEndValue(QRect(0,0,appWidth,appHeight));

    switch(index)
    {
        case 0:
                animation->setDuration(m_currentInterval*1500);
                animation->setEasingCurve(QEasingCurve::OutElastic);
                animation->start();
                break;
        case 1:
                if (faderWidget) faderWidget->close();
                faderWidget = new FaderWidget(m_pixLabel);
                faderWidget->start();
                break;
        case 2:
                animation->setDuration(m_currentInterval*500);
                animation->setEasingCurve(QEasingCurve::TCBSpline);
                animation->start();
                break;
    }
}


void slideDialog::resizeEvent(QResizeEvent *e)
{
    m_menuBar->move((width()-400)/2, height()-DISTANCE-SLIDETOOLBARHEIGTH);
    //m_close->setGeometry(width()-m_close->width(), 0, 56, 28);
    QPoint gPos = m_menuBar->mapToParent(m_menuBar->m_effect->pos());
    m_effectDialog->move(gPos.x()-(m_effectDialog->width() - m_menuBar->m_effect->width())/2 +35,
                         height() - DISTANCE - SLIDETOOLBARHEIGTH - m_effectDialog->height()-2);
}

void slideDialog::mouseMoveEvent(QMouseEvent *e)
{
    //鼠标移动停止定时器
    if(m_1_timer->isActive())
    {}

    //如果鼠标在menubar上 直接返回 不开定时器 不隐藏
    if(m_menuBar->getIsEntered())
        return;

    //如果隐藏 则显示
    if(m_menuBar->isHidden())
    {
        this->setCursor(Qt::ArrowCursor);
        m_menuBar->m_animation->slot_showAnimation();
    }

    m_1_timer->start();
}

void slideDialog::mousePressEvent(QMouseEvent *e)
{
    QDialog::mousePressEvent(e);
    m_effectDialog->hide();
}

void slideDialog::paintEvent(QPaintEvent *e)
{

    QDialog::paintEvent(e);
    //QPainter painter(this);
    //painter.drawPixmap(0, this->height()-currentPix.height(), currentPix.width(), currentPix.height(), currentPix);
}

void slideDialog::slot_hideMenuBar()
{
    if(m_menuBar->getIsEntered() || !m_effectDialog->isHidden())
        return;

    m_menuBar->m_animation->slot_hideAnimation();
    m_1_timer->stop();
    this->setCursor(Qt::BlankCursor);
}

void slideDialog::slot_quit()
{
    sig_sendCurrentNum(m_currentNum); //退出幻灯片返回当前的序号

    m_2_timer->stop();

    if(m_loadSlideThread->isRunning())
        m_loadSlideThread->breakFlag = false;

    m_loadSlideThread->m_pixQueue.clear();

    m_i = 0;

    this->hide();
}

//定时器时间到 切换图片
void slideDialog::slot_updatePixmap()
{
    QString realFileName;
    int desktopW = QApplication::desktop()->width();
    int desktopH = QApplication::desktop()->height();
    switch(m_currentPlayEffectIndex)
    {
        case 0:break;
        case 1:
            playEffectSetting(m_currentPlayEffectIndex-1);
            break;
        case 2:
            playEffectSetting(m_currentPlayEffectIndex-1);
            break;
        case 3:
            playEffectSetting(m_currentPlayEffectIndex-1);
            break;
        case 4:
            playEffectSetting(qrand()%(m_currentPlayEffectIndex));
            break;
    default:
        break;
    }

    if(!m_loadSlideThread->m_pixQueue.isEmpty())
    {
        if(m_loadSlideThread->m_allImagesInfo.length() <= NUMOFBUFFER)
        {
                m_pixLabel->clear();
                realFileName = m_loadSlideThread->m_allImagesInfo.at(m_i).absoluteFilePath();
                imageReader->readImage(realFileName);
                m_currentNum = m_loadSlideThread->m_pixQueue.at(m_i).m_num;

                QImageReader reader;
                QByteArray ba = reader.imageFormat(imageReader->fileChanged);
                QMovie *movie = new QMovie(imageReader->fileChanged);
                if(ba == "gif")
                {
                   movie->stop();
                   movie->start();
                   m_pixLabel->setMovie(movie);
                }
                else
                {
                    QPixmap tmpPix;
                    tmpPix.load(imageReader->fileChanged);
                    if(tmpPix.width() > desktopW || tmpPix.height() > desktopH)
                    {
                        tmpPix = tmpPix.scaled(QSize(desktopW,desktopH),Qt::KeepAspectRatio);
                    }
                    m_pixLabel->setPixmap(tmpPix);
                }

            //顺序播放
            if(configList[4].at(0) == QChar('1'))
            {
                //循环控制变量 在 0～n 之间
                if(m_i == m_loadSlideThread->m_allImagesInfo.length()-1)
                    m_i = 0;
                else
                    m_i++;
            }
            else//随机播放(产生下一次播放的序号)
            {
                m_i = qrand() % m_loadSlideThread->m_allImagesInfo.length();
            }
        }
        else
        {
            imgInfo t_imgInfo = m_loadSlideThread->m_pixQueue.dequeue();                        
            m_currentNum = t_imgInfo.m_num;
/*this is very important*/
            realFileName = m_loadSlideThread->m_allImagesInfo.at(m_currentNum).absoluteFilePath();
            imageReader->readImage(realFileName);

/*if the current picture is gif*/
            QImageReader reader;
            QByteArray ba = reader.imageFormat(imageReader->fileChanged);
            QMovie *movie = new QMovie(imageReader->fileChanged);
            if(ba == "gif")
            {
               movie->stop();
               movie->start();
               m_pixLabel->setMovie(movie);
            }
            else
            {
                 m_pixLabel->setPixmap(t_imgInfo.m_pix);
            }
        }
        m_pixLabel->setAlignment(Qt::AlignCenter);
        m_menuBar->m_counter->setText(tr("第")+QString::number(m_currentNum + 1)+"/"+
                                      QString::number(m_total)+tr("张"));
    }
}

void slideDialog::slot_upInterval()
{
    if(m_currentInterval==9)
        return;

    m_2_timer->stop();
    m_currentInterval++;

    if(isRunning)
    {
        m_2_timer->start(m_currentInterval*1000);
    }

    m_menuBar->m_interval->setText(tr("间隔")+QString::number(m_currentInterval)+tr("秒"));
}

void slideDialog::slot_downInterval()
{
    if(m_currentInterval==1)
        return;

    m_2_timer->stop();
    m_currentInterval--;

    if(isRunning) //如果正在播放幻灯片 才打开定时器
    {
        m_2_timer->start(m_currentInterval*1000);
    }

    m_menuBar->m_interval->setText(tr("间隔")+QString::number(m_currentInterval)+tr("秒"));
}


void slideDialog::slot_setPlay()
{
    if(isRunning)
    {
        m_2_timer->stop();
        isRunning = false;
        m_menuBar->m_pause->setStyleSheet("QPushButton{border-image:url(:/prefix3/3-images/play_normal.png);}"
                                          "QPushButton:hover{border-image:url(:/prefix3/3-images/play_hover.png);}"
                                          "QPushButton:pressed{border-image:url(:/prefix3/3-images/play_pressed.png);}");
    }
    else
    {
        m_2_timer->start(m_currentInterval*1000);       
        isRunning = true;
        m_menuBar->m_pause->setStyleSheet("QPushButton{border-image:url(:/prefix3/3-images/pause_normal.png);}"
                                          "QPushButton:hover{border-image:url(:/prefix3/3-images/pause_hover.png);}"
                                          "QPushButton:pressed{border-image:url(:/prefix3/3-images/pause_pressed.png);}");
    }
}

void slideDialog::slot_showEffectDialog()
{
    if(m_effectDialog->isHidden())
        m_effectDialog->show();
    else
        m_effectDialog->hide();
}

void slideDialog::slot_setCurrentPlayEffectIndex(int id)
{
    m_currentPlayEffectIndex = id;
}

//开启幻灯片执行的第一个函数
void slideDialog::setCurrentPixmap(QString &currentFile)
{
    int desktopW = QApplication::desktop()->width();
    int desktopH = QApplication::desktop()->height();

    //将图片缩放到 适应全屏
    if(m_currentPix.width() > desktopW || m_currentPix.height() > desktopH)
    {
        m_currentPix = m_currentPix.scaled(QSize(desktopW, desktopH), Qt::KeepAspectRatio);
    }

    //在label上设置图片
/*if the current picture is gif*/
    QImageReader t_reader;
    QByteArray ba = t_reader.imageFormat(currentFile);
    QMovie *movie = new QMovie(currentFile);
    if(ba == "gif")
    {
       movie->stop();
       movie->start();
       m_pixLabel->setMovie(movie);
    }
    else
    {
         m_pixLabel->setPixmap(m_currentPix);
    }

    //开启线程来预加载5张图片
    if(m_loadSlideThread->m_allImagesInfo.length() > 1)
    {
        m_loadSlideThread->start();
    }
    else//如果只有一张不开线程  直接将图片放入线程的缓冲队列中
    {
        imgInfo t_imgInfo;
        t_imgInfo.m_pix = m_currentPix;
        t_imgInfo.m_num = 0;
        m_loadSlideThread->m_pixQueue.enqueue(t_imgInfo);
    }
    //打开播放幻灯片的定时器

    m_2_timer->start(m_currentInterval*1000);

    //设置显示label
    m_menuBar->m_interval->setText(tr("间隔")+QString::number(m_currentInterval)+tr("秒"));
    m_menuBar->m_counter->setText("第"+QString::number(m_currentNum+1)+"/"+
                                  QString::number(m_total)+tr("张"));
}


FaderWidget::FaderWidget(QWidget *parent)
        : QWidget(parent)
{
    if (parent)
        startColor = parent->palette().window().color();
    else
        startColor = Qt::white;

    currentAlpha = 0;
    duration = 1000;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));

    setAttribute(Qt::WA_DeleteOnClose);
    resize(parent->size());
}

void FaderWidget::start()
{
    currentAlpha = 255;
    timer->start(100);
    show();
}

void FaderWidget::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    QColor semiTransparentColor = startColor;
    semiTransparentColor.setAlpha(currentAlpha);
    painter.fillRect(rect(), semiTransparentColor);

    currentAlpha -= 255 * timer->interval() / duration;
    if (currentAlpha <= 0)
    {
        timer->stop();
        close();
    }
}



