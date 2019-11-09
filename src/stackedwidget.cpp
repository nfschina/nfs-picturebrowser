#include "stackedwidget.h"

stackedWidget::stackedWidget(QWidget *parent)
{
    //setAutoFillBackground(true);
    //setPalette(QPalette(QColor(255, 255, 255, 0))); //不同于QColor(0, 0, 0, 0)
    setAttribute(Qt::WA_TranslucentBackground, true);

    m_1_widget = new mainWidget_1;
    this->addWidget(m_1_widget);

    m_2_widget = new mainWidget_2;
    this->addWidget(m_2_widget);

    connect(m_1_widget,SIGNAL(sig_openFile(QString)), this, SLOT(slot_changeMainWidget(QString)));
    connect(m_1_widget, SIGNAL(sig_openFile(QString)), m_2_widget->m_mainTaskWidget, SLOT(slot_receivePicFile(QString)));

    connect(m_2_widget->m_mainTaskWidget, SIGNAL(sig_changePage()),
            this, SLOT(slot_setCurrentPageto1()));
}

stackedWidget::~stackedWidget()
{

}

void stackedWidget::slot_changeMainWidget(const QString &filename)
{
    Q_UNUSED(filename);
    setCurrentWidget(m_2_widget);  
    m_appTitleBar->fullScreenBtn->show();  //打开文件页不显示 fullScreenBtn
}

void stackedWidget::slot_setCurrentPageto1()
{
    setCurrentWidget(m_1_widget);
    m_appTitleBar->fullScreenBtn->hide(); //显示图片页显示 fullScreenBtn
}


