#include "loadfileobj.h"

QString openFileDialog(QWidget *_parent, const QString defaultDir)
{
    QString t_defaultDir;

    if(defaultDir.isNull())
    {
       t_defaultDir  = QDir::homePath();//指定默认打开的路径
    }
    else
    {
       t_defaultDir = defaultDir;
    }


/*
    "*.pnm *.ras *.wbmp "  //eog
*/
 /*
    *.exr *.jp2   //opencv
*/


//从默认打开的路径开始查找选择文件
    QLocale locale;
    QString  fileName;
    if( locale.language() == QLocale::English )
       fileName = QFileDialog::getOpenFileName(_parent,"打开图片",t_defaultDir,"Images (*.bmp *.BMP *.bitmap *.gif *.tif  *.tiff *.jpg *.JPG *.jpeg *.JPEG *.jpeg2000 *.jpc *.j2k *.jpf *.png *.PNG *.pbm *.pgm *.psd *.jp2 *.ico *.icns *.dib *.pcx *.xcf *.svg *.svgz *.pnm *.ras *.wbmp *.ani *.eps *.ppm *.xbm *.xpm )");
    else
         fileName = QFileDialog::getOpenFileName(_parent,"打开图片",t_defaultDir,"图像(*.bmp *.BMP *.bitmap *.gif *.tif  *.tiff *.jpg *.JPG *.jpeg *.JPEG *.jpeg2000 *.jpc *.j2k *.jpf *.png *.PNG *.pbm *.pgm *.psd *.jp2 *.ico *.icns *.dib *.pcx *.xcf *.svg *.svgz *.pnm *.ras *.wbmp *.ani *.eps *.ppm *.xbm *.xpm )");
    
    if((QFileInfo(fileName).isFile())&&(fileName.trimmed()!=""))//判断是不是文件并且没有取消选择文件
    {
        return fileName;
    }
    else
    {
        return "";
    }
}

