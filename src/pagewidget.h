#ifndef PAGEWIDGET
#define PAGEWIDGET

#include "define.h"
#include <QWidget>
#include <QLine>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QRadioButton>
#include <QPainter>
#include <QComboBox>
#include <QSpinBox>
#include <QDebug>
#include <QListWidget>
#include <QFile>
#include <QButtonGroup>

extern QString configList[ROWS];

class checkBox : public QCheckBox
{
    Q_OBJECT
public:
    checkBox(QString name, QWidget *parent=0) : QCheckBox(parent)
    {
        //this->setStyleSheet("color:#464646;");
        setStyleSheet("QCheckBox{color: grey;"
                      "spacing: 2px;}"
                      "QCheckBox::indicator{"
                      "width: 20px;"
                      "height: 20px;}"
                      "QCheckBox::indicator:unchecked {"
                      "image: url(:/prefix1/1-images/checkbox_unchecked.png);}"
                      "QCheckBox::indicator:checked {"
                      "image: url(:/prefix1/1-images/checkbox_checked.png);}");
        this->setText(name);
        this->setFocusPolicy(Qt::NoFocus);
    }
    ~checkBox()
    {

    }
};

class radioButton : public QRadioButton
{
public:
    radioButton(const QString &text, QWidget *parent=0) : QRadioButton(text, parent)
    {
        setStyleSheet("QRadioButton{color: grey;"
                      "spacing: 2px;}"
                      "QRadioButton::indicator{"
                      "width: 15px;"
                      "height: 15px;}"
                      "QRadioButton::indicator:unchecked {"
                      "image: url(:/prefix1/1-images/radio_normal.png);}"
                      "QRadioButton::indicator:checked {"
                      "image: url(:/prefix1/1-images/radio_selected.png);}");
        this->setFocusPolicy(Qt::NoFocus);
    }
    ~radioButton()
    {}
};


class normalWidget : public QWidget
{
    Q_OBJECT

public:
    normalWidget (QWidget* parent = 0, Qt::WindowFlags f = 0);
    checkBox *m_1_cb;
    checkBox *m_2_cb;
    checkBox *m_3_cb;
    checkBox *m_4_cb;
    checkBox *m_5_cb;

    radioButton *m_8_cb;
    radioButton *m_7_cb;
    QButtonGroup* mCbBtnGroup;

    ~normalWidget();

    void setNormalWidetConfig(const QString &config);
    //生成中间变量 保存t_configList 返回
    const QString getNormalWidgetConfig();


public slots:
    void slot_test();
    void slot_normalSetting1_cb();
    void slot_normalSetting2_cb();
    void slot_normalSetting3_cb();
    void slot_normalSetting4_cb();
    void slot_normalSetting5_cb();
signals:
    void sig_normalSetting1(bool isChecked );
    void sig_normalSetting2(bool isChecked );
    void sig_normalSetting3(bool isChecked );
    void sig_normalSetting4(bool isChecked );
    void sig_normalSetting5(bool isChecked );

#if 0  //没有colseEvent
    void closeEvent(QCloseEvent *e)
    {
        Q_UNUSED(e);
        qDebug() << "normalWidget 写文件";
    }
#endif

};

class habitWidget : public QWidget
{
    Q_OBJECT
public:
    habitWidget (QWidget* parent = 0, Qt::WindowFlags f = 0);
    radioButton *m_1_rb;
    radioButton *m_2_rb;


    void sethabitWidgetConfig(const QString &config);
    const QString gethabitWidgetConfig();

    ~habitWidget();

};




class slideWidget : public QWidget
{
    Q_OBJECT
public:

    radioButton *m_1_rb;
    radioButton *m_2_rb;
    QSpinBox *m_spinbox;
    QComboBox *m_1_comboBox;
    QComboBox *m_2_comboBox;
    slideWidget (QWidget* parent = 0, Qt::WindowFlags f = 0);

    ~slideWidget();

    void setslideWidgetConfig(const QString &config1, const QString &config2);
    const QString getslideWidgetConfig();
    const QString getValue();
    void setComboboxStyle(QComboBox *comboBox);

};

class helpWidget : public QWidget
{
    Q_OBJECT
public:
    helpWidget (QWidget* parent = 0, Qt::WindowFlags f = 0) ;
    ~helpWidget();

};

#endif // PAGEWIDGET

