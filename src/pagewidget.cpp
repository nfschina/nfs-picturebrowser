#include"pagewidget.h"

normalWidget::normalWidget (QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f)
{
    QFont ft;
    ft.setBold(true);
    ft.setPointSize(10.5);
    setStyleSheet("color: grey");

    QLabel *t_1_lb = new QLabel(tr("常规设置"));
    t_1_lb->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    t_1_lb->setFont(ft);


    m_1_cb = new checkBox(tr("标题栏显示图片名称"));
    connect(m_1_cb,SIGNAL(clicked()),this,SLOT(slot_normalSetting1_cb()));
    m_2_cb = new checkBox(tr("工具栏默认不隐藏"));
    connect(m_2_cb,SIGNAL(clicked()),this,SLOT(slot_normalSetting2_cb()));
    m_3_cb = new checkBox(tr("左右翻页箭头不隐藏"));
    connect(m_3_cb,SIGNAL(clicked()),this,SLOT(slot_normalSetting3_cb()));
    m_4_cb = new checkBox(tr("删除图片时弹出提示框"));
    connect(m_4_cb,SIGNAL(clicked()),this,SLOT(slot_normalSetting4_cb()));
    m_5_cb = new checkBox(tr("默认打开缩略图"));
    connect(m_5_cb,SIGNAL(clicked()),this,SLOT(slot_normalSetting5_cb()));


    QLabel *t_2_lb = new QLabel(tr("全屏透明"));
    t_2_lb->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    t_2_lb->setFont(ft);

    m_7_cb = new radioButton(tr("透明"));
    m_8_cb = new radioButton(tr("不透明"));
    mCbBtnGroup=new QButtonGroup(this);
    mCbBtnGroup->addButton(m_7_cb,1);
    mCbBtnGroup->addButton(m_8_cb,0);



    QFrame* t_line1 = new QFrame(this);
    t_line1->setFixedWidth(215);
    t_line1->setFrameShape(QFrame::HLine);
    t_line1->setFrameShadow(QFrame::Sunken);
    t_line1->move(103, 14);

    QFrame* t_line2 = new QFrame(this);
    t_line2->setFixedWidth(215);
    t_line2->setFrameShape(QFrame::HLine);
    t_line2->setFrameShadow(QFrame::Sunken);
    t_line2->move(103, 165);

    QHBoxLayout *t_1_hl = new QHBoxLayout;
    t_1_hl->setSpacing(20);
    t_1_hl->setMargin(0);
    t_1_hl->addWidget(m_7_cb);
    t_1_hl->addWidget(m_8_cb);
    t_1_hl->addStretch();

    QVBoxLayout *t_1_vl = new QVBoxLayout;
    t_1_vl->setContentsMargins(42, 20, 42, 20);
    t_1_vl->setSpacing(2);

    t_1_vl->addWidget(t_1_lb);
    t_1_vl->addWidget(m_1_cb);
    t_1_vl->addWidget(m_2_cb);
    t_1_vl->addWidget(m_3_cb);
    t_1_vl->addWidget(m_4_cb);
    t_1_vl->addWidget(m_5_cb);
    t_1_vl->addStretch(5);
    //t_1_vl->addWidget(m_7_cb);
    t_1_vl->addWidget(t_2_lb);
    t_1_vl->addLayout(t_1_hl);

    this->setLayout(t_1_vl);

    setNormalWidetConfig(configList[0]);
}

normalWidget::~normalWidget(){}
void normalWidget::slot_test()
{
    qDebug()<<"slot test:"<<m_1_cb->isChecked();
}


void normalWidget::slot_normalSetting1_cb()
{
    this->sig_normalSetting1(m_1_cb->isChecked());
}
void normalWidget::slot_normalSetting2_cb()
{
    this->sig_normalSetting2(m_2_cb->isChecked());
}
void normalWidget::slot_normalSetting3_cb()
{
    this->sig_normalSetting3(m_3_cb->isChecked());
}
void normalWidget::slot_normalSetting4_cb()
{
    this->sig_normalSetting4(m_4_cb->isChecked());
}
void normalWidget::slot_normalSetting5_cb()
{
    this->sig_normalSetting5(m_5_cb->isChecked());
}



