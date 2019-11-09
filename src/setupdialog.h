#ifndef SETUPDIALOG_H
#define SETUPDIALOG_H

#include <QDialog>
#include <QPaintEvent>
#include <QPainter>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QMouseEvent>
#include <QFont>
#include <QListWidget>
#include <QEvent>
#include <qstackedwidget.h>
#include "pagewidget.h"
#include "define.h"
#include "bigpictureview.h"

#include"common.h"

class titleBarForSetupDialog : public QWidget
{
    Q_OBJECT

public:

    QPushButton *m_close;
    QPoint dragPosition;

    titleBarForSetupDialog(QWidget* parent = 0, Qt::WindowFlags f = 0):QWidget(parent, f)
    {
        setWindowFlags(Qt::FramelessWindowHint);
        setFixedHeight(28);
        setAutoFillBackground(true);

        QLinearGradient linearGradient(0, 0, 0, 32);
        linearGradient.setColorAt(0, QColor(175, 212, 245));
        linearGradient.setColorAt(1,QColor(175, 212, 245));

        QPalette t_pal;
        t_pal.setBrush(QPalette::Window, linearGradient);

        setPalette(t_pal);

        QLabel *t_label = new QLabel;
        t_label->setStyleSheet("color:white;");
        QFont ft;
        ft.setBold(true);
        ft.setPointSize(10.5);
        ft.setFamily("Waree");
        t_label->setFont(ft);
        t_label->setText("设置");
        t_label->adjustSize();

        m_close = new QPushButton;
        m_close->setFixedSize(28, 28);
        m_close->setFlat(true);
        m_close->setFocusPolicy(Qt::NoFocus);
        m_close->setStyleSheet("QPushButton{border-image:url(:/prefix1/1-images/btn_close_nomal.png);}"
                                "QPushButton:hover{border-image:url(:/prefix1/1-images/btn_close_hover.png);}"
                                "QPushButton:pressed{border-image:url(:/prefix1/1-images/btn_close_pressed.png);}"
                                );

        QHBoxLayout *t_1_hl = new QHBoxLayout;
        t_1_hl->setMargin(0);

        t_1_hl->addWidget(t_label);
        t_1_hl->setContentsMargins(11,0,0,0);
        t_1_hl->addWidget(m_close, 0, Qt::AlignRight);

        this->setLayout(t_1_hl);
    }

    ~titleBarForSetupDialog()
    {

    }

    void mouseMoveEvent(QMouseEvent *e)
    {
        if (e->buttons() & Qt::LeftButton)
        {
                this->parentWidget()->move(e->globalPos() - dragPosition);

                e->accept();
        }
    }

    void mousePressEvent(QMouseEvent *e)
    {
        if (e->button() == Qt::LeftButton)
        {
              dragPosition = e->globalPos() - this->parentWidget()->frameGeometry().topLeft();

              e->accept();
          }
    }
};

class itemButton : public QFrame
{
    Q_OBJECT

public:

    bool m_isCurrorEnter;
    bool m_isPressed;
    int m_id;
    QLabel *m_icon;
    QLabel *m_name;
    QLabel *m_arrow;
    QPixmap m_pix;
    QPixmap m_pixHover;

    itemButton(QPixmap icon, QPixmap icon_hover, QString name, bool flag, int id, QWidget* parent = 0, Qt::WindowFlags f = 0) : QFrame(parent, f)
    {
        setWindowFlags(Qt::FramelessWindowHint);
        this->setAutoFillBackground(true);
        this->setFixedSize(118, 28);

        m_pix = icon;
        m_pixHover = icon_hover;
        m_isCurrorEnter = flag;
        m_isPressed = flag;
        m_id = id;

        m_icon = new QLabel(this);
        m_icon->setPixmap(m_pix);
        m_icon->adjustSize();

        m_icon->setGeometry(12, (32-m_pix.height())/2-2, this->m_pix.width(), this->m_pix.height());

        m_name = new QLabel(this);
        m_name->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        QFont ft;
        ft.setPointSize(11);
        ft.setFamily("Sans");
        m_name->setFont(ft);
        m_name->setText(name);
        m_name->adjustSize();

        m_name->setGeometry(17+(m_pix.width()), (32-m_pix.height())/2-2, 100, 16);

        m_arrow = new QLabel(this);
        m_arrow->setPixmap(QPixmap(":/prefix3/3-images/left_arrow.png"));
        m_arrow->setGeometry(105, (40-m_pix.height())/2-2, 6, 9);
        if(!m_isPressed)
            m_arrow->hide();
        else
            m_arrow->show();

        setStyleSheet("QFrame{background: white;"
                      "color: grey;"
                      "font:16px;"
                      "border-radius:14px;"
                      "border: 1px;"
                      "border-style: solid;"
                      "border-color: grey;}"
                      "QFrame:hover{background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 0, 0, 0), stop:1 rgb(175, 212, 245));}");

        m_name->setStyleSheet("background: transparent;border: 0px;");
        m_icon->setStyleSheet("background: transparent;border: 0px");
        m_arrow->setStyleSheet("background: transparent;border: 0px");
    }

    void paintEvent(QPaintEvent *e)
    {
        Q_UNUSED(e);
        if(m_isCurrorEnter)
        {
            QPalette t_pal;
            t_pal.setBrush(QPalette::Window, QColor(255, 255, 255));

            setPalette(t_pal);
        }
        else
        {
            QPalette t_pal;
            t_pal.setColor(QPalette::Window, QColor(235, 241, 255));
            this->setPalette(t_pal);
        }
    }

