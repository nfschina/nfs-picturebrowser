#include "widget.h"

extern QString configList[ROWS];
extern QString globalParameter;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{        
   // setWindowFlags(Qt::FramelessWindowHint /*| Qt::X11BypassWindowManagerHint*/);
   // setAttribute(Qt::WA_TranslucentBackground, true);
    setMinimumHeight(MINHEIGTH);
    setMinimumWidth(MINWEITH);
    setFocusPolicy(Qt::StrongFocus);

    setWindowTitle(tr("看图工具"));

/*mouse move event*/
    isLeftPressed=false;
    curPos=0;//标记鼠标左击时的位置
    QCursor cursor;
    cursor.setShape(Qt::ArrowCursor);
    QWidget::setCursor(cursor);//当放在主窗口上时，为手形

    deskWidth = QApplication::desktop()->width();
    deskHeigth = QApplication::desktop()->height();
    //不包含任务栏 screenGeometry()是获得整个屏幕大小
    deskRect = QApplication::desktop()->availableGeometry();
    f = Normal;
    m_isFullScreen = false;   

    drag = new DragProxy(this);
    int defaultWidth = 5;
    drag->SetBorderWidth(defaultWidth,defaultWidth,defaultWidth,defaultWidth);

    m_titleBar = new titleBarWidget;
    m_stackedWidget = new stackedWidget;

    connect(this->m_stackedWidget->m_2_widget->m_mainTaskWidget,SIGNAL(sig_normalSetting1(bool)),SLOT(slot_normalSetting1(bool)));
    m_stackedWidget->m_appTitleBar = m_titleBar; //隐藏最大化图标用


    QVBoxLayout *t_2_vl = new QVBoxLayout;
    t_2_vl->setContentsMargins(0, 0, 0, 0);
    t_2_vl->setSpacing(0);
    t_2_vl->addWidget(m_stackedWidget);

    vl = new QVBoxLayout;
    vl->addWidget(m_titleBar);
    vl->addLayout(t_2_vl);
    vl->setMargin(5);
    vl->setSpacing(0);
    this->setLayout(vl);

    //窗口的大小 根据配置文件中保存窗口为是否置位 如果置位根据config来决定 窗口的 初始大小
    //否则为（740, 450）
    QDesktopWidget* desktop = QApplication::desktop(); // =qApp->desktop();也可以
    if(configList[1].at(0) == QChar('1'))
        setGeometry((desktop->width() - this->width())/2, (desktop->height() - this->height())/3, configList[2].toInt(), configList[3].toInt());
    else
        setGeometry(500, 250, 740, 450);

//update  name of the picture
    connect(m_stackedWidget->m_2_widget->m_mainTaskWidget,SIGNAL(sig_sendToInfoWidget(QFileInfo,QStringList)),
            m_titleBar,SLOT(slot_showTitlePicName(const QFileInfo &, const QStringList &)));

    connect(this->m_titleBar->closeBtn, SIGNAL(clicked()), this, SLOT(close()));
    connect(this->m_titleBar->maxBtn, SIGNAL(clicked()), this, SLOT(slot_showMaximized()));

    connect(this->m_titleBar->minBtn, SIGNAL(clicked()), this, SLOT(showMinimized()));
    connect(this->m_titleBar->fullScreenBtn, SIGNAL(clicked()), this, SLOT(slot_showFullScreen()));
    connect(this->m_titleBar,SIGNAL(sig_NormalOrMaximized()), this,SLOT(slot_showMaximized()));
//    sig_moveDoubleClick
    connect(this->m_titleBar->m_moveWidget,SIGNAL(sig_moveDoubleClick()),this,SLOT(slot_showMaximized()));
    connect(this->m_stackedWidget->m_2_widget->m_mainTaskWidget->m_view,
            SIGNAL(sig_doubleClick()), this, SLOT(slot_showFullScreen()));
/*hide the picture infomation*/
    connect(this->m_stackedWidget->m_2_widget->m_mainTaskWidget->m_view,SIGNAL(sig_singleClick()),this,SLOT(slot_picInfoDetail()));

    connect(this, SIGNAL(sig_pressedUp()), this->m_stackedWidget->m_2_widget->m_mainTaskWidget->m_view,
            SLOT(slot_largerImage()));
    connect(this, SIGNAL(sig_pressedDown()), this->m_stackedWidget->m_2_widget->m_mainTaskWidget->m_view,
            SLOT(slot_reduceImage()));
    connect(this, SIGNAL(sig_pressedLeft()), this->m_stackedWidget->m_2_widget->m_mainTaskWidget,
            SLOT(slot_prev()));
    connect(this, SIGNAL(sig_pressedRight()), this->m_stackedWidget->m_2_widget->m_mainTaskWidget,
            SLOT(slot_next()));
    connect(this, SIGNAL(sig_pressedDelete()), this->m_stackedWidget->m_2_widget->m_mainTaskWidget,
            SLOT(slot_removeImg()));
    connect(this, SIGNAL(sig_pressedF5()), this->m_stackedWidget->m_2_widget->m_mainTaskWidget,
            SLOT(slot_refresh()));
    connect(this, SIGNAL(sig_pressedControlAndS()), this->m_stackedWidget->m_2_widget->m_mainTaskWidget,
            SLOT(slot_saveAs()));
    connect(this, SIGNAL(sig_pressedControlAndO()), this->m_stackedWidget->m_2_widget->m_mainTaskWidget,
            SLOT(slot_openFile()));
    connect(this, SIGNAL(sig_pressedControlAndL()), this->m_stackedWidget->m_2_widget->m_mainTaskWidget->m_view,
            SLOT(slot_setAsWallpaper()));
    connect(this, SIGNAL(sig_pressedControlAndC()), this->m_stackedWidget->m_2_widget->m_mainTaskWidget,
            SLOT(slot_copyImg()));

}