void normalWidget::setNormalWidetConfig(const QString &config)
{

    if(config.at(0) == QChar('1'))
        m_1_cb->setChecked(true);
    else
        m_1_cb->setChecked(false);

    if(config.at(1) == QChar('1'))
        m_2_cb->setChecked(true);
    else
        m_2_cb->setChecked(false);

    if(config.at(2) == QChar('1'))
        m_3_cb->setChecked(true);
    else
        m_3_cb->setChecked(false);

    if(config.at(3) == QChar('1'))
        m_4_cb->setChecked(true);
    else
        m_4_cb->setChecked(false);

    if(config.at(4) == QChar('1'))
        m_5_cb->setChecked(true);
    else
        m_5_cb->setChecked(false);

    if(config.at(5) == QChar('1'))
        m_7_cb->setChecked(true);
    else
        m_8_cb->setChecked(true);
}


//生成中间变量 保存t_configList 返回
const QString normalWidget::getNormalWidgetConfig()
{
    QString t_configList;

    if(m_1_cb->isChecked())
        t_configList.append("1");
    else
        t_configList.append("0");

    if(m_2_cb->isChecked())
        t_configList.append("1");
    else
        t_configList.append("0");

    if(m_3_cb->isChecked())
        t_configList.append("1");
    else
        t_configList.append("0");

    if(m_4_cb->isChecked())
        t_configList.append("1");
    else
        t_configList.append("0");

    if(m_5_cb->isChecked())
        t_configList.append("1");
    else
        t_configList.append("0");

    if(m_7_cb->isChecked())
        t_configList.append("1");
    else
        t_configList.append("0");

    return t_configList;
}




habitWidget::habitWidget (QWidget* parent , Qt::WindowFlags f)
    : QWidget(parent, f)
{
    QFont ft;
    ft.setBold(true);
    ft.setPointSize(10.5);
    setStyleSheet("color: grey");

    QLabel *t_1_lb = new QLabel(tr("是否保存窗口大小"));
    t_1_lb->setFont(ft);

    m_1_rb = new radioButton(tr("是"));
    m_2_rb = new radioButton(tr("否"));

    QHBoxLayout *t_1_hl = new QHBoxLayout;
    t_1_hl->setSpacing(20);
    t_1_hl->addWidget(m_1_rb);
    t_1_hl->addWidget(m_2_rb);
    t_1_hl->addStretch();

    QFrame* t_line1 = new QFrame(this);
    t_line1->setFixedWidth(215);
    t_line1->setFrameShape(QFrame::HLine);
    t_line1->setFrameShadow(QFrame::Sunken);
    t_line1->move(153, 14);
    t_line1->setFixedWidth(175);

    QVBoxLayout *t_1_vl = new QVBoxLayout;
    t_1_vl->setContentsMargins(42, 20, 42, 20);
    t_1_vl->addWidget(t_1_lb);
    t_1_vl->addLayout(t_1_hl);
    t_1_vl->addStretch();

    setLayout(t_1_vl);

    sethabitWidgetConfig(configList[1]);
}

void habitWidget::sethabitWidgetConfig(const QString &config)
{
    if(config.at(0) == QChar('1'))
        m_1_rb->setChecked(true);
    else
        m_2_rb->setChecked(true);
}
const QString habitWidget::gethabitWidgetConfig()
{
    QString t_configList;

    if(m_1_rb->isChecked())
        t_configList.append("1");
    else
        t_configList.append("0");

    return t_configList;
}
habitWidget::~habitWidget()
{}


