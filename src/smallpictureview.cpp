#include "smallpictureview.h"

extern QString configList[ROWS];

smallPictureView::smallPictureView(QWidget *parent) : QGraphicsView(parent)
{

    if(configList[0].at(4) == QChar('1'))
        m_flag = Show;
    else
        m_flag = Hide;

    if(m_flag == Hide)
        hide();
    else
        show();

    currentThreadId = 0;
    wheelFlag = 0;
    selectedNum = 0;
    oldSelectNum = 0;
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setStyleSheet("background:transparent;border:0px");
    this->setScene(&scene);
    this->setFixedHeight(THUMBNAILHEIGTH);

    saveAsThread = new loadThumbnailThread;

    ThumbThreadpre = new loadThumbnailThread;
    connect(ThumbThreadpre,SIGNAL(sendThumb(QImage, QImage, QString, /*int, */int)),
            this,SLOT(slot_itemAddImgpre(QImage, QImage, QString, /*int, */int)));

    ThumbThreadnext = new loadThumbnailThread;
    connect(ThumbThreadnext,SIGNAL(sendThumb(QImage, QImage, QString, /*int, */int)),
            this,SLOT(slot_itemAddImgnext(QImage, QImage, QString, /*int, */int)));

    this->setCursor(Qt::ClosedHandCursor);

    this->timeLine = new QTimeLine(150);
}


/******************************************
1.从imgwidget获取到allimgs后，给secene添加item，
并且把allimgs传给线程对象1,让他创建半张和整张缩略图。
为下一步item添加图片做基础
*******************************************/
void smallPictureView::setItem()
{
    //1.重新加载设置item和图片，所以需要清空
    if(!allPixItem.isEmpty())
    {
        qDeleteAll(allAnimation.begin(), allAnimation.end());
        qDeleteAll(midPixItems.begin(), midPixItems.end());
        qDeleteAll(allPixItem.begin(), allPixItem.end());

        allAnimation.clear();
        midPixItems.clear();
        allPixItem.clear();

        scene.clear();
    }

    //3.缩略图半张图的item准备好,并添加到scene,位移的变量也准备好
    for(int i =0;i<imgNum;++i)
    {
        //创建两张空的链表  做准备
        smallPictureItem *pixItemhalf = new smallPictureItem;
        smallPictureItem *pixItemfull = new smallPictureItem;

        allPixItem.append(pixItemhalf);
        midPixItems.append(pixItemfull);
        scene.addItem(allPixItem.at(i));

        QGraphicsItemAnimation *animation = new QGraphicsItemAnimation;
        allAnimation.append(animation);
    }
    //4.保存中间的全张缩略图的item--imgNum也准备好，并且添加到scene，位移变量也准备好
    midPixItemFlag = midPixItems.at(selectedNum);
    scene.addItem(midPixItemFlag);

    QGraphicsItemAnimation *animation = new QGraphicsItemAnimation;
    allAnimation.append(animation);
    allAnimation.at(imgNum)->setItem(midPixItemFlag);

    //设置本次的线程ID
    if(ThumbThreadpre->id == 10000)
        ThumbThreadpre->id = 0;
    ThumbThreadpre->id++;

    ThumbThreadpre->pre = 1;     //pre 为 1 0
    ThumbThreadpre->selectedNum = selectedNum;
    ThumbThreadpre->allImgs = allImgs;
    ThumbThreadpre->imgNum = imgNum;

    //设置本次线程ID
    if(ThumbThreadnext->id == 10000)
        ThumbThreadnext->id = 0;
    ThumbThreadnext->id++;

    ThumbThreadnext->next = 1; //next 为 0 1
    ThumbThreadnext->selectedNum = selectedNum;
    ThumbThreadnext->allImgs = allImgs;
    ThumbThreadnext->imgNum = imgNum;

    //5.allimgs传给线程对象1，让他创建半张和全张缩略图，并传递过来

    //QThread::sleep(5);

    ThumbThreadnext->start();
    ThumbThreadpre->start();
}
/**********************************
* 2.接收线程发送过来的半张和全张缩略图  *
* 分别添加到midpixitem和allpixitem *
***********************************/
void smallPictureView::slot_itemAddImgpre(const QImage &fullImg, const QImage &img,
                                     const QString &filename, const int &id)
{
    if(id != currentThreadId)
    {
        return;
    }

    //判断线程发来的图片 是否为在当前图片链表中(防止缩略图没有加载完，就删除图片)
    if(!allImgsName.contains(filename))
    {
        return;
    }

    //根据图片的名称 来找到索引
    int i= allImgsName.indexOf(filename);

    QImage halfImg = fullImg.copy(18,0,36,THUMBNAILHEIGTH);
    midPixItems.at(i)->setPixmap(QPixmap::fromImage(fullImg));
    midPixItems.at(i)->m_thumbnail = img;

    allPixItem.at(i)->setPixmap(QPixmap::fromImage(halfImg));
    allPixItem.at(i)->setData(1001,allImgs.at(i).absoluteFilePath());
    allPixItem.at(i)->setToolTip(
                "<span style=\"color: white; background-color: transparent;\">"
                                   + allImgs.at(i).fileName() + "</span>");
    midPixItems.at(i)->setToolTip(
                "<span style=\"color: white; background-color:transparent;\">"
                                   + allImgs.at(i).fileName() + "</span>");
    setItemPos(i);
}

