#ifndef MAINWIDGET_2_H
#define MAINWIDGET_2_H

#include <QWidget>
#include "common.h"
#include "maintaskwidget.h"
#include "picinfowidget.h"

class mainWidget_2 : public QWidget
{
    Q_OBJECT
public:

    QHBoxLayout *t_1_hl;
    QVBoxLayout *t_1_vl;

    mainTaskWidget *m_mainTaskWidget;


    explicit mainWidget_2(QWidget *parent = 0);
    void paintEvent(QPaintEvent *e);
signals:

public slots:
};

#endif // MAINWIDGET_2_H