slideWidget::slideWidget (QWidget* parent, Qt::WindowFlags f)  : QWidget(parent, f)
{
    setStyleSheet("color: grey");

    QFont ft;
    ft.setBold(true);
    ft.setPointSize(10.5);

    QLabel *t_1_lb = new QLabel(tr("默认播放顺序"));
    t_1_lb->setFont(ft);

    m_1_rb = new radioButton(tr("循环播放"));
    m_2_rb = new radioButton(tr("随机播放"));


    QHBoxLayout *t_1_hl = new QHBoxLayout;
    t_1_hl->setSpacing(20);
    t_1_hl->addWidget(m_1_rb);
    t_1_hl->addWidget(m_2_rb);
    t_1_hl->addStretch();

    QLabel *t_2_lb = new QLabel(tr("默认播放速度"));
    t_2_lb->setFont(ft);

    QLabel *t_lb1 = new QLabel(tr("播放间隔"));

    m_spinbox = new QSpinBox;
    m_spinbox->setRange(1, 9);
    m_spinbox->setFocusPolicy(Qt::NoFocus);
    //m_spinbox->setFixedSize(50, 30);
    m_spinbox->setFixedWidth(50);
    QLabel *t_lb2 = new QLabel(tr("(范围在1～9秒)"));

    QHBoxLayout *t_2_hl = new QHBoxLayout;
    t_2_hl->setSpacing(5);
    t_2_hl->addWidget(t_lb1);
    t_2_hl->addWidget(m_spinbox);
    t_2_hl->addWidget(t_lb2);
    t_2_hl->addStretch();

    QLabel *t_3_lb = new QLabel(tr("默认播放效果"));
    t_3_lb->setFont(ft);

    m_1_comboBox = new QComboBox;
    m_1_comboBox->setFixedWidth(100);
    m_1_comboBox->setFocusPolicy(Qt::NoFocus);
    m_1_comboBox->setView(new QListView);     //可调节item高度

    setComboboxStyle(m_1_comboBox);

    m_1_comboBox->addItem(tr("无效果"));
    m_1_comboBox->addItem(tr("快速推入"));
    m_1_comboBox->addItem(tr("淡入淡出"));
    m_1_comboBox->addItem(tr("扇形飞入"));
    m_1_comboBox->addItem(tr("随机效果"));

    QLabel *t_4_lb = new QLabel(tr("默认背景"));
    t_4_lb->setFont(ft);

    m_2_comboBox = new QComboBox;
    m_2_comboBox->setFixedWidth(100);
    m_2_comboBox->setFocusPolicy(Qt::NoFocus);
    m_2_comboBox->setView(new QListView);
    m_2_comboBox->addItem(tr("黑色背景"));
    m_2_comboBox->addItem(tr("花纹背景"));

    setComboboxStyle(m_2_comboBox);

    QVBoxLayout *t_1_vl = new QVBoxLayout;
    t_1_vl->setContentsMargins(42, 20, 42, 20);
    t_1_vl->addWidget(t_1_lb);
    t_1_vl->addLayout(t_1_hl);
    t_1_vl->addWidget(t_2_lb);
    t_1_vl->addLayout(t_2_hl);
    t_1_vl->addWidget(t_3_lb);
    t_1_vl->addWidget(m_1_comboBox);
    t_1_vl->addWidget(t_4_lb);
    t_1_vl->addWidget(m_2_comboBox);
//        t_1_vl->addLayout(t_2_hl);
    t_1_vl->addStretch();

    this->setLayout(t_1_vl);


    QFrame* t_line1 = new QFrame(this);
    t_line1->setFrameShape(QFrame::HLine);
    t_line1->setFrameShadow(QFrame::Sunken);
    t_line1->move(128, 14);
    t_line1->setFixedWidth(195);

    QFrame* t_line2 = new QFrame(this);
    t_line2->setFrameShape(QFrame::HLine);
    t_line2->setFrameShadow(QFrame::Sunken);
    t_line2->move(128, 62);
    t_line2->setFixedWidth(195);

    QFrame* t_line3 = new QFrame(this);
    t_line3->setFixedWidth(195);
    t_line3->setFrameShape(QFrame::HLine);
    t_line3->setFrameShadow(QFrame::Sunken);
    t_line3->move(128, 110);

    QFrame* t_line4 = new QFrame(this);
    t_line4->setFixedWidth(195);
    t_line4->setFrameShape(QFrame::HLine);
    t_line4->setFrameShadow(QFrame::Sunken);
    t_line4->move(128, 160);

    setslideWidgetConfig(configList[4], configList[5]);
}