    void enterEvent(QEvent *e)
    {
        Q_UNUSED(e);
        m_isCurrorEnter = true;
        update();
    }

    void leaveEvent(QEvent *e)
    {
        Q_UNUSED(e);
        if(!m_isPressed)
        {
            m_isCurrorEnter = false;
            update();
        }
    }

    void mousePressEvent(QMouseEvent *e)
    {
        Q_UNUSED(e);
        m_isPressed = true;
        sig_pressed();
        m_arrow->show();
        sig_sendId(m_id);
    }

    ~itemButton()
    {}

signals:
    void sig_pressed();
    void sig_sendId(int);


public slots:

    void slot_cancelBackground()
    {
        m_isCurrorEnter = false;
        m_isPressed = false;
        update();
        m_arrow->hide();
    }

};

class  bottomWidget : public QWidget
{
    Q_OBJECT

public:
    QPushButton *m_ok;
    QPushButton *m_cancel;
    QPushButton *m_recovery;

    bottomWidget(QWidget* parent = 0, Qt::WindowFlags f = 0) : QWidget(parent, f)
    {
         setWindowFlags(Qt::FramelessWindowHint);
         setFixedHeight(40);
         setAutoFillBackground(true);
         setPalette(QPalette(QColor(0,0,0,0))); //透明的

         m_recovery = new QPushButton("恢复默认设置");
         m_recovery->setFixedSize(98, 28);

         m_ok = new QPushButton("确定");
         m_ok->setFixedSize(78, 28);
         m_cancel = new QPushButton("取消");
         m_cancel->setFixedSize(78, 28);
         m_cancel->setFocusPolicy(Qt::NoFocus);

         m_recovery->setStyleSheet("QPushButton{color: grey;"
                                   "font:13px;"
                                   "border: 1px;"     //去边框
                                   "border-radius:3px;"
                                   "border-style: solid;"
                                   "border-color: grey;"
                                   "background-color: rgb(255,255,255);}"
                                   "QPushButton:hover{background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 0, 0, 0), stop:1 rgb(175, 212, 245));"
                                   "border-color: rgb(135, 141, 155);}"
                                   "QPushButton:pressed{background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 0, 0, 0),stop:1 rgba(155, 230, 245, 200));}"
                                   );

         m_ok->setStyleSheet("QPushButton{color: grey;"
                             "font:13px;"
                             "border: 1px;"     //去边框
                             "border-radius:3px;"
                             "border-style: solid;"
                             "border-color: grey;"
                             "background-color: rgb(255,255,255);}"
                             "QPushButton:hover{background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 0, 0, 0), stop:1 rgb(175, 212, 245));"
                             "border-color: rgb(135, 141, 155);}"
                             "QPushButton:pressed{background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 0, 0, 0),stop:1 rgba(155, 230, 245, 200));}"
                             );

         m_cancel->setStyleSheet("QPushButton{color: grey;"
                                 "font:13px;"
                                 "border: 1px;"     //去边框
                                 "border-radius:3px;"
                                 "border-style: solid;"
                                 "border-color: grey;"
                                 "background-color: rgb(255,255,255);}"

                                 "QPushButton:hover{background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 0, 0, 0), stop:1 rgb(175, 212, 245));"
                                 "border-color: rgb(135, 141, 155);}"
                                 "QPushButton:pressed{background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 0, 0, 0),stop:1 rgba(155, 230, 245, 200));}"
                                 );

        QHBoxLayout *t_1_hl = new QHBoxLayout;
        t_1_hl->setMargin(0);
        t_1_hl->setSpacing(8);

        t_1_hl->addWidget(m_recovery);
        t_1_hl->addStretch();
       // t_1_hl->addWidget(m_ok);
        t_1_hl->addWidget(m_cancel);
        t_1_hl->addWidget(m_ok);

        QWidget *t_w = new QWidget;
        t_w->setAutoFillBackground(true);

        t_w->setPalette(QPalette(Qt::white));

        t_w->setLayout(t_1_hl);

        QVBoxLayout *t_1_vl = new QVBoxLayout;
        t_1_vl->setContentsMargins(5, 0, 5, 0);
        t_1_vl->setSpacing(0);

        t_1_vl->addWidget(t_w);

        this->setLayout(t_1_vl);

     }
     ~bottomWidget()
     {}
};

//class setupDoneWidget;
class setUpDialog : public QDialog
{
    Q_OBJECT

public:
    normalWidget *m_normalWidget;
    habitWidget *m_habitWidget;
    slideWidget *m_slideWidget;
    setUpDialog(QWidget *parent = 0);
    QLabel *recovery_label;
    QStackedWidget *t_stackedWidget;
    ~setUpDialog();

    void obtainSetInfo();
    void paintEvent(QPaintEvent *e);
    void closeEvent(QCloseEvent *e);


public slots:
    void slot_confirmConfig();
    void slot_recovery();
    void slot_cancel();

signals:
    void sig_sendSlideSetInfo(QString setInfo);
    void sig_normalSetting1(bool isChecked );
    void sig_normalSetting2(bool isChecked );
    void sig_normalSetting3(bool isChecked );
    void sig_normalSetting4(bool isChecked );
    void sig_normalSetting5(bool isChecked );
private:
    QString init_normalwidget;
};


#endif // SETUPDIALOG_H
