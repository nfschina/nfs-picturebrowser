#include "bottomwidget.h"

extern QString configList[ROWS];

bottomWidgt::bottomWidgt(QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAutoFillBackground(true);

    //背景色为透明色的
    QPalette t_pal;
    t_pal.setColor(QPalette::Window, QColor(255, 255, 255, 0));
    this->setPalette(t_pal);

    m_thumbnailView = new smallPictureView;
    m_smallToolBar = new smallToolBarWidget;
    m_toolbar = new toolBarWidget;
    m_toolbar->m_smallToolBar_p = m_smallToolBar;
    m_smallToolBar->hide();

    connect(m_smallToolBar->m_picInfo, SIGNAL(clicked()), m_toolbar->m_picInfo, SIGNAL(clicked()));
    connect(m_smallToolBar->m_nail, SIGNAL(clicked()), m_toolbar->m_nail, SIGNAL(clicked()));
    connect(m_smallToolBar->m_thumbnail, SIGNAL(clicked()), m_toolbar->m_thumbnail, SIGNAL(clicked()));
    connect(m_smallToolBar->m_file, SIGNAL(clicked()), m_toolbar->m_file, SIGNAL(clicked()));
    connect(m_smallToolBar->m_left, SIGNAL(clicked()), m_toolbar->m_left, SIGNAL(clicked()));
    connect(m_smallToolBar->m_right, SIGNAL(clicked()), m_toolbar->m_right, SIGNAL(clicked()));
    connect(m_smallToolBar->m_rotating, SIGNAL(clicked()), m_toolbar->m_rotating, SIGNAL(clicked()));
    connect(m_smallToolBar->m_magnifying, SIGNAL(clicked()), m_toolbar->m_magnifying, SIGNAL(clicked()));
    connect(m_smallToolBar->m_reduce, SIGNAL(clicked()), m_toolbar->m_reduce, SIGNAL(clicked()));
    connect(m_smallToolBar->m_slide, SIGNAL(clicked()), m_toolbar->m_slide, SIGNAL(clicked()));
    connect(m_smallToolBar->m_one_one, SIGNAL(clicked()), m_toolbar->m_one_one, SIGNAL(clicked()));
    connect(m_smallToolBar->m_delete, SIGNAL(clicked()), m_toolbar->m_delete, SIGNAL(clicked()));
    connect(m_smallToolBar->m_beautify, SIGNAL(clicked()), m_toolbar->m_beautify, SIGNAL(clicked()));

    m_1_vl = new QVBoxLayout;
    m_1_vl->setMargin(0);
    m_1_vl->setSpacing(0);
    m_1_vl->addWidget(m_thumbnailView);
    m_1_vl->addWidget(m_toolbar, 0, Qt::AlignBottom);
    m_1_vl->addWidget(m_smallToolBar, 0, Qt::AlignBottom | Qt::AlignHCenter);

    setLayout(m_1_vl);

    //设置默认 隐藏还是显示
    if(configList[0].at(1) == QChar('1'))   //显示
    {
        m_isShowed = true;
        m_transparentValue = 1;       //默认透明度为 1
        m_toolbar->m_isfixed = true;
        show();
    }
    else                                    //隐藏
    {
        m_isShowed = false;
        m_transparentValue = 0;       //默认透明度为 0 全透明
        hide();
    }


    m_effect = new QGraphicsOpacityEffect(this);
    m_effect->setOpacity(m_transparentValue);
    setGraphicsEffect(m_effect);

    m_1_timer = new QTimer(this);
    connect(m_1_timer, SIGNAL(timeout()), this, SLOT(slot_reduceTransparency()));

    m_2_timer = new QTimer(this);
    m_2_timer->setInterval(7);
    connect(m_2_timer, SIGNAL(timeout()), this, SLOT(slot_increaseTransparency()));

    connect(m_toolbar->m_thumbnail, SIGNAL(clicked()), this, SLOT(slot_smallPictureViewShowOrHide()));

    m_toolbar->setStyleSheetNail();   //设置钉子的样式
}

