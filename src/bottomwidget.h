#ifndef TOOLBARWIDGET_H
#define TOOLBARWIDGET_H

#include <QWidget>
#include "common.h"
#include "define.h"
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QStackedWidget>
#include "smallpictureview.h"

class toolBarButton : public QPushButton
{
    Q_OBJECT
public:
    toolBarButton(int w=32, int h=32, QWidget *parent=0);
    ~toolBarButton();

};

class toolBarWidget;

class smallToolBarWidget : public QFrame
{
    Q_OBJECT
public:

    toolBarButton *m_picInfo;
    toolBarButton *m_magnifying;
    toolBarButton *m_reduce;
    toolBarButton *m_one_one;
    toolBarButton *m_beautify;
    toolBarButton *m_share;

    QPushButton *m_left;
    QPushButton *m_right;

    toolBarButton *m_rotating;
    toolBarButton *m_slide;
    toolBarButton *m_delete;
    toolBarButton *m_file;
    toolBarButton *m_nail;
    toolBarButton *m_thumbnail;

    //toolBarWidget *m_toolBar;


    smallToolBarWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~smallToolBarWidget();

    void setStyleSheetNail();
    void setSmallToolBarStyleSheet();

    void paintEvent(QPaintEvent *e);
};

class toolBarWidget : public QWidget
{
    Q_OBJECT
public:
    bool m_isfixed;
    smallToolBarWidget *m_smallToolBar_p;

    toolBarButton *m_picInfo;
    toolBarButton *m_magnifying;
    toolBarButton *m_reduce;
    toolBarButton *m_one_one;
    toolBarButton *m_beautify;
    toolBarButton *m_share;

    toolBarButton *m_left;
    toolBarButton *m_right;

    toolBarButton *m_rotating;
    toolBarButton *m_slide;
    toolBarButton *m_delete;
    toolBarButton *m_file;
    toolBarButton *m_nail;
    toolBarButton *m_thumbnail;

    explicit toolBarWidget(QWidget *parent = 0);
    ~toolBarWidget();

    void toolBarSetStyleSheet();
    void setStyleSheetNail();
    void paintEvent(QPaintEvent *e);

public slots:
    void slot_changeOnetoOneIcon(bool f);
    void slot_setIsfixed();
};

class bottomWidgt : public QWidget
{
    Q_OBJECT
public:

    qreal m_transparentValue;
    QTimer *m_1_timer;
    QTimer *m_2_timer;
    QGraphicsOpacityEffect *m_effect;

    smallPictureView *m_thumbnailView;
    toolBarWidget * m_toolbar;
    smallToolBarWidget *m_smallToolBar;
    QVBoxLayout *m_1_vl;

    bottomWidgt(QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~bottomWidgt();

    void mouseDoubleClickEvent(QMouseEvent *e);

    void changeToolBar();
    void adjustSize(int , int );
signals:
    void sig_needSetItemPos();

public slots:
    void slot_smallPictureViewShowOrHide();
    void slot_showAnimation();//显示动画
    void slot_hideAnimation();//隐藏动画
    void slot_increaseTransparency(); //增加透明度???
    void slot_reduceTransparency();   //减少透明度????
    void slot_statTimer2();

private:
    bool m_isShowed;         //显示标志
};

#endif // TOOLBARWIDGET_H