void Widget::slot_normalSetting1(bool isChecked)
{
    if(isChecked)
    {
        this->m_titleBar->m_moveWidget->m_filename->setText(this->m_titleBar->titleName);
    }
    else
    {
        this->m_titleBar->m_moveWidget->m_filename->setText(" ");
    }
}
void Widget::mousePressEvent(QMouseEvent *event)//鼠标按下事件
{
  if(event->button()==Qt::LeftButton)
  {
    this->isLeftPressed=true;
    QPoint temp=event->globalPos();
    pLast=temp;
    curPos=countFlag(event->pos(),countRow(event->pos()));
    event->ignore();
  }
}

void Widget::mouseReleaseEvent(QMouseEvent *event)//鼠标释放事件
{
  if(isLeftPressed)
  {
    isLeftPressed=false;
  }
  QApplication::restoreOverrideCursor();//恢复鼠标指针性状
  event->ignore();
}

void Widget::mouseDoubleClickEvent(QMouseEvent *event)//鼠标双击 全屏
{
  if(event->button()==Qt::LeftButton)
  {
    /*if(windowState()!=Qt::WindowFullScreen)
    {
        setWindowState(Qt::WindowFullScreen);
    }
    else
    {
        setWindowState(Qt::WindowNoState);
    }//恢复正常模式*/
    slot_showFullScreen();
  }
  event->ignore();
}

void Widget::mouseMoveEvent(QMouseEvent *event)//鼠标移动事件
{    
}

int Widget::countFlag(QPoint p,int row)//计算鼠标在哪一列和哪一行
{
  if(p.y()<MARGIN)
    return 10+row;
  else if(p.y()>this->height()-MARGIN)
    return 30+row;
  else
    return 20+row;
}

void Widget::setCursorType(int flag)//根据鼠标所在位置改变鼠标指针形状
{
  Qt::CursorShape cursor;
  switch(flag)
  {
  case 11:
  case 33:
    cursor=Qt::SizeFDiagCursor;break;
  case 13:
  case 31:
    cursor=Qt::SizeBDiagCursor;break;
  case 21:
  case 23:
    cursor=Qt::SizeHorCursor;break;
  case 12:
  case 32:
    cursor=Qt::SizeVerCursor;break;
  case 22:
    cursor=Qt::OpenHandCursor;break;
  default:
     //  QApplication::restoreOverrideCursor();//恢复鼠标指针性状
     break;

  }
  setCursor(cursor);
}

int Widget::countRow(QPoint p)//计算在哪一列
{
  return (p.x()<MARGIN)?1:(p.x()>(this->width()-MARGIN)?3:2);
}


Widget::~Widget()
{

}

//如果带参数 打开图片文件
void Widget::openPicture(const QString &filename)
{
    m_stackedWidget->m_1_widget->sig_openFile(filename);
}