bottomWidgt::~bottomWidgt()
{

}

void bottomWidgt::mouseDoubleClickEvent(QMouseEvent *e)
{

}

void bottomWidgt::changeToolBar()
{
    if(m_toolbar->isHidden())
    {
        m_toolbar->show();
        m_smallToolBar->hide();
    }
    else
    {
        m_toolbar->hide();
        m_smallToolBar->show();
    }

}

void bottomWidgt::adjustSize(int h, int d)
{
    if(m_thumbnailView->getShowOrHide() == Show)
    {
        m_thumbnailView->hide();

        setGeometry(0, parentWidget()->height()- h - d,
                        parentWidget()->width(), h);
        m_thumbnailView->setShowOrHide(Hide);
    }
    else
    {
        setGeometry(0, parentWidget()->height() - (h + THUMBNAILHEIGTH) - d,
                 parentWidget()->width(), (h + THUMBNAILHEIGTH));
        m_thumbnailView->setShowOrHide(Show);
        m_thumbnailView->show();

        sig_needSetItemPos();
    }

}

//点击打开或关闭缩略图按钮
void bottomWidgt::slot_smallPictureViewShowOrHide()
{
    //当非全屏时
    if(!parentWidget()->parentWidget()->parentWidget()->parentWidget()->isFullScreen())
    {
        //调整高度为 TOOLBARHEIGTH 距离屏幕底部为0
        adjustSize(TOOLBARHEIGTH, 0);
    }
    else//当全屏点击缩略图按钮时 为小工具栏 高度为SMALLTOOLBARHEIGTH  距离地步距离为DISTANCE
    {
        adjustSize(SMALLTOOLBARHEIGTH, DISTANCE);
    }
}

void bottomWidgt::slot_showAnimation()
{
    if(!m_isShowed)
    {
        this->show();
        m_2_timer->stop();
        m_1_timer->start(0.05);
        m_isShowed = true;
    }
}

void bottomWidgt::slot_hideAnimation()
{
    if(m_toolbar->m_isfixed)//如果固定符号为真 则不隐藏
        return;

    if(m_isShowed)
    {
        m_1_timer->stop();
        m_2_timer->start(7);
        m_isShowed = false;
    }
}

void bottomWidgt::slot_increaseTransparency()
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

void bottomWidgt::slot_reduceTransparency()
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

void bottomWidgt::slot_statTimer2()
{
    if(m_isShowed)
        return;
    else
        m_2_timer->start(7);
}

toolBarButton::toolBarButton(int w, int h, QWidget *parent)
{
    setFocusPolicy(Qt::NoFocus);
    setFixedSize(w, h);
}

toolBarButton::~toolBarButton()
{

}

