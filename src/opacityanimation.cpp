#include "opacityanimation.h"

opacityAnimation::opacityAnimation(QWidget *p, QObject *parent):QObject(parent)
{
    pWidget = p;
    m_transparentValue = 0;

    m_effect = new QGraphicsOpacityEffect(pWidget);
    m_effect->setOpacity(m_transparentValue);
    p->setGraphicsEffect(m_effect);

    m_1_timer = new QTimer(this);
    connect(m_1_timer, SIGNAL(timeout()), this, SLOT(slot_reduceTransparency()));

    m_2_timer = new QTimer(this);
    connect(m_2_timer, SIGNAL(timeout()), this, SLOT(slot_increaseTransparency()));
}

opacityAnimation::~opacityAnimation()
{
    delete m_effect;
    delete m_1_timer;
    delete m_2_timer;
}

void opacityAnimation::slot_showAnimation()
{
    if(pWidget->isHidden())
    {
        pWidget->show();
        m_2_timer->stop();
        m_1_timer->start(0.05);
    }
}

void opacityAnimation::slot_hideAnimation()
{
    if(!pWidget->isHidden())
    {
        m_1_timer->stop();
        m_2_timer->start(7);
    }
}

void opacityAnimation::slot_increaseTransparency()
{
    if(m_transparentValue >= 0)
    {
        m_transparentValue -= 0.01;
        m_effect->setOpacity(m_transparentValue);
    }
    else
    {
        m_2_timer->stop();
        pWidget->hide();
    }
}

void opacityAnimation::slot_reduceTransparency()
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