void Widget::defaultSettingFunc()
{
//    QProcess *setProc=new QProcess;
//    QString strText="xdg-mime default picturebrowser.desktop image/jpeg";
//    setProc->start(strText);

#if 0
    QProcess::execute("xdg-mime default picturebrowser.desktop image/bmp");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/bitmap");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/gif");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/tif");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/tiff");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/jpg");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/jpeg");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/jpeg2000");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/jpc");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/j2k");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/jpf");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/png");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/pbm");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/pgm");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/x-psd");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/jp2");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/x-ico");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/icns");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/dib");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/x-pcx");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/x-xcf");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/svg");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/svgz");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/pnm");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/ras");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/wbmp");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/ani");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/eps");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/ppm");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/xbm");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/xpm");


    QProcess::execute("xdg-mime default picturebrowser.desktop image/BMP");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/BITMAP");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/GIF");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/TIF");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/TIFF");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/JPG");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/JPEG");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/JPEG2000");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/JPC");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/J2K");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/JPF");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/PNG");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/PBM");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/PGM");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/X-PSD");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/JP2");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/X-ICO");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/ICNS");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/DIB");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/X-PCX");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/X-XCF");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/SVG");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/SVGZ");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/PNM");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/RAS");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/WBMP");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/ANI");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/EPS");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/PPM");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/XBM");
    QProcess::execute("xdg-mime default picturebrowser.desktop image/XPM");
#endif

}
void Widget::defaultSettingCheck()
{
//get the default configure file
    QString fileName=QDir::homePath()+"/.local/share/applications/mimeapps.list";
    QFile configureFile(fileName);
//check the configure file is exist or not
    if(!configureFile.exists())
    {
//create the configure file
        configureFile.open(QIODevice::WriteOnly);
        configureFile.close();
    }
//start to check the configuration
    QSettings *defaultSettingConfigure = new QSettings(fileName, QSettings::NativeFormat);

    QString suffix=QFileInfo(globalParameter).suffix();
    if(suffix == "jpg") suffix="jpeg";
    QString keyString="Default Applications/image/"+suffix;

    if(defaultSettingConfigure->value(keyString,"").toString() == PICTUREBROWSER_DESKTOP)
    {
//do nothing
    }
    else
    {
//update the default setting
        defaultSettingFunc();
    }
}


void Widget::paintEvent(QPaintEvent *e)
{
     Q_UNUSED(e);
    if(!isFullScreen())   //全屏时不需要画框
    {
        QPainter painter(this);
        painter.setPen(QColor(0, 0, 0,0));
        painter.drawRect(5, 33, width()-11, height()-38);
    }
}

void Widget::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
      {
          case Qt::Key_Escape:
                if(this->isFullScreen())
                      slot_showFullScreen();
                break;

          case Qt::Key_Up:
                if(this->m_stackedWidget->currentIndex() == 1)
                    sig_pressedUp();
                break;

          case Qt::Key_Down:
                if(this->m_stackedWidget->currentIndex() == 1)
                    sig_pressedDown();
                break;

          case Qt::Key_Left:
                if(this->m_stackedWidget->currentIndex() == 1 && !e->isAutoRepeat())
                    sig_pressedLeft();
                break;

          case Qt::Key_Right:
                if(this->m_stackedWidget->currentIndex() == 1 && !e->isAutoRepeat())
                    sig_pressedRight();
                break;

         case Qt::Key_Delete:
                if(this->m_stackedWidget->currentIndex() == 1)
                    sig_pressedDelete();
                break;

        case Qt::Key_F5:
                if(this->m_stackedWidget->currentIndex() == 1)
                     sig_pressedF5();
                break;

        default:
                if((e->modifiers() & Qt::ShiftModifier) && (e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_S)
                {
                    if(this->m_stackedWidget->currentIndex() == 1)
                        sig_pressedControlAndS();
                }

                if(e->modifiers() == Qt::ControlModifier && e->key() == Qt::Key_O)
                {
                    if(this->m_stackedWidget->currentIndex() == 1)
                        sig_pressedControlAndO();
                }

                if(e->modifiers() == Qt::ControlModifier && e->key() == Qt::Key_L)
                {
                    if(this->m_stackedWidget->currentIndex() == 1)
                        sig_pressedControlAndL();
                }
                if(e->modifiers() == Qt::ControlModifier && e->key() == Qt::Key_C)
                {
                    if(this->m_stackedWidget->currentIndex() == 1)
                        sig_pressedControlAndC();
                }

                QWidget::keyPressEvent(e);
                break;
    }
}

