#ifndef SLIDEDIALOG_H
#define SLIDEDIALOG_H

#include <QDialog>
#include "common.h"
#include "define.h"
#include <QDesktopWidget>
#include <QTimerEvent>
#include <QComboBox>
#include <QQueue>
#include <QThread>
#include "opacityanimation.h"
#include <QDesktopWidget>
#include <QRadioButton>

#define NUMOFBUFFER 5

extern QString configList[ROWS];

class FaderWidget : public QWidget
{
    Q_OBJECT

public:
    FaderWidget(QWidget *parent);

    void start();

protected:
    void paintEvent(QPaintEvent *event);

private:
    QTimer *timer;
    int currentAlpha;
    QColor startColor;
    int duration;
};

/*end add*/

//效果选择项
class SelectItem : public QRadioButton
{
public:
    SelectItem(const QString &text, QWidget *parent=0) : QRadioButton(text, parent)
    {
        setFocusPolicy(Qt::NoFocus);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setStyleSheet("QRadioButton{background: transparent; border:none; font:13px; color: rgb(180, 180, 180); spacing: 5px;}"
                       "QRadioButton:hover{background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 0, 0, 0), stop:0.4 rgba(201, 234, 255, 150),stop:1 rgb(0, 0, 0, 0));"
                       "color:white;}"
                       "QRadioButton:checked{color:rgb(201,234,255);}"
                       "QRadioButton::indicator{width: 16px; height: 16px; margin-left: 10}"
                       "QRadioButton::indicator:unchecked {"
                       "image: none;}"
                       "QRadioButton::indicator:checked {"
                       "image: url(:/prefix3/3-images/s_selected.png);}");
    }
    ~SelectItem()
    {}
};
//选择效果对话框
class EffectDialog : public QDialog
{
    Q_OBJECT

public:

    QPixmap pix;
    SelectItem *m_btn[5];

    EffectDialog(int i=0, QWidget *parent=0) : QDialog(parent)
    {
        setWindowFlags(Qt::FramelessWindowHint);
        setAttribute(Qt::WA_TranslucentBackground);
        setAttribute(Qt::WA_DeleteOnClose);

        pix.load(":/prefix3/3-images/effect_background.png");
        setFixedSize(pix.size());

        for(int i=0; i<5; i++)
        {
            m_btn[i] = new SelectItem("");
            connect(m_btn[i], SIGNAL(clicked()), this, SLOT(slot_getcheckedItem()));
        }
        m_btn[0]->setText("无效果");
        m_btn[1]->setText("快速推入");
        m_btn[2]->setText("淡入淡出");
        m_btn[3]->setText("扇形飞入");
        m_btn[4]->setText("随机效果");

        checkItem(i);

        QFrame *t_line[4];

        for(int i=0; i<4; i++)
        {
            t_line[i] = new QFrame;
            t_line[i]->setFrameShape(QFrame::HLine);
            t_line[i]->setFixedHeight(1);
            t_line[i]->setFixedSize(width()-20, 1);
            t_line[i]->setFrameShadow(QFrame::Sunken);
            t_line[i]->setStyleSheet("border: 1px inset black;");
        }

        QVBoxLayout *t_1_vl = new QVBoxLayout;
        t_1_vl->setContentsMargins(0, 0, 0, 13);
        t_1_vl->setSpacing(0);

        t_1_vl->addWidget(m_btn[0]);
        t_1_vl->addWidget(t_line[0], 0, Qt::AlignHCenter);
        t_1_vl->addWidget(m_btn[1]);
        t_1_vl->addWidget(t_line[1], 0, Qt::AlignHCenter);
        t_1_vl->addWidget(m_btn[2]);
        t_1_vl->addWidget(t_line[2], 0, Qt::AlignHCenter);
        t_1_vl->addWidget(m_btn[3]);
        t_1_vl->addWidget(t_line[3], 0, Qt::AlignHCenter);
        t_1_vl->addWidget(m_btn[4]);

        setLayout(t_1_vl);
    }

    ~EffectDialog()
    {}

    void paintEvent(QPaintEvent *e)
    {
        QPainter painter(this);
        painter.drawPixmap(0, 0, pix);//绘制背景
    }

    void checkItem(const int id)
    {
        m_btn[id]->setChecked(true);
    }

public slots:
    void slot_getcheckedItem()
    {
        for(int i=0; i<5; i++)
        {
            if(m_btn[i]->isChecked())
            {
                sig_sendID(i);
                break;
            }
        }

        //hide();
    }

signals:
    void sig_sendID(int);
};


class imgInfo
{
public:
    QPixmap m_pix;
    int m_num;
};

class loadSlideThread : public QThread {

    Q_OBJECT

public:
    QString m_filename;
    QImage currentImg;

    ImageReader *imageReader;
    int desktopW;
    int desktopH;

    bool breakFlag;

