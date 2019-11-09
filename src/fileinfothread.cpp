#include"fileinfothread.h"
fileInfoThread::fileInfoThread()
{

}

void fileInfoThread::run()
{
    // 得到文件所在的文件夹路径
    QString currentdir = "";

    if(QFileInfo(orignalFile).isFile())
    {
        currentdir = QFileInfo(orignalFile).absoluteDir().absolutePath();
    }
    else
    {
        //如果在目录中删除了当前显示的图片文件 点击刷新 执行此else获得文件路径
        int index = orignalFile.lastIndexOf("/");
        currentdir = orignalFile.left(index+1);

        int next = currentNum + 1; //将当前文件设置为下一张
        if(next == imagesNum)
        {
            next = 0;
        }
        orignalFile = allImagesInfo.at(next).absoluteFilePath();
    }

    //创建文件夹
    QDir *new_dir = new QDir(currentdir);

    if(!allImagesInfo.isEmpty())
    {
        allImagesInfo.clear();
    }
    if(!allImagesName.isEmpty())
    {
        allImagesName.clear();
    }

    allImagesInfo = new_dir->entryInfoList(QDir::NoFilter,QDir::IgnoreCase);

    //找到当前文件路径在 链表中的序号
    int t_length = allImagesInfo.length();

    QStringList format;
    format << "bmp" << "gif" << "jpg" << "jpeg" << "png"
           << "pbm" << "pgm" << "ppm" << "xbm" << "xpm"
            <<"jp2" <<"ico" <<"tif"<<"tiff"<<"dib"
           <<"pcx" <<"xcf"<<"svg"<<"eps"<<"bitmap"<<"jpeg2000"
          <<"jpc"<<"j2k"<<"jpf"<<"psd"<<"icns"<<"svgz"<<"pnm"
         <<"ras"<<"wbmp"<<"ani"
              ;

    //找出文件夹中损坏的图片 并删除
    for(int i=0; i<t_length; i++)
    {
        QImageReader t_reader;
        QByteArray ba = t_reader.imageFormat(allImagesInfo.at(i).absoluteFilePath());
        QFileInfo fileInfo;
        fileInfo = QFileInfo(allImagesInfo.at(i).absoluteFilePath());
        QString fileSuffix = fileInfo.suffix().toLower();

        if(format.contains(ba) || format.contains(fileSuffix))
        {
            if(allImagesInfo.at(i).absoluteFilePath() == orignalFile)
            {
                currentNum = i;
            }

            allImagesName.append(allImagesInfo.at(i).fileName());
        }
        else
        {
            //因为m_currentfile一定是图片文件此处用if用不到了
            if(orignalFile == allImagesInfo.at(i).absoluteFilePath())
            {
                //如果当前文件为空 寻找下一张是图片的文件
                int index = i;
                if(i+1 == t_length)
                {
                    index = 0;
                }
                else
                {
                    index++;
                }

                orignalFile = allImagesInfo.at(index).absoluteFilePath();
                currentNum = i;
            }

            allImagesInfo.removeAt(i);
            t_length--;
            i--;
        }
    }
    imagesNum = allImagesInfo.length();
}