void smallPictureView::slot_itemAddImgnext(const QImage &fullImg,const QImage &img,
                                      const QString &filename, const int &id)
{
    if(id != currentThreadId)
    {
        return;
    }

    if(!allImgsName.contains(filename))
    {
        return;
    }

    int i = allImgsName.indexOf(filename);

    QImage halfImg = fullImg.copy(18, 0, 36, THUMBNAILHEIGTH);
    midPixItems.at(i)->setPixmap(QPixmap::fromImage(fullImg));
    midPixItems.at(i)->m_thumbnail = img;

    allPixItem.at(i)->setPixmap(QPixmap::fromImage(halfImg));
    allPixItem.at(i)->setData(1001,allImgs.at(i).absoluteFilePath());
    allPixItem.at(i)->setToolTip(
                "<span style=\"color: white; background-color: transparent;\">"
                                   + allImgs.at(i).fileName() + "</span>");
    midPixItems.at(i)->setToolTip(
                "<span style=\"color: white; background-color:transparent;\">"
                                   + allImgs.at(i).fileName() + "</span>");
    setItemPos(i);
}
/**********************************
 *      动态设置item的位置          *
 *  窗口改变时需要动态传递进来窗口大小 *
 *           并改变位置            *
**********************************/
void smallPictureView::setItemPos(int i)
{
    scene.setSceneRect(0, 0, widwidth, widheight);

    if(i<selectedNum)
    {//left  --
        //将该项目的位置设置为给定的步长值到指定的点。
       allPixItem.at(i)->setPos((36+1)*(i-selectedNum-1)+widwidth/2.0, 0);

    }
    else if(i>selectedNum)
    {//right  ++
        //将该项目的位置设置为给定的步长值到指定的点。
        allPixItem.at(i)->setPos((36+1)*(i-selectedNum)+widwidth/2.0+1, 0);
    }
    else
    {//selected
        //将该项目的位置设置为给定的步长值到指定的点。
        midPixItemFlag->setPos(widwidth/2.0-36, 0);
        allPixItem.at(i)->setPos(widwidth/2.0-18, 0);
        midPixItemFlag->show();
        allPixItem.at(i)->show();
        allPixItem.at(i)->hide();
    }
}

//移动动画
void smallPictureView::movItemPos(QTimeLine *timeLine, int i)
{
//click function
//鼠标点击或者滑轮事件后，缩略图的改变
//    qDebug() << "movItemPos(int i)";
    allPixItem.at(i)->show();
    allAnimation.at(i)->setTimeLine(timeLine);
    allAnimation.at(i)->clear();
    allAnimation.at(i)->setItem(allPixItem.at(i));

    if(i<selectedNum)
    {//left
        //将该项目的位置设置为给定的步长值到指定的点。
        allAnimation.at(i)->setPosAt(1, QPointF(0+(36+1)*(i-selectedNum-1)+width()/2.0, 0));
    }
    else if(i>selectedNum)
    {
        //将该项目的位置设置为给定的步长值到指定的点。
        allAnimation.at(i)->setPosAt(1, QPointF(1+(36+1)*(i-selectedNum)+width()/2.0, 0));
    }
    else
    {
        if(selectedNum != oldSelectNum)
        {
            if(midPixItemFlag)
                midPixItemFlag->hide();
            oldSelectNum = selectedNum;
        }
        //将该项目的位置设置为给定的步长值到指定的点。
        allAnimation.at(i)->setPosAt(1, QPointF(-18+(36+1)*(i-selectedNum)+width()/2.0, 0/*widheight-81*/));
    }

}

/*中间大的那张缩略图的一些设置*/
void smallPictureView::timelineFinished()
{
//删除图片 midpixitemflag 被释放 赋了NULL
    if(midPixItemFlag)
    {
        scene.removeItem(midPixItemFlag);
    }
    //先删除上一次的 中间图片
    if(midPixItems.length() > 0 && allPixItem.length() > 0)
    {
        midPixItemFlag = midPixItems.at(selectedNum);   //
        scene.addItem(midPixItemFlag);
        midPixItemFlag->setPos(width()/2.0-36, 0);
        midPixItemFlag->show();
        allPixItem.at(selectedNum)->hide();
    }
}

