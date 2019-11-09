#ifndef TITLEBARWIDGET_H
#define TITLEBARWIDGET_H

#include <QWidget>
#include "common.h"
#include "define.h"

//stackedWidget 和 titleBarWidget 两个类相互包含
class stackedWidget;

class moveWidget : public QWidget
{
    Q_OBJECT
public:
    explicit moveWidget(QWidget *parent = 0);
    ~moveWidget();
    QProcess *m_moveProcess;
    QLabel *m_iconLabel;
    QLabel *m_filename;
    QLabel *m_picTitleLabel;
    QTimer *m_1_timer;
    QTimer *m_2_timer;

    bool m_isAltDown;


protected:

    void mouseDoubleClickEvent(QMouseEvent *event);

public slots:
    void slot_releaseAlt();
    void slot_pressAlt();

signals:
    void sig_moveDoubleClick();
};

class titleBarWidget : public QWidget
{
    Q_OBJECT

public:
    moveWidget *m_moveWidget;
    QPushButton *fullScreenBtn;
    QPushButton *minBtn;
    QPushButton *maxBtn;
    QPushButton *closeBtn;
    QPoint dragPosition;
    QString titleName;


    explicit titleBarWidget(QWidget *parent = 0);
    ~titleBarWidget();

    void titleBarSetStyleSheet();
    void maxBtnSetStyleSheet(bool );


    void paintEvent(QPaintEvent *e);

     QPoint mdragPosition;    //记录鼠标按下时 坐标
    void mousePressEvent(QMouseEvent *e);         //记录点击的点
    void mouseMoveEvent(QMouseEvent *e);          //移动窗口

signals:
    void sig_NormalOrMaximized();


public slots:
    void slot_showTitlePicName(const QFileInfo &,const QStringList &);


protected:
    void mouseDoubleClickEvent(QMouseEvent *event);
};

#endif // TITLEBARWIDGET_H
