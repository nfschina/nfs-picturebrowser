#include "loadthumbnailthread.h"

loadThumbnailThread::loadThumbnailThread()
{
    saveAsFlag = 0;
    breakNum = 0;
    selectedNum =0;
    imgNum = 0;
    next =0;
    pre=0;
    id = 0;
    imageReader = new ImageReader(true);
}


Image::Image()
{

}

//x：y：图片在背景的位置。sx：图片被使用的起始位置。w：图片被使用的宽度
//将缩略图放置在背景图片上
void Image::painter(QImage image, int x, int y, int w, int h)
{
    QRectF target(x, y, w, h);    //目标文件位置
    QRectF source(0, 0.0, w, h);

    QPainter painter(this);
    painter.drawImage(target, image, source);
}

void loadThumbnailThread::setimg(const int &i)
{
    float multiple = 0;//倍数

    QImage img;
    Image imgFlag72;

    imgFlag72.load(":/prefix2/2-images/bkbig.png");

    imageReader->readImage(allImgs.at(i).absoluteFilePath());
    QImageReader *imgread = new QImageReader(imageReader->fileChanged);

//    QImageReader *imgread = new QImageReader(allImgs.at(i).absoluteFilePath());
    //判断图片是否支持

    multiple = (imgread->size().width()/72.0 > imgread->size().height()/THUMBNAILHEIGTH
                ?imgread->size().width()/72.0:imgread->size().height()/THUMBNAILHEIGTH);

    if(multiple>0)
        imgread->setScaledSize(imgread->size()/(multiple));//设置缩略图的  缩略比例

    int x = (72-imgread->scaledSize().width())/2.0;
    int y = (THUMBNAILHEIGTH-imgread->scaledSize().height())/2.0;

//    imgread->read(&img);
    if(imgread->read(&img))
    {
        imgFlag72.painter(img, x, y, 72, THUMBNAILHEIGTH);
        if(breakNum == 0)
        {
            emit sendThumb(imgFlag72, img, allImgs.at(i).fileName(), /*i, */id);
        }
    }
}

void loadThumbnailThread::run()
{
    if(pre == 1)
    {
        for(int i = 0; i<imgNum/2+1; i++)
        {
           pre = selectedNum - i;

           if(pre<0)
               pre = allImgs.length()+pre;

           setimg(pre);

           if(breakNum == 1)
           {
                breakNum = 0;
                break;
           }
        }

        pre = 0;
        next = 0;
    }

    if(next == 1)
    {
        for(int i = 1; i<imgNum/2+1; i++)
        {
           next = selectedNum + i;

           if(next > allImgs.length()-1)
               next =next - allImgs.length();

           setimg(next);

           if(breakNum == 1)
           {
                breakNum = 0;
                break;
           }
        }

        next = 0;
        pre = 0;
    }

}
