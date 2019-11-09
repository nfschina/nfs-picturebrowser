#include "setupdialog.h"
#include <QDebug>

extern QString configList[ROWS];

setUpDialog::setUpDialog(QWidget *parent)
    : QDialog(parent)
{
    setFixedSize(500, 300);
    setAttribute(Qt::WA_DeleteOnClose);  //关闭时释放
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAutoFillBackground(true);
    setPalette(QPalette(Qt::white));

    recovery_label = new QLabel(tr("已恢复默认设置"));
    recovery_label->setFrameStyle(QFrame::Raised);
    recovery_label->setStyleSheet("background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgb(200, 200, 200, 100), stop:1 rgb(20, 20, 20, 100));"
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
    recovery_label->setGraphicsEffect(shadow_effect);

    recovery_label->setAlignment(Qt::AlignCenter);
    recovery_label->setFixedSize(150, 32);

    QWidget *recoveryWidget = new QWidget;
    recoveryWidget->setAttribute(Qt::WA_TransparentForMouseEvents);//子窗口鼠标交给父窗口处理
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(recovery_label, 0, Qt::AlignCenter);
    recoveryWidget->setLayout(layout);

    titleBarForSetupDialog *t_titleBar = new titleBarForSetupDialog;

    itemButton *t_1_item = new itemButton(QPixmap(":/prefix3/3-images/常规设置.png"),
                                          QPixmap(":/prefix3/3-images/常规设置-当前.png"),
                                          QString("常规设置"), true, 0);
    itemButton *t_2_item = new itemButton(QPixmap(":/prefix3/3-images/习惯设置.png"),
                                          QPixmap(":/prefix3/3-images/习惯设置-当前.png"),
                                          QString("习惯设置"), false, 1);
    itemButton *t_3_item = new itemButton(QPixmap(":/prefix3/3-images/幻灯片.png"),
                                          QPixmap(":/prefix3/3-images/幻灯片-当前.png"),
                                          QString("幻灯片"), false, 2);
    itemButton *t_4_item = new itemButton(QPixmap(":/prefix3/3-images/帮助.png"),
                                          QPixmap(":/prefix3/3-images/帮助-当前.png"),
                                          QString("关于"), false, 3);

    connect(t_1_item, SIGNAL(sig_pressed()), t_2_item, SLOT(slot_cancelBackground()));
    connect(t_1_item, SIGNAL(sig_pressed()), t_3_item, SLOT(slot_cancelBackground()));
    connect(t_1_item, SIGNAL(sig_pressed()), t_4_item, SLOT(slot_cancelBackground()));
    connect(t_2_item, SIGNAL(sig_pressed()), t_1_item, SLOT(slot_cancelBackground()));
    connect(t_2_item, SIGNAL(sig_pressed()), t_3_item, SLOT(slot_cancelBackground()));
    connect(t_2_item, SIGNAL(sig_pressed()), t_4_item, SLOT(slot_cancelBackground()));
    connect(t_3_item, SIGNAL(sig_pressed()), t_1_item, SLOT(slot_cancelBackground()));
    connect(t_3_item, SIGNAL(sig_pressed()), t_2_item, SLOT(slot_cancelBackground()));
    connect(t_3_item, SIGNAL(sig_pressed()), t_4_item, SLOT(slot_cancelBackground()));
    connect(t_4_item, SIGNAL(sig_pressed()), t_1_item, SLOT(slot_cancelBackground()));
    connect(t_4_item, SIGNAL(sig_pressed()), t_2_item, SLOT(slot_cancelBackground()));
    connect(t_4_item, SIGNAL(sig_pressed()), t_3_item, SLOT(slot_cancelBackground()));

    QVBoxLayout *t_2_vl = new QVBoxLayout;
    t_2_vl->setContentsMargins(5, 0 ,5 ,0);
    t_2_vl->setSpacing(12);

    t_2_vl->addStretch(4);
    t_2_vl->addWidget(t_1_item);
    t_2_vl->addWidget(t_2_item);
    t_2_vl->addWidget(t_3_item);
    t_2_vl->addWidget(t_4_item);
    t_2_vl->addStretch(10);

    QWidget *t_itemList = new QWidget;
    t_itemList->setAutoFillBackground(true);
    t_itemList->setPalette(QPalette(Qt::white));

    t_itemList->setLayout(t_2_vl);

    m_normalWidget = new normalWidget;
    m_habitWidget = new habitWidget;
    m_slideWidget = new slideWidget;
    helpWidget *t_helpWidget = new helpWidget;

    t_stackedWidget = new QStackedWidget;
    t_stackedWidget->setAutoFillBackground(true);
    t_stackedWidget->setPalette(QPalette(QColor(255, 255, 255)));

    t_stackedWidget->addWidget(m_normalWidget);
    t_stackedWidget->addWidget(m_habitWidget);
    t_stackedWidget->addWidget(m_slideWidget);
    t_stackedWidget->addWidget(t_helpWidget);
    t_stackedWidget->addWidget(recoveryWidget);

    QFrame* t_line1 = new QFrame;
    t_line1->setFrameShape(QFrame::VLine);
    t_line1->setFrameShadow(QFrame::Sunken);
    t_line1->setStyleSheet("color: grey");

    connect(t_1_item, SIGNAL(sig_sendId(int)), t_stackedWidget, SLOT(setCurrentIndex(int)));
    connect(t_2_item, SIGNAL(sig_sendId(int)), t_stackedWidget, SLOT(setCurrentIndex(int)));
    connect(t_3_item, SIGNAL(sig_sendId(int)), t_stackedWidget, SLOT(setCurrentIndex(int)));
    connect(t_4_item, SIGNAL(sig_sendId(int)), t_stackedWidget, SLOT(setCurrentIndex(int)));

    init_normalwidget= m_normalWidget->getNormalWidgetConfig();
qDebug()<<"zlzlzlzllllllllll"<<init_normalwidget;
    QHBoxLayout *t_1_hl = new QHBoxLayout;
    t_1_hl->setMargin(0);
    t_1_hl->setSpacing(0);

    t_1_hl->addWidget(t_itemList);
    t_1_hl->addWidget(t_line1);
    t_1_hl->addWidget(t_stackedWidget);

    QWidget *t_centerWidget = new QWidget;
    t_centerWidget->setLayout(t_1_hl);

    bottomWidget *t_bottomWidget = new bottomWidget;
    connect(t_bottomWidget->m_ok, SIGNAL(clicked()), this, SLOT(slot_confirmConfig()));
    connect(t_bottomWidget->m_cancel, SIGNAL(clicked()) ,this, SLOT(slot_cancel()));
//reset all the software to default
    connect(t_bottomWidget->m_recovery, SIGNAL(clicked()), this, SLOT(slot_recovery()));

    QVBoxLayout *t_1_vl = new QVBoxLayout;
    t_1_vl->setMargin(0);
    t_1_vl->setSpacing(0);

    QFrame* t_line2 = new QFrame;
    t_line2->setFixedWidth(this->width()-2);
    t_line2->setFrameShape(QFrame::HLine);
    t_line2->setFrameShadow(QFrame::Sunken);
    t_line2->setStyleSheet("color: grey");

    t_centerWidget->setContentsMargins(1, 0, 1, 0);
    t_bottomWidget->setContentsMargins(1,0,1,1);

    t_1_vl->addWidget(t_titleBar/*, 0, Qt::AlignTop*/);
    t_1_vl->addWidget(t_centerWidget);
    t_1_vl->addWidget(t_line2, 0, Qt::AlignHCenter);
    t_1_vl->addWidget(t_bottomWidget);

    this->setLayout(t_1_vl);

    connect(t_titleBar->m_close, SIGNAL(clicked()), this, SLOT(slot_cancel()));

}

setUpDialog::~setUpDialog()
{

}

void setUpDialog::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter painter(this);
    painter.setPen(QColor(175, 212, 245));
    painter.drawRect(0, 0, this->width()-1, this->height()-1);
}

