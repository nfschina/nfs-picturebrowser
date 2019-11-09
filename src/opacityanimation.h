#ifndef OPACITYANIMATION_H
#define OPACITYANIMATION_H

#include <QObject>
#include <QGraphicsOpacityEffect>
#include <QWidget>
#include <QTimer>

class opacityAnimation : public QObject
{
    Q_OBJECT

public:
    opacityAnimation(QWidget *p, QObject *parent=0);
    ~opacityAnimation();


public slots:
    void slot_showAnimation();
    void slot_hideAnimation();
    void slot_increaseTransparency();
    void slot_reduceTransparency();

private:
    QWidget *pWidget;
    qreal m_transparentValue;
    QTimer *m_1_timer;
    QTimer *m_2_timer;
    QGraphicsOpacityEffect *m_effect;
};

#endif // OPACITYANIMATION_H
