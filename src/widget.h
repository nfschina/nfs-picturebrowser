#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QApplication>
#include "common.h"
#include "define.h"
#include "dragproxy.h"
#include "stackedwidget.h"
#include "titlebarwidget.h"
#include <QSettings>
#include <QDialog>
#define MARGIN 20//四个角的长度

class Application : public QApplication
{
    Q_OBJECT

public:
    Application(int &argc, char **argv) : QApplication(argc, argv)
    {

    }

    bool notify(QObject *obj, QEvent *e)
    {

        if(e->type() == QEvent::MouseButtonPress)
        {
            sig_appMousePressed();
        }
        else if(e->type() == QEvent::MouseButtonDblClick)
            sig_appMouseDBClick();

        return QApplication::notify(obj, e);
    }

signals:
    void sig_appMousePressed();
     void sig_appMouseDBClick();
};

class Widget : public QWidget
{
    Q_OBJECT

public:

    enum NormalOrMaximized
    {
        Normal,
        Maximized
    };

    int deskWidth;
    int deskHeigth;
    QRect deskRect;

    int winWidth;
    int winHeight;

    int winX;
    int winY;

    DragProxy *drag;

    titleBarWidget *m_titleBar;
    stackedWidget *m_stackedWidget;
    NormalOrMaximized f;             //是否是全屏的标志位
    bool m_isFullScreen;             //过滤showfullscreen第一个resizevent用
    QVBoxLayout *vl;
    QMenu *menu;

/*mouse move*/
    QPoint  dragPosition;
    bool                  dragging;
    int countFlag(QPoint p, int row);
    void setCursorType(int flag);
    int countRow(QPoint p);

    void showMaximized();
    void showNormal();

    Widget(QWidget *parent = 0);
    ~Widget();



    void openPicture(const QString &filename);
    void defaultSettingCheck();
    void defaultSettingFunc();
protected:
    void resizeEvent(QResizeEvent *e);
    void paintEvent(QPaintEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void closeEvent(QCloseEvent *e);

/*set mouse move event*/
    void mousePressEvent(QMouseEvent *event);
     void mouseReleaseEvent(QMouseEvent *event);
     void mouseDoubleClickEvent(QMouseEvent *event);
     void mouseMoveEvent(QMouseEvent *event);

signals:
    void sig_changeSizeEnd();
    void sig_pressedUp();
    void sig_pressedDown();
    void sig_pressedLeft();
    void sig_pressedRight();
    void sig_pressedDelete();
    void sig_pressedF5();
    void sig_pressedControlAndO();
    void sig_pressedControlAndS();
    void sig_pressedControlAndL();
    void sig_pressedControlAndC();
    //void sig_pressed
public slots:
    void slot_showMaximized();
    void slot_showFullScreen();
    void slot_picInfoDetail();
    void slot_normalSetting1(bool isChecked);

private:
    bool isLeftPressed;
     int curPos;
     QPoint pLast;
};

#endif // WIDGET_H