    QQueue<imgInfo> m_pixQueue;      //缓冲队列
    QFileInfoList m_allImagesInfo;
    int m_StartingPoint;             //播放的起点 0～n

    loadSlideThread(QThread *parent = 0) : QThread(parent)
    {
        imageReader = new ImageReader(true);
    }

    void createImg()
    {
        currentImg = imageReader->readImage(m_filename);

        imgInfo imageInfo;
        QPixmap tmpPix = QPixmap::fromImage(currentImg);
        if(tmpPix.width() > desktopW || tmpPix.height() > desktopH)
        {
            imageInfo.m_pix = tmpPix.scaled(QSize(desktopW,desktopH),Qt::KeepAspectRatio);
        }
        else
        {
            imageInfo.m_pix = tmpPix;
        }
        imageInfo.m_num = m_allImagesInfo.indexOf(QFileInfo(m_filename));
        m_pixQueue.enqueue(imageInfo);
    }

signals:
    void sig_sendThumbnail(QImage);

protected:
    void run()
    {
        desktopW = QApplication::desktop()->width();
        desktopH = QApplication::desktop()->height();
        qsrand(time(NULL));

        if(!m_pixQueue.isEmpty())
            m_pixQueue.clear();

        //队列中保存5张图片
        while(breakFlag)
        {
            //如果所有图片大于5张
            if(m_allImagesInfo.length() > NUMOFBUFFER)
            {
                if(m_pixQueue.length() < NUMOFBUFFER)
                {
                    //顺序播放
                    if(configList[4].at(0) == QChar('1'))
                    {
                        if(m_StartingPoint == m_allImagesInfo.length()-1)
                            m_StartingPoint = 0;
                        else
                            m_StartingPoint++;
                        m_filename = m_allImagesInfo.at(m_StartingPoint).absoluteFilePath();
                    }
                    else //随机播放
                    {
                        int n = qrand() % m_allImagesInfo.length();
                        m_filename = m_allImagesInfo.at(n).absoluteFilePath();
                    }

                    createImg();
                }

            }
            else //所有图片 <= 5张  保存了所有图片退出循环
            {
                if(m_pixQueue.length() == m_allImagesInfo.length())
                    break;

                //设置入队起点 应该是当前显示图片的下一张
                if(m_StartingPoint == m_allImagesInfo.length()-1)
                    m_StartingPoint = 0;
                else
                    m_StartingPoint++;

                m_filename = m_allImagesInfo.at(m_StartingPoint).absoluteFilePath();

                createImg();

            }
        }
    }
};



class menuButton : public QPushButton
{
    Q_OBJECT

public:
    menuButton(QSize size, QWidget *parent=0) : QPushButton(parent)
    {
        this->setFixedSize(size);
        this->setFlat(true);
        this->setFocusPolicy(Qt::NoFocus);
    }

    ~menuButton()
    {}
};

class menuBar : public QFrame
{
    Q_OBJECT
public:

    menuButton *m_increase;
    menuButton *m_reduce;
    menuButton *m_quit;
    menuButton *m_pause;
    menuButton *m_effect;
    QLabel *m_interval;
    QLabel *m_counter;
    opacityAnimation *m_animation;

    bool getIsEntered();
    void meneBarSetStyleSheet();

    menuBar(QWidget *parent = 0, Qt::WindowFlags f = 0);
    ~menuBar();

    void enterEvent(QEvent* e);
    void leaveEvent(QEvent * e);

private:
    bool m_isEntered;

    menuButton *m_left;
    menuButton *m_right;
    QComboBox *m_comboBox;
};

class slideDialog : public QDialog
{
    Q_OBJECT

public:
   int m_total;
   QPixmap m_currentPix;
   loadSlideThread *m_loadSlideThread;
   int m_currentNum;
   bool isRunning;
   QPointer<FaderWidget> faderWidget;
   QString m_currentfile;
   QMovie *movie;
   ImageReader *imageReader;


   slideDialog(QWidget *parent = 0, Qt::WindowFlags f = 0);
   ~slideDialog();

    void resizeEvent(QResizeEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *e);

    void setCurrentPixmap(QString &);
    void playEffectSetting(int index);
    QSize getImageSize(const QString &);


    QPalette backgroundColor;
    int m_currentPlayEffectIndex;
    int m_currentInterval;


signals:
    void sig_sendCurrentNum(int);

public slots:
    void slot_hideMenuBar();
    void slot_quit();
    void slot_updatePixmap();
    void slot_upInterval();
    void slot_downInterval();
    void slot_setPlay();
    void slot_showEffectDialog();
    void slot_setCurrentPlayEffectIndex(int id);

private:
   menuBar *m_menuBar;
   QTimer *m_1_timer;
   QTimer *m_2_timer;
   QLabel *m_pixLabel;

   int m_currentPlayOrder;

   int m_i; // 控制 < 5的个数
   EffectDialog *m_effectDialog;
};

#endif // SLIDEDIALOG_H
