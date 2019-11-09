#include "imagereader.h"
#include <QtDebug>
#include<sys/types.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

ImageReader::ImageReader(bool v) : QObject()
{
    verbose = v;
    gmfiles = "";
    qtfiles = "";
    extrasfiles = "";

    fileFormats = new FileFormats;
    fileFormats->getFormats();

    gmfiles=fileFormats->formatsGmEnabled.join(",");
    qtfiles=fileFormats->formatsQtEnabled.join(",");
//    extrasfiles=fileFormats->formatsExtrasEnabled.join(",");

}

QImage ImageReader::readImage(QString filename)
{
    // Which GraphicsEngine should we use?
    QString whatToUse = whatDoIUse(filename);

// Try to use GraphicsMagick (if available)
     if(whatToUse == "gm")
     {
        return readImage_GM(filename);
     }
     else if(whatToUse == "qt")
     {
        return readImage_QT(filename);
     }
     else
     {
         return readImage_QT(":/prefix2/2-images/plainerrorimg.png");
     }
}

QImage ImageReader::readImage_QT(QString filename)
{
    // For reading SVG files
    QSvgRenderer svg;
    QPixmap svg_pixmap;

    // For all other supported file types
    QImageReader reader;

    // Return image
    QImage img;

    // Suffix, for easier access later-on
    QString suffix = QFileInfo(filename).suffix().toLower();

    if(suffix == "svg")
    {
        // Loading SVG file
        svg.load(filename);

        // Invalid vector graphic
        if(!svg.isValid()) {
            std::cerr << "[reader svg] Error: invalid svg file" << std::endl;
            QPixmap pix(":/prefix2/2-images/plainerrorimg.png");
            QPainter paint(&pix);
            QTextDocument txt;
            txt.setHtml("<center><div style=\"text-align: center; font-size: 12pt; font-wight: bold; color: white; background: none;\">ERROR LOADING IMAGE<br><br><bR>The file doesn't contain valid a vector graphic</div></center>");
            paint.translate(100,150);
            txt.setTextWidth(440);
            txt.drawContents(&paint);
            paint.end();
            fileformat = "";
            origSize = pix.size();
            scaleImg1 = -1;
            scaleImg2 = -1;
            animatedImg = false;
            return pix.toImage();
        }

        // Render SVG into pixmap
        svg_pixmap = QPixmap(svg.defaultSize());
        svg_pixmap.fill(Qt::transparent);
        QPainter painter(&svg_pixmap);
        svg.render(&painter);

        // Store the width/height for later use
        origSize = svg.defaultSize();
        // Store the fileformat for later use
        fileformat = "SVG";

    }
    else if(suffix == "tga")
    {
        QPixmap pic(filename);
        return pic.toImage();
    }
    else
    {
        // Setting QImageReader
        reader.setFileName(filename);
        // Store the width/height for later use
        origSize = reader.size();
        // Store the fileformat for later use
        if(QFileInfo(filename).baseName() != "nfspb_tmp")
        {
            fileformat = reader.format().toLower();
        }
        else
        {
            fileformat = "";
        }
    }

    // Finalise SVG files
    if(suffix == "svg")
    {
        // Convert pixmap to image
        img = svg_pixmap.toImage();
      }
    else
    {
        // Eventually load the image
        img = reader.read();
        // If an error occured
        if(img.isNull())
        {
            QString err = reader.errorString();
            std::cerr << "[reader qt] Error: file failed to load: " << err.toStdString() << std::endl;
            QPixmap pix(":/prefix2/2-images/plainerrorimg.png");
            QPainter paint(&pix);
            QTextDocument txt;
            txt.setHtml(QString("<center><div style=\"text-align: center; font-size: 12pt; font-wight: bold; color: white; background: none;\"><b>ERROR LOADING IMAGE</b><br><br><bR>%1</div></center>").arg(err));
            paint.translate(100,150);
            txt.setTextWidth(440);
            txt.drawContents(&paint);
            paint.end();
            fileformat = "";
            origSize = pix.size();
            scaleImg1 = -1;
            scaleImg2 = -1;
            animatedImg = false;
            return pix.toImage();
        }
    }
    this->fileChanged = filename;
    return img;
}