void Widget::closeEvent(QCloseEvent *e)
{
    Q_UNUSED(e);

    QDir configDir(QDir::homePath() + "/" + PICTUREBROWSER);
    if(!configDir.exists())
        configDir.mkdir(QDir::homePath() + "/" + PICTUREBROWSER);

    QFile file(QDir::homePath() + "/" + PICTUREBROWSER + "/" + "config");

    if(file.exists())
        file.resize(0);

    if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
        std::cout << "open failed for read" << std::endl;

    QTextStream in(&file);

    //写六行
    in << configList[0] << "\n";
    in << configList[1] << "\n";
    in << width() << "\n";
    in << height() << "\n";
    in << configList[4] << "\n";
    in << configList[5];

    file.close();
}

void Widget::resizeEvent(QResizeEvent *e)
{
    Q_UNUSED(e);
    //当showfullscreen后 过滤掉第一次 resizeevent
    if(m_isFullScreen &&  rect() != QApplication::desktop()->screenGeometry())
    {
        return;
    }

    if(!drag->m_mousePressed)
    {    
        if(deskRect == rect())
        {
            vl->setMargin(0);
            m_stackedWidget->setContentsMargins(0,0,0,0);
            f = Maximized;
            m_titleBar->maxBtnSetStyleSheet(false);     
        }
        else if(deskRect.contains(rect()))
        {
            vl->setMargin(0);
            m_stackedWidget->setContentsMargins(0,0,0,0);
            f = Normal;
            m_titleBar->maxBtnSetStyleSheet(true);            
        }
        else//全屏边距为0
        {
            vl->setMargin(0);
            m_stackedWidget->setContentsMargins(0,0,0,0);            
        }
    }
}

void Widget::slot_picInfoDetail()
{

    switch (this->m_stackedWidget->m_2_widget->m_mainTaskWidget->sOhFlag)
    {
    case Hide:
//        this->m_stackedWidget->m_2_widget->m_PicInfoWidget->show();
//        this->m_stackedWidget->m_2_widget->f=Show;
        break;
    case Show:
        this->m_stackedWidget->m_2_widget->m_mainTaskWidget->m_PicInfoWidget->hide();

        this->m_stackedWidget->m_2_widget->m_mainTaskWidget->sOhFlag=Hide;
        break;
    default:
        break;
 }
}

void Widget::slot_showMaximized()
{
    if(f == Maximized) {
        this->showNormal();
        f = Normal;
    }
    else if(f == Normal)  {
        winWidth = this->geometry().width();
        winHeight = this->geometry().height();
        winX = this->pos().x();
        winY = this->pos().y();
        this->showMaximized();
        f = Maximized;
    }
    /*switch (f)
    {
    case Normal:
        winWidth = this->geometry().width();
        winHeight = this->geometry().height();
        winX = this->pos().x();
        winY = this->pos().y();
        this->showMaximized();
        f = Maximized;
        break;

    case Maximized:
        this->showNormal();
        f = Normal;
        break;

    default:
        break;
    }*/
}

void Widget::slot_showFullScreen()
{
    if(this->isFullScreen())
    {
        m_isFullScreen = false;
      //  m_titleBar->show();        

        //改变背景颜色
        m_stackedWidget->m_2_widget->m_mainTaskWidget->m_view->scene->setBackgroundBrush(QColor(235,241,255));        
        //切换工具栏
        m_stackedWidget->m_2_widget->m_mainTaskWidget->m_bottomWidget->changeToolBar();

        if(this->f == Maximized) {
            this->showMaximized();
            //f = Maximized;
        }
        else {
            this->showNormal();
            //f = Normal;
        }

    }
    else
    {
        //改变背景颜色
        m_stackedWidget->m_2_widget->m_mainTaskWidget->m_view->scene->setBackgroundBrush(Qt::black);
        //切换工具栏
        m_stackedWidget->m_2_widget->m_mainTaskWidget->m_bottomWidget->changeToolBar();

        m_titleBar->hide();
        m_isFullScreen = true;
        //this->f = this->isMaximized() ? Maximized : Normal;

	if(this->f == Normal) {
            winWidth = this->geometry().width();
            winHeight = this->geometry().height();
            winX = this->pos().x();
            winY = this->pos().y();
        }
        this->showFullScreen();  //执行两次resizevent 第一次变为Normal 第二次变为全屏
    }
}

void Widget::showMaximized()
{
    //QWidget::showMaximized();
    QWidget::showNormal();
    this->resize(QSize(deskWidth, deskHeigth));
}

void Widget::showNormal()
{
    QWidget::showNormal();
    this->resize(QSize(winWidth, winHeight));
    this->move(winX, winY);
}