toolBarWidget::toolBarWidget(QWidget *parent) : QWidget(parent)
{
    setFixedHeight(TOOLBARHEIGTH);

    //刷渐变背景
    this->setAutoFillBackground(true);
    QLinearGradient linearGradient(0, 0, 0, TOOLBARHEIGTH);
    linearGradient.setColorAt(0, QColor(255, 255, 255, 220));
    linearGradient.setColorAt(1,QColor(255, 255, 255, 220));

    QPalette t_pal;
    t_pal.setBrush(QPalette::Window, linearGradient);
    this->setPalette(t_pal);

    m_picInfo = new toolBarButton;
    m_picInfo->setToolTip(tr("图片信息"));
    m_magnifying = new toolBarButton;
    m_magnifying->setAutoRepeat(true);
    m_magnifying->setToolTip(tr("放大"));
    m_reduce = new toolBarButton;
    m_reduce->setAutoRepeat(true);
    m_reduce->setToolTip(tr("缩小"));
    m_beautify = new toolBarButton;
    m_beautify->setToolTip(tr("编辑图片"));
    m_share = new toolBarButton;
    m_share->setToolTip(tr("将图片分享到..."));

    m_left = new toolBarButton(36, 32);
    m_left->setAutoRepeat(true);
    m_left->setToolTip(tr("上一张"));
    m_right = new toolBarButton(36, 32);
    m_right->setAutoRepeat(true);
    m_right->setToolTip(tr("下一张"));

    m_one_one = new toolBarButton;
    m_one_one->setToolTip(tr("实际尺寸"));
    m_rotating = new toolBarButton;
    m_rotating->setToolTip(tr("旋转"));
    m_slide = new toolBarButton;
    m_slide->setToolTip(tr("幻灯片"));
    m_delete = new toolBarButton;
    m_delete->setToolTip(tr("删除"));
    m_file = new toolBarButton;
    m_file->setToolTip(tr("打开新的图片"));
    m_thumbnail = new toolBarButton;
    m_thumbnail->setToolTip(tr("缩略图"));

    m_nail = new toolBarButton;
    m_nail->setToolTip(tr("固定/隐藏"));

    //默认不固定工具栏
    m_isfixed = false;
    connect(m_nail, SIGNAL(clicked()), this, SLOT(slot_setIsfixed()));

    QHBoxLayout *t_1_hl = new QHBoxLayout;
    t_1_hl->addWidget(m_picInfo);
    t_1_hl->addSpacing(30);
    t_1_hl->addStretch(1);
    t_1_hl->addWidget(m_magnifying);
    t_1_hl->addWidget(m_reduce);
    t_1_hl->addWidget(m_one_one);
    t_1_hl->addWidget(m_beautify);
//this function will open on future
//    t_1_hl->addWidget(m_share);

    t_1_hl->addWidget(m_left);
    t_1_hl->addSpacing(1);
    t_1_hl->addWidget(m_right);

    t_1_hl->addWidget(m_rotating);
    t_1_hl->addWidget(m_delete);
    t_1_hl->addWidget(m_slide);
    t_1_hl->addWidget(m_file);
    t_1_hl->addWidget(m_nail);
    t_1_hl->addStretch(1);
    t_1_hl->addSpacing(30);
    t_1_hl->addWidget(m_thumbnail);
    t_1_hl->setMargin(0);
    t_1_hl->setSpacing(10);

    this->setLayout(t_1_hl);

    toolBarSetStyleSheet();
}

toolBarWidget::~toolBarWidget()
{
}

void toolBarWidget::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

}

void toolBarWidget::toolBarSetStyleSheet()
{
    m_picInfo->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix2/2-images/qie_35.png)}"
                             "QPushButton:hover{border-image:url(:/prefix2/2-images/hover_small.png);}"
                             "QPushButton:pressed{border-image:url(:/prefix2/2-images/pressed_small.png);}");

    m_magnifying->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix2/2-images/qie_37.png)}"
                                "QPushButton:hover{border-image:url(:/prefix2/2-images/hover_small.png);}"
                                "QPushButton:pressed{border-image:url(:/prefix2/2-images/pressed_small.png);}");

    m_reduce->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix2/2-images/qie_39.png)}"
                            "QPushButton:hover{border-image:url(:/prefix2/2-images/hover_small.png);}"
                            "QPushButton:pressed{border-image:url(:/prefix2/2-images/pressed_small.png);}");

    m_one_one->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix2/2-images/qie_41.png)}"
                             "QPushButton:hover{border-image:url(:/prefix2/2-images/hover_small.png);}"
                             "QPushButton:pressed{border-image:url(:/prefix2/2-images/pressed_small.png);}");

    m_beautify->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix2/2-images/qie_43.png)}"
                              "QPushButton:hover{border-image:url(:/prefix2/2-images/hover_small.png);}"
                              "QPushButton:pressed{border-image:url(:/prefix2/2-images/pressed_small.png);}");

    m_share->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix2/2-images/qie_45.png)}"
                           "QPushButton:hover{border-image:url(:/prefix2/2-images/hover_small.png);}"
                           "QPushButton:pressed{border-image:url(:/prefix2/2-images/pressed_small.png);}");

    m_left->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix2/2-images/qie_47.png)}"
                          "QPushButton:hover{border-image:url(:/prefix2/2-images/hover_big.png);}"
                          "QPushButton:pressed{border-image:url(:/prefix2/2-images/pressed_big.png);}");

    m_right->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix2/2-images/qie_49.png)}"
                           "QPushButton:hover{border-image:url(:/prefix2/2-images/hover_big.png);}"
                           "QPushButton:pressed{border-image:url(:/prefix2/2-images/pressed_big.png);}");

    m_rotating->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix2/2-images/qie_53.png)}"
                              "QPushButton:hover{border-image:url(:/prefix2/2-images/hover_small.png);}"
                              "QPushButton:pressed{border-image:url(:/prefix2/2-images/pressed_small.png);}");

    m_delete->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix2/2-images/qie_55.png)}"
                            "QPushButton:hover{border-image:url(:/prefix2/2-images/hover_small.png);}"
                            "QPushButton:pressed{border-image:url(:/prefix2/2-images/pressed_small.png);}");

    m_slide->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix2/2-images/qie_57.png)}"
                           "QPushButton:hover{border-image:url(:/prefix2/2-images/hover_small.png);}"
                           "QPushButton:pressed{border-image:url(:/prefix2/2-images/pressed_small.png);}");

    m_file->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix2/2-images/qie_59.png)}"
                          "QPushButton:hover{border-image:url(:/prefix2/2-images/hover_small.png);}"
                          "QPushButton:pressed{border-image:url(:/prefix2/2-images/pressed_small.png);}");

    m_thumbnail->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix2/2-images/qie_63.png)}"
                                                       "QPushButton:hover{border-image:url(:/prefix2/2-images/hover_small.png);}"
                                                       "QPushButton:pressed{border-image:url(:/prefix2/2-images/pressed_small.png);}");
}