// If GraphicsMagick supports the file format,
QImage ImageReader::readImage_GM(QString filename)
{
/*Determine whether there is convert*/
    // We first check if xcftools is actually installed
    QProcess which;
#if QT_VERSION >= 0x050200
    which.setStandardOutputFile(QProcess::nullDevice());
#endif
    which.start("which convert");
    which.waitForFinished();
    // If it isn't -> display error
    if(which.exitCode())
    {
        std::cerr << "[reader picture format] Error: convert not found" << std::endl;
        QPixmap pix(":/prefix2/2-images/plainerrorimg.png");
        QPainter paint(&pix);
        QTextDocument txt;
        txt.setHtml("<center><div style=\"text-align: center; font-size: 12pt; font-wight: bold; color: white; background: none;\">ERROR LOADING IMAGE<br><br><bR>PhotoQt relies on 'xcftools'' to display XCF images, but it wasn't found!</div></center>");
        paint.translate(100,150);
        txt.setTextWidth(440);
        txt.drawContents(&paint);
        paint.end();
        fileformat = "";
        origSize = pix.size();
        scaleImg1 = -1;
        scaleImg2 = -1;
        animatedImg = false;
        return pix.toImage();
    }


    QString  fileSuffix,baseName,desName;//, file_path,file_suffix ;
    QFileInfo fileInfo;
    fileInfo = QFileInfo(filename);
    fileSuffix = fileInfo.suffix();
    baseName = fileInfo.baseName();
    desName = QDir::tempPath() + "/" + baseName + ".png";

    QFile desFile(desName);


    if(!desFile.exists())
    {
// Convert pics to png using convert (part of convert)
        if(fileSuffix == "psd")
        {
//convert "psd" to "png"
            psd2png(filename,desName);
        }
        else
        {
            QProcess p;
            p.execute(QString("convert \"%1\" %2").arg(filename).arg(desName));
            p.waitForFinished();
        }
    }

// And load it
    return readImage_QT(desName);

//#ifdef GM
#if 0
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    char *data = new char[file.size()];
    qint64 s = file.read(data, file.size());
    if (s < file.size()) {
        delete[] data;
        if(verbose) std::cerr << "[reader gm] ERROR reading image file data" << std::endl;
        return QImage();
    }

    Magick::Blob blob(data, file.size());
    try {
        Magick::Image image;

        QString suf = QFileInfo(filename).suffix().toLower();

        if(suf == "x" || suf == "avs")

            image.magick("AVS");

        else if(suf == "cals" || suf == "cal" || suf == "dcl"  || suf == "ras")

            image.magick("CALS");

        else if(suf == "cgm")

            image.magick("CGM");

        else if(suf == "cut")

            image.magick("CUT");

        else if(suf == "cur")

            image.magick("CUR");

        else if(suf == "acr" || suf == "dcm" || suf == "dicom" || suf == "dic")

            image.magick("DCM");

        else if(suf == "fax")

            image.magick("FAX");

        else if(suf == "ico")

            image.magick("ICO");

        else if(suf == "mono") {

            image.magick("MONO");
            image.size(Magick::Geometry(4000,3000));

        } else if(suf == "mtv")

            image.magick("MTV");

        else if(suf == "otb")

            image.magick("OTB");

        else if(suf == "palm")

            image.magick("PALM");

        else if(suf == "pfb")

            image.magick("PFB");

        else if(suf == "pict" || suf == "pct" || suf == "pic")

            image.magick("PICT");

        else if(suf == "pix"
            || suf == "pal")

            image.magick("PIX");

        else if(suf == "tga")

//            image.magick("TGA");
         return   readImage_QT(filename,rotation,zoomed,fitinwindow,maxSize,dontscale);

        else if(suf == "ttf")

            image.magick("TTF");

        else if(suf == "txt")

            image.magick("TXT");

        else if(suf == "wbm"
            || suf == "wbmp")

            image.magick("WBMP");

        else if(suf == "dib")
            return readImage_QT(filename,rotation,zoomed,fitinwindow,maxSize,dontscale);

        image.read(blob);
        Magick::Blob ob;
        image.type(Magick::TrueColorMatteType);
        image.magick("PNG");
        image.write(&ob);

        QFile out(QDir::homePath() + "/nfspb.png");
        out.open(QIODevice::WriteOnly);
        out.write(static_cast<const char*>(ob.data()), ob.length());

        return readImage_QT(QDir::homePath() + "/nfspb.png",rotation,zoomed,fitinwindow,maxSize,dontscale);



    } catch(Magick::Exception &error_) {
        delete[] data;
        std::cerr << "[reader gm] Error: " << error_.what() << std::endl;
        QPixmap pix(":/prefix2/2-images/plainerrorimg.png");
        QPainter paint(&pix);
        QTextDocument txt;
        txt.setHtml("<center><div style=\"text-align: center; font-size: 12pt; font-wight: bold; color: white; background: none;\">ERROR LOADING IMAGE<br><br><bR>" + QString(error_.what()) + "</div></center>");
        paint.translate(100,150);
        txt.setTextWidth(440);
        txt.drawContents(&paint);
        paint.end();
        pix.save(QDir::tempPath() + "/nfspb_tmp.png");
        fileformat = "";
        origSize = pix.size();
        scaleImg1 = -1;
        scaleImg2 = -1;
        animatedImg = false;
        return pix.toImage();
    }

//#endif

    return QImage();
#endif
}


