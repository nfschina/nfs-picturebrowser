#ifndef MAINSTACKEDWIDGET_H
#define MAINSTACKEDWIDGET_H

#include <QStackedWidget>
#include "mainwidget_1.h"
#include "mainwidget_2.h"
#include "titlebarwidget.h"

class stackedWidget : public QStackedWidget
{
    Q_OBJECT

public:

    mainWidget_1 *m_1_widget;
    mainWidget_2 *m_2_widget;

    titleBarWidget *m_appTitleBar;

    stackedWidget(QWidget *parent=0);
    ~stackedWidget();

public slots:
    void slot_changeMainWidget(const QString &);
    void slot_setCurrentPageto1();

};

#endif // MAINSTACKEDWIDGET_H