void toolBarWidget::setStyleSheetNail()
{
    if(m_isfixed)
    {
        m_nail->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix2/2-images/qie_61.png)}"
                          "QPushButton:hover{border-image:url(:/prefix2/2-images/hover_small.png);}"
                          "QPushButton:pressed{border-image:url(:/prefix2/2-images/pressed_small.png);}");
        m_smallToolBar_p->m_nail->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix2/2-images/s_63.png)}"
                                              "QPushButton:hover{border-image:url(:/prefix2/2-images/s_hover_small.png);}"
                                              "QPushButton:pressed{border-image:url(:/prefix2/2-images/s_pressed_small.png);}");
    }
    else
    {
        m_nail->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix2/2-images/nail_1.png)}"
                              "QPushButton:hover{border-image:url(:/prefix2/2-images/hover_small.png);}"
                              "QPushButton:pressed{border-image:url(:/prefix2/2-images/pressed_small.png);}");
        m_smallToolBar_p->m_nail->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix2/2-images/nail_2.png)}"
                                              "QPushButton:hover{border-image:url(:/prefix2/2-images/s_hover_small.png);}"
                                              "QPushButton:pressed{border-image:url(:/prefix2/2-images/s_pressed_small.png);}");

    }
}

//更改1：1按钮图片
void toolBarWidget::slot_changeOnetoOneIcon(bool f)
{
    if(f)
    {
        m_one_one->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix2/2-images/qie_41.png)}"
                                 "QPushButton:hover{border-image:url(:/prefix2/2-images/hover_small.png);}"
                                 "QPushButton:pressed{border-image:url(:/prefix2/2-images/pressed_small.png);}");
        m_smallToolBar_p->m_one_one->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix2/2-images/big.png)}"
                                                   "QPushButton:hover{border-image:url(:/prefix2/2-images/s_hover_small.png);}"
                                                   "QPushButton:pressed{border-image:url(:/prefix2/2-images/s_pressed_small.png);}");
	m_smallToolBar_p->m_one_one->setToolTip(tr("全屏显示"));
	m_one_one->setToolTip(tr("适应窗口"));
    }
    else
    {
        m_one_one->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix2/2-images/qie_65.png)}"
                                 "QPushButton:hover{border-image:url(:/prefix2/2-images/hover_small.png);}"
                                 "QPushButton:pressed{border-image:url(:/prefix2/2-images/pressed_small.png);}");
        m_smallToolBar_p->m_one_one->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix2/2-images/big.png)}"
                                                   "QPushButton:hover{border-image:url(:/prefix2/2-images/s_hover_small.png);}"
                                                   "QPushButton:pressed{border-image:url(:/prefix2/2-images/s_pressed_small.png);}");
         m_smallToolBar_p->m_one_one->setToolTip(tr("全屏显示"));
        m_one_one->setToolTip(tr("实际尺寸"));

    }
}

