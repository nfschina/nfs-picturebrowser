#include "mainwidget_2.h"

mainWidget_2::mainWidget_2(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setFocusPolicy(Qt::NoFocus);

    m_mainTaskWidget = new mainTaskWidget;


    t_1_hl = new QHBoxLayout;

    t_1_hl->addWidget(m_mainTaskWidget);
    t_1_hl->setMargin(0);
    t_1_hl->setSpacing(0);
    this->setLayout(t_1_hl);
}


void mainWidget_2::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
}





