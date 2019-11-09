#include "picinfowidget.h"
#include <QDesktopWidget>
#include <QApplication>

picInfoWidget::picInfoWidget(QWidget *parent) : QScrollArea(parent)
{
    setFixedWidth(200);
    setAutoFillBackground(true);
    setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    setFrameShape(QFrame::NoFrame);

//    setAttribute(Qt::WA_TranslucentBackground, true);
     setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    QFrame* line = new QFrame;
    line->setAutoFillBackground(true);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    QFrame* line2 = new QFrame;
    line2->setAutoFillBackground(true);
    line2->setFrameShape(QFrame::HLine);
    line2->setFrameShadow(QFrame::Sunken);


    QLabel *photoInfo = new QLabel(tr("图片信息"));
    QLabel *fileInfo  = new QLabel(tr("文件信息"));
    lb_PhotoInfo = new QLabel;
    lb_FileInfo = new QLabel;

    lb_PhotoInfo->setAttribute(Qt::WA_TranslucentBackground);
    lb_PhotoInfo->setStyleSheet("color:black;");

    lb_FileInfo->setAttribute(Qt::WA_TranslucentBackground);
    lb_FileInfo->setStyleSheet("color:black;");

    photoInfo->setAttribute(Qt::WA_TranslucentBackground);
    photoInfo->setStyleSheet("color:black;");

    fileInfo->setAttribute(Qt::WA_TranslucentBackground);
    fileInfo->setStyleSheet("color:black;");

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setContentsMargins(5, 10, 10, 0);
    vbox->addWidget(photoInfo);
    vbox->addWidget(line);

    vbox->addWidget(lb_PhotoInfo);
    vbox->addSpacing(15);
    //vbox->addStretch(2);
    vbox->addWidget(fileInfo);
    vbox->addWidget(line2);
    vbox->addWidget(lb_FileInfo);
    vbox->addStretch(30);

    QWidget *t_widget = new QWidget;
    t_widget->setLayout(vbox);

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setWidgetResizable(true);
    setWidget(t_widget);

    horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal"
                                "{height: 5px;"
                                "background: #CCCCFF;"
                                "border: 0px;}"
                        "QScrollBar::handle:horizontal"
                                "{height: 5px;"
                                "background: rgb(65, 102, 245);}"
                        "QScrollBar::add-line:horizontal"
                                "{height: 0px; width: 0px;}"
                        "QScrollBar::sub-line:horizontal"
                                "{height: 0px; width: 0px;}");

    setStyleSheet("background: rgb(255, 251, 240,200);");
    t_widget->setStyleSheet("color: white");
}


void picInfoWidget::mouseDoubleClickEvent(QMouseEvent *e){}
void picInfoWidget::adjustSize()
{
        QDesktopWidget* desktop = QApplication::desktop();
        QApplication::desktop()->width();
        setGeometry(desktop->pos().x(),desktop->pos().y(),desktop->size().width(),parentWidget()->height() - TOOLBARHEIGTH);
}

void picInfoWidget::receiveThreadMsg(QFileInfo imgInfo, QStringList sizeAndDpi)
{
    lb_PhotoInfo->setText(tr("名称：")+imgInfo.fileName()
    +tr("\n\n类型：")+imgInfo.suffix()
    +tr("\n\n尺寸：")+sizeAndDpi.at(0)+" x "+sizeAndDpi.at(1)+tr(" 像素")
    +tr("\n\n分辨率：")+sizeAndDpi.at(2)+"*"+sizeAndDpi.at(3)+ "dpi");

    qint64 t_int_size = imgInfo.size();
    QString t_string_size;

    if(t_int_size >= 1000 && t_int_size <= 1000*1000)
       t_string_size = QString::number((imgInfo.size()/1000.0),'f',2)+"KB";
    if(t_int_size >= 1000*1000)
       t_string_size = QString::number((imgInfo.size()/(1000.0*1000.0)),'f',2)+"MB";

    lb_FileInfo->setText(tr("文件夹路径：")+imgInfo.absolutePath()
    +tr("\n\n创建时间：")+imgInfo.created().toString("yyyy/MM/dd hh:mm:ss")
    +tr("\n\n修改时间：")+imgInfo.lastModified().toString("yyyy/MM/dd hh:mm:ss")
    +tr("\n\n大小：")+ t_string_size
    +tr("\n\n所有者：")+imgInfo.owner());
}