void setUpDialog::closeEvent(QCloseEvent *e)
{
    Q_UNUSED(e);
    this->obtainSetInfo();
}


void setUpDialog::obtainSetInfo()
{
    QString slideInfo;
    slideInfo = this->m_slideWidget->getslideWidgetConfig()+
                this->m_slideWidget->getValue();
    sig_sendSlideSetInfo(slideInfo);
}

void setUpDialog::slot_confirmConfig()
{

    configList[0] = m_normalWidget->getNormalWidgetConfig();

    QVariant normal_1=configList[0].mid(0,1);
    QVariant normal_2=configList[0].mid(1,1);
    QVariant normal_3=configList[0].mid(2,1);
    QVariant normal_4=configList[0].mid(3,1);
    QVariant normal_5=configList[0].mid(4,1);

    emit sig_normalSetting1(normal_1.toBool());
    emit sig_normalSetting2(normal_2.toBool());
    emit sig_normalSetting3(normal_3.toBool());
    emit sig_normalSetting4(normal_4.toBool());
    emit sig_normalSetting5(normal_5.toBool());

    configList[1] = m_habitWidget->gethabitWidgetConfig();
    configList[4] = m_slideWidget->getslideWidgetConfig();
    configList[5] = m_slideWidget->getValue();
    close();
    this->obtainSetInfo();

//    supWgt->show();
}

//恢复默认设置
void setUpDialog::slot_recovery()
{
    configList[0] = "110100";
    configList[1] = "1";
    configList[2] = "740";//app width
    configList[3] = "450";//app height
    configList[4] = "1";
    configList[5] = "300";

    m_normalWidget->setNormalWidetConfig(configList[0]);
    m_habitWidget->sethabitWidgetConfig(configList[1]);
    m_slideWidget->setslideWidgetConfig(configList[4], configList[5]);

    int num = t_stackedWidget->currentIndex();
    recovery_label->show();
    t_stackedWidget->setCurrentIndex(4);
    QElapsedTimer* timer = new QElapsedTimer;
    timer->start();
    //等待一秒钟
    while(timer->elapsed()<1000)
        QCoreApplication::processEvents();
    recovery_label->hide();
    t_stackedWidget->setCurrentIndex(num);
}
void setUpDialog::slot_cancel()
{
    QVariant normal_1=init_normalwidget.mid(0,1);
    QVariant normal_2=init_normalwidget.mid(1,1);
    QVariant normal_3=init_normalwidget.mid(2,1);
    QVariant normal_4=init_normalwidget.mid(3,1);
    QVariant normal_5=init_normalwidget.mid(4,1);

    emit sig_normalSetting1(normal_1.toBool());
    emit sig_normalSetting2(normal_2.toBool());
    emit sig_normalSetting3(normal_3.toBool());
    emit sig_normalSetting4(normal_4.toBool());
    emit sig_normalSetting5(normal_5.toBool());

    this->close();
}