//点击固定工具栏函数
void toolBarWidget::slot_setIsfixed()
{
    if(m_isfixed)
        m_isfixed = false;
    else
        m_isfixed = true;

    setStyleSheetNail();
}

smallToolBarWidget::smallToolBarWidget(QWidget *parent, Qt::WindowFlags f):QFrame(parent, f)
{
    //小工具栏的比大工具栏高8
    setFixedSize(600, SMALLTOOLBARHEIGTH);

    m_picInfo = new toolBarButton(36, 36);
    m_picInfo->setToolTip(tr("图片信息"));
    m_magnifying = new toolBarButton(36, 36);
    m_magnifying->setAutoRepeat(true);
    m_magnifying->setToolTip(tr("放大"));
    m_reduce = new toolBarButton(36, 36);
    m_reduce->setAutoRepeat(true);
    m_reduce->setToolTip(tr("缩小"));
    m_beautify = new toolBarButton(36, 36);
    m_beautify->setToolTip(tr("美化/编辑图片"));
    m_share = new toolBarButton(36, 36);
    m_share->setToolTip(tr("将图片分享到..."));

    m_left = new toolBarButton(44, 36);
    m_left->setAutoRepeat(true);
    m_left->setToolTip(tr("上一张"));
    m_right = new toolBarButton(44, 36);
    m_right->setAutoRepeat(true);
    m_right->setToolTip(tr("下一张"));

    m_one_one = new toolBarButton(36, 36);
    m_one_one->setToolTip(tr("适应窗口"));
    m_rotating = new toolBarButton(36, 36);
    m_rotating->setToolTip(tr("旋转"));
    m_slide = new toolBarButton(36, 36);
    m_slide->setToolTip(tr("幻灯片"));
    m_delete = new toolBarButton(36, 36);
    m_delete->setToolTip(tr("删除"));
    m_file = new toolBarButton(36, 36);
    m_file->setToolTip(tr("打开新的图片"));
    m_nail = new toolBarButton(36, 36);
    m_nail->setToolTip(tr("固定/隐藏"));
    m_thumbnail = new toolBarButton(36 ,36);
    m_thumbnail->setToolTip(tr("缩略图"));


    QHBoxLayout *t_1_hl = new QHBoxLayout;
    t_1_hl->addStretch(1);
    t_1_hl->addWidget(m_picInfo);
    //t_1_hl->addSpacing(30);
    t_1_hl->addWidget(m_magnifying);
    t_1_hl->addWidget(m_reduce);
    t_1_hl->addWidget(m_one_one);
    t_1_hl->addWidget(m_beautify);
//this function will open on future
//    t_1_hl->addWidget(m_share);

    t_1_hl->addWidget(m_left);
    //t_1_hl->addSpacing(1);
    t_1_hl->addWidget(m_right);

    t_1_hl->addWidget(m_rotating);
    t_1_hl->addWidget(m_delete);
    t_1_hl->addWidget(m_slide);
    t_1_hl->addWidget(m_file);
    t_1_hl->addWidget(m_nail);
    //t_1_hl->addSpacing(30);
    t_1_hl->addWidget(m_thumbnail);
    t_1_hl->addStretch(1);
    t_1_hl->setMargin(0);
    t_1_hl->setSpacing(3);

    this->setLayout(t_1_hl);

    setStyleSheet("QFrame{background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgb(100, 100, 100, 150), stop:1 rgb(40, 40, 40, 150));"
                           "border-radius:18px;"
                           "border-width: 1px;"
                           "border-style: outset;"
                           "border-color: rgb(200, 200, 200);}");

    setSmallToolBarStyleSheet();
}