void smallPictureView::changeImg(int time)//缩略图移动
{   
    midPixItemFlag->hide();

    QTimeLine *timeLine = new QTimeLine(time);
    connect(timeLine,SIGNAL(finished()),this,SLOT(timelineFinished()));
    for(int i=0; i<imgNum; i++)
    {
        movItemPos(timeLine, i);
    }
    timeLine->start();

//发送信号  maintaskWidget加载原图
    sig_sendToDisplay(selectedNum);
    wheelFlag = 0;
}

void smallPictureView::wheelEvent(QWheelEvent *e)
{
    if(!allImgs.isEmpty())
    {
        if (e->orientation() == Qt::Vertical)
        {
//            if(wheelFlag == 0)
//            {
//                QTimer::singleShot(1, this, SLOT(slot_wheelChangeImg()));
//            }

            int numDegrees = e->delta() / 8;//滚动的角度，*8就是鼠标滚动的距离

            int numSteps = numDegrees / 15;//滚动的步数，*15就是鼠标滚动的角度

            wheelFlag += numSteps;

            selectedNum -= numSteps;

            if(selectedNum<0)
            {
                selectedNum = imgNum-1;
            }
            else if(selectedNum>=imgNum)
            {
                selectedNum = 0;
            }

            currentfile = allPixItem.at(selectedNum)->data(1001).toString();
        }
        else
        {
            // qDebug()<<"水平滚动 = ";      //水平滚动
        }

        changeImg(200);
        e->accept();      //接收该事件
    }
    else
    {
        QMessageBox message(QMessageBox::NoIcon, tr("提示"), tr("请先选择图片文件!"));
        message.setIconPixmap(QPixmap("../2.png"));
        message.exec();
    }
}

void smallPictureView::mouseDoubleClickEvent(QMouseEvent *e)
{
    mousePressEvent(e);
}

void smallPictureView::mousePressEvent(QMouseEvent *e)
{
    if(e->buttons() == Qt::RightButton)
        return;

    if(!allImgs.isEmpty())
    {
        currentfile = this->itemAt(e->pos())->data(1001).toString();


        if(currentfile != "")
        {
            for(int i=0; i<imgNum; i++)
            {
                if(!allImgs.at(i).absoluteFilePath().compare(currentfile))
                    selectedNum = i;
            }

        }

        if(selectedNum != oldSelectNum)
        {
            if(wheelFlag == 0)
            {
                //QTimer::singleShot(1, this, SLOT(slot_wheelChangeImg()));
            }

            wheelFlag = 1;
            changeImg(150);
        }
    }
    else
    {
        QMessageBox message(QMessageBox::NoIcon, tr("提示"), tr("请先选择图片文件!"));
        message.setIconPixmap(QPixmap("../2.png"));
        message.exec();
    }
}

//删除图片 更新缩略图 包括位置和该删除的
void smallPictureView:: slot_updatePos()
{
    if(!allImgs.isEmpty())
    {
        if(imgNum>0)
        {
            allImgs.removeAt(selectedNum);         //删除文件名称
            allImgsName.removeAt(selectedNum);
            allAnimation.removeAt(selectedNum);    //删除动画

            scene.removeItem(midPixItems.at(selectedNum)); //清空场景
            scene.removeItem(allPixItem.at(selectedNum));

            delete midPixItems.at(selectedNum);
            delete allPixItem.at(selectedNum);

            midPixItems.removeAt(selectedNum);     //删除72, 48
            allPixItem.removeAt(selectedNum);      //删除36, 48

            if(selectedNum == imgNum-1)
                selectedNum = 0;

            imgNum--;

            oldSelectNum = selectedNum;

            if(imgNum>0)
            {
                currentfile = allImgs.at(selectedNum).absoluteFilePath();
                //changeImg(1);   //移动缩略图 不设动画 防止动画没有结束 就将item删除

                //将中间因为此时已经将item释放
                midPixItemFlag = midPixItems.at(selectedNum);
                scene.addItem(midPixItemFlag);

                //设置位置
                for(int i=0; i<imgNum; i++)
                {
                    setItemPos(i);
                }
            }
        }
    }
}


smallPictureView::~smallPictureView()
{

}

showOrhide smallPictureView::getShowOrHide()
{
    return m_flag;
}

void smallPictureView::setShowOrHide(showOrhide f)
{
    m_flag = f;
}