#if 0
QImage ImageReader::readImage_XCF(QString filename, int rotation, bool zoomed, bool fitinwindow, QSize maxSize, bool dontscale) {
    // We first check if xcftools is actually installed
    QProcess which;
#if QT_VERSION >= 0x050200
    which.setStandardOutputFile(QProcess::nullDevice());
#endif
    which.start("which xcf2png");
    which.waitForFinished();
    // If it isn't -> display error
    if(which.exitCode()) {
        std::cerr << "[reader xcf] Error: xcftools not found" << std::endl;
        QPixmap pix(":/prefix2/2-images/plainerrorimg.png");
        QPainter paint(&pix);
        QTextDocument txt;
        txt.setHtml("<center><div style=\"text-align: center; font-size: 12pt; font-wight: bold; color: white; background: none;\">ERROR LOADING IMAGE<br><br><bR>PhotoQt relies on 'xcftools'' to display XCF images, but it wasn't found!</div></center>");
        paint.translate(100,150);
        txt.setTextWidth(440);
        txt.drawContents(&paint);
        paint.end();
        fileformat = "";
        origSize = pix.size();
        scaleImg1 = -1;
        scaleImg2 = -1;
        animatedImg = false;
        return pix.toImage();
    }

    // Convert xcf to png using xcf2png (part of xcftools)
    QProcess p;
    p.execute(QString("xcf2png \"%1\" -o %2").arg(filename).arg(QDir::homePath() + "/nfspb_tmp.png"));

    // And load it
    return readImage_QT(QDir::homePath() + "/nfspb_tmp.png",rotation,zoomed,fitinwindow,maxSize,dontscale);

}

#endif

QString ImageReader::whatDoIUse(QString filename) {

    QString use = "nfspb";

    // Check for GM (i.e., check for not qt and not extra)
//    bool usegm = true;
    QStringList qtFiles = qtfiles.split(",");

    QFileInfo fileInfo;
    fileInfo = QFileInfo(filename);
    char buf[1024];

    if(fileInfo.suffix().size() == 0)
    {
        FILE *stream;
        QString fileStr ="file -b --mime-type "+filename;
        stream = popen(fileStr.toStdString().c_str(),"r");
        fread(buf,sizeof(char),sizeof(buf),stream);
        pclose(stream);
        QString bufStr(buf);

        int index = bufStr.lastIndexOf('/');
        QString str1 = bufStr.mid(0,index);
        QString str2 = bufStr.mid(index+1);

        if(str1 == "image")
        {
            use = "qt";
        }

    }



    for(int i = 0; i < qtFiles.length(); ++i)
    {
        // We need to remove the first character of qtfiles.at(i), since that is a "*"
        if(filename.toLower().endsWith(QString(qtFiles.at(i)).remove(0,1)))
        {
            use = "qt";
        }
    }

    QStringList gmFiles = gmfiles.split(",");

    for(int i = 0; i < gmFiles.length(); ++i)
    {
        // We need to remove the first character of qtfiles.at(i), since that is a "*"
        if(filename.toLower().endsWith(QString(gmFiles.at(i)).remove(0,1)))
        {
            use = "gm";
        }
    }

     return use;

}