smallToolBarWidget::~smallToolBarWidget()
{

}

void smallToolBarWidget::setSmallToolBarStyleSheet()
{
    m_picInfo->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix2/2-images/s_35.png)}"
                             "QPushButton:hover{border-image:url(:/prefix2/2-images/s_hover_small.png);}"
                             "QPushButton:pressed{border-image:url(:/prefix2/2-images/s_pressed_small.png);}");

    m_magnifying->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix2/2-images/s_37.png)}"
                                "QPushButton:hover{border-image:url(:/prefix2/2-images/s_hover_small.png);}"
                                "QPushButton:pressed{border-image:url(:/prefix2/2-images/s_pressed_small.png);}");

    m_reduce->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix2/2-images/s_39.png)}"
                            "QPushButton:hover{border-image:url(:/prefix2/2-images/s_hover_small.png);}"
                            "QPushButton:pressed{border-image:url(:/prefix2/2-images/s_pressed_small.png);}");

    m_one_one->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix2/2-images/s_41.png)}"
                             "QPushButton:hover{border-image:url(:/prefix2/2-images/s_hover_small.png);}"
                             "QPushButton:pressed{border-image:url(:/prefix2/2-images/s_pressed_small.png);}");

    m_beautify->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix2/2-images/s_45.png)}"
                              "QPushButton:hover{border-image:url(:/prefix2/2-images/s_hover_small.png);}"
                              "QPushButton:pressed{border-image:url(:/prefix2/2-images/s_pressed_small.png);}");

    m_share->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix2/2-images/s_43.png)}"
                           "QPushButton:hover{border-image:url(:/prefix2/2-images/s_hover_small.png);}"
                           "QPushButton:pressed{border-image:url(:/prefix2/2-images/s_pressed_small.png);}");

    m_left->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix2/2-images/s_49.png)}"
                          "QPushButton:hover{border-image:url(:/prefix2/2-images/s_hover_big.png);}"
                          "QPushButton:pressed{border-image:url(:/prefix2/2-images/s_pressed_big.png);}");

    m_right->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix2/2-images/s_51.png)}"
                           "QPushButton:hover{border-image:url(:/prefix2/2-images/s_hover_small.png);}"
                           "QPushButton:pressed{border-image:url(:/prefix2/2-images/s_pressed_small.png);}");

    m_rotating->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix2/2-images/s_55.png)}"
                              "QPushButton:hover{border-image:url(:/prefix2/2-images/s_hover_small.png);}"
                              "QPushButton:pressed{border-image:url(:/prefix2/2-images/s_pressed_small.png);}");

    m_delete->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix2/2-images/s_57.png)}"
                            "QPushButton:hover{border-image:url(:/prefix2/2-images/s_hover_small.png);}"
                            "QPushButton:pressed{border-image:url(:/prefix2/2-images/s_pressed_small.png);}");

    m_slide->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix2/2-images/s_59.png)}"
                           "QPushButton:hover{border-image:url(:/prefix2/2-images/s_hover_small.png);}"
                           "QPushButton:pressed{border-image:url(:/prefix2/2-images/s_pressed_small.png);}");

    m_file->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix2/2-images/s_61.png)}"
                          "QPushButton:hover{border-image:url(:/prefix2/2-images/s_hover_small.png);}"
                          "QPushButton:pressed{border-image:url(:/prefix2/2-images/s_pressed_small.png);}");



    m_thumbnail->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix2/2-images/s_47.png)}"
                                       "QPushButton:hover{border-image:url(:/prefix2/2-images/s_hover_small.png);}"
                                       "QPushButton:pressed{border-image:url(:/prefix2/2-images/s_pressed_small.png);}");
}

//渐隐渐显 不断调用此函数
void smallToolBarWidget::paintEvent(QPaintEvent *e)
{
    QFrame::paintEvent(e);
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

}