slideWidget::~slideWidget(){}

void slideWidget::setslideWidgetConfig(const QString &config1, const QString &config2)
{
    if(config1.at(0) == QChar('1'))
        m_1_rb->setChecked(true);
    else
        m_2_rb->setChecked(true);

    if(config2.length() == 3)
    {
        m_spinbox->setValue(config2.at(0).toLatin1() - '0');
        m_1_comboBox->setCurrentIndex(config2.at(1).toLatin1()- '0');
        m_2_comboBox->setCurrentIndex(config2.at(2).toLatin1()- '0');
    }
    else
    {
        m_spinbox->setValue(config2.mid(0, 2).toInt());
        m_1_comboBox->setCurrentIndex(config2.at(2).toLatin1()- '0');
        m_2_comboBox->setCurrentIndex(config2.at(3).toLatin1()- '0');
    }
}



const QString slideWidget::getslideWidgetConfig()
{
    QString t_configList;

    if(m_1_rb->isChecked())
        t_configList.append("1");
    else
        t_configList.append("0");

    return t_configList;
}

const QString slideWidget::getValue()
{
    QString t_configList;

    t_configList = QString::number(m_spinbox->value()) +
            QString::number(m_1_comboBox->currentIndex()) +
            QString::number((m_2_comboBox->currentIndex()));

    return t_configList;
}


void slideWidget::setComboboxStyle(QComboBox *comboBox)
{
    comboBox->setStyleSheet("QComboBox{"
                            "font:13px;"
                            "border-radius: 0px;"
                            "padding: 1px 1px 1px 1px;"    //上右下左
                            "border: 1px solid grey;}"
                            "QComboBox::drop-down{"                   //
                            "subcontrol-origin: padding;"
                            "subcontrol-position: top right;"
                            "width: 15px; "
                            "border-left-width: 1px;"
                            "border-left-style: solid;"
                            "border-left-color: grey;}"
                            "QComboBox::down-arrow{"
                            "image: url(:/prefix1/1-images/array_down.png);}"
                            "QAbstractItemView::item{height:23px;}");
}



helpWidget::helpWidget (QWidget* parent, Qt::WindowFlags f)  : QWidget(parent, f)
{
    setStyleSheet("color: grey");
    QLabel *appLogo = new QLabel(this);
    appLogo->adjustSize();
    QPixmap logo = QPixmap(tr(":/prefix1/1-images/LOGO.png")).copy(QRect(0, 0, 98, 90));
    appLogo->setPixmap(logo);


    QLabel *appName = new QLabel(tr("看图工具"));
    appName->setStyleSheet("font-size: 15px;");
    appName->adjustSize();

    QLabel *appVersion = new QLabel;
    appVersion->adjustSize();

    QFile file(":/data/data/version");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        appVersion->setText(tr("version: 未知"));
    else
    {
        QTextStream out(&file);
        QString t_text = out.readAll();
        file.close();
        appVersion->setText(t_text);
    }

    QLabel *appDescriptions = new QLabel;
    appDescriptions->adjustSize();
    appDescriptions->setText(tr("    看图工具是一款简洁的图片查看器， 设计宗旨是：美观，\n易用和快捷。"));

    QVBoxLayout *vl = new QVBoxLayout;
    vl->setContentsMargins(10, 10, 10, 10);
    vl->setSpacing(10);
    vl->addStretch(1);
    vl->addWidget(appLogo, 0, Qt::AlignHCenter);
    vl->addWidget(appName, 0, Qt::AlignHCenter);
    vl->addWidget(appVersion, 0, Qt::AlignHCenter);
    vl->addWidget(appDescriptions, 0, Qt::AlignHCenter);
    vl->addStretch(2);

    setLayout(vl);
}

helpWidget::~helpWidget(){}
