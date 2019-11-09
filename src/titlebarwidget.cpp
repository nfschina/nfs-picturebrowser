#include "titlebarwidget.h"

extern QString configList[ROWS];

titleBarWidget::titleBarWidget(QWidget *parent) : QWidget(parent)
{
    this->setFixedHeight(28);

     m_moveWidget = new moveWidget(this);

    fullScreenBtn = new QPushButton;
    fullScreenBtn->setFixedSize(28,28);
    fullScreenBtn->setFocusPolicy(Qt::NoFocus);
    fullScreenBtn->setToolTip(tr("全屏"));

    minBtn = new QPushButton;
    minBtn->setFixedSize(28,28);
    minBtn->setFocusPolicy(Qt::NoFocus);
    minBtn->setToolTip(tr("最小化"));

    maxBtn = new QPushButton;
    maxBtn->setFixedSize(28,28);
    maxBtn->setFocusPolicy(Qt::NoFocus);
    maxBtn->setToolTip(tr("最大化"));

    closeBtn = new QPushButton;
    closeBtn->setFixedSize(28,28);
    closeBtn->setFocusPolicy(Qt::NoFocus);
    closeBtn->setToolTip(tr("关闭"));

    titleBarSetStyleSheet();

    QHBoxLayout *HLayout = new QHBoxLayout;
    HLayout->setSpacing(0);
    HLayout->setMargin(0);

    QVBoxLayout *t_vl = new QVBoxLayout;
    t_vl->setMargin(0);
    t_vl->addWidget(m_moveWidget);

    HLayout->addWidget(m_moveWidget);
    HLayout->addWidget(fullScreenBtn);
    HLayout->addWidget(minBtn);
    HLayout->addWidget(maxBtn);
    HLayout->addWidget(closeBtn);

    fullScreenBtn->hide();
    setLayout(HLayout);
    this->hide();

}

titleBarWidget::~titleBarWidget()
{

}

 void titleBarWidget::slot_showTitlePicName(const QFileInfo &pictureName, const QStringList &str)
 {
      Q_UNUSED(str);

      this->titleName = pictureName.fileName();
      if(configList[0].at(0) == QChar('1'))
        m_moveWidget->m_filename->setText(pictureName.fileName());
      else
          m_moveWidget->m_filename->setText("");

 }


void titleBarWidget:: mouseDoubleClickEvent(QMouseEvent *event)
{    }



void titleBarWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        dragPosition = e->globalPos() - parentWidget()->frameGeometry().topLeft();
        e->accept();
    }
}

void titleBarWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton)
    {
        parentWidget()->move(e->globalPos() - dragPosition);
        e->accept();
    }
}

void titleBarWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#6699FF"));
    painter.drawRect(this->rect());
}

void titleBarWidget::titleBarSetStyleSheet()
{
    fullScreenBtn->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix1/1-images/btn_fullscreen_namal.png);}"
                                 "QPushButton:hover{background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 0, 255, 70),stop:1 rgba(0, 0, 255, 70));}"
                                 "QPushButton:pressed{background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 0, 255, 200),stop:1 rgba(0, 0, 255, 200));}");

    minBtn->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix1/1-images/btn_min_namal.png);}"
                          "QPushButton:hover{background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 0, 255, 70),stop:1 rgba(0, 0, 255, 70));}"
                          "QPushButton:pressed{background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 0, 255, 200),stop:1 rgba(0, 0, 255, 200));}");


    closeBtn->setStyleSheet("QPushButton{border:none; image:url(:/prefix1/1-images/btn_close_nomal.png);}"
                            "QPushButton:hover{background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgb(255, 120, 120), stop:1 rgb(255, 120, 120));}"
                            "QPushButton:pressed{background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgb(255, 80, 80), stop:1 rgb(255, 80, 80));}");

    maxBtn->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix1/1-images/btn_max_namal.png);}"
                          "QPushButton:hover{background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 0, 255, 70),stop:1 rgba(0, 0, 255, 70));}"
                          "QPushButton:pressed{background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 0, 255, 200),stop:1 rgba(0, 0, 255, 200));}");
}

void titleBarWidget::maxBtnSetStyleSheet(bool flag)
{
    if(flag)
        maxBtn->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix1/1-images/btn_max_normal.png);}"
                              "QPushButton:hover{background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 0, 255, 70),stop:1 rgba(0, 0, 255, 70));}"
                              "QPushButton:pressed{background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 0, 255, 200),stop:1 rgba(0, 0, 255, 200));}");
    else
        maxBtn->setStyleSheet("QPushButton{background: transparent; border:none; image:url(:/prefix1/1-images/recovery_normal.png);}"
                              "QPushButton:hover{background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 0, 255, 70),stop:1 rgba(0, 0, 255, 70));}"
                              "QPushButton:pressed{background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 0, 255, 200),stop:1 rgba(0, 0, 255, 200));}");
}

moveWidget::moveWidget(QWidget *parent) : QWidget(parent)
{
   setMouseTracking(true);
    m_isAltDown = false;
    m_iconLabel = new QLabel;
    m_iconLabel->setPixmap(QPixmap(":/prefix1/1-images/logo.svg"));
    m_iconLabel->adjustSize();
    m_iconLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    m_picTitleLabel=new QLabel;
    m_picTitleLabel->setText(tr("看图工具"));
    m_picTitleLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_picTitleLabel->setStyleSheet("color:white; font:14px;");

    m_filename = new QLabel;
    m_filename->adjustSize();
    m_filename->setStyleSheet("color:white; font:14px;");

    QHBoxLayout *t_hl = new QHBoxLayout;
    t_hl->setSpacing(5);
    t_hl->setContentsMargins(6, 0, 0, 0);

    t_hl->addWidget(m_iconLabel,0, Qt::AlignVCenter);
    t_hl->addWidget(m_picTitleLabel, 0, Qt::AlignVCenter);
    t_hl->addWidget(m_filename, 0, Qt::AlignCenter);

    setLayout(t_hl);

}

void moveWidget::slot_releaseAlt()
{
    if(!this->rect().contains(this->mapFromGlobal(QCursor::pos())))
    {
        m_1_timer->stop();
        QString command = "xdotool keyup Alt";
        m_moveProcess->start(command);
        m_2_timer->start(50);

        m_isAltDown = false;
    }
}

void moveWidget::slot_pressAlt()
{
    if(this->rect().contains(this->mapFromGlobal(QCursor::pos())))
    {
        if(!this->isActiveWindow())
                return;

        m_2_timer->stop();
        QString command = "xdotool keydown Alt";
        m_moveProcess->start(command);
        m_1_timer->start(50);
    }
}
moveWidget::~moveWidget()
{

}
void moveWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
   emit  sig_moveDoubleClick();
}
