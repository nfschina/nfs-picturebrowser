#ifndef FILEINFOTHREAD
#define FILEINFOTHREAD

#include <QThread>
#include <QFileInfo>
#include <QDir>
#include <QImageReader>

class fileInfoThread:public QThread
{
    Q_OBJECT
public:
    fileInfoThread();

    QString orignalFile;
    QStringList allImagesName;                    //所有图片的名字
    QFileInfoList allImagesInfo;                  //所有图片文件信息

    int imagesNum;       //图片总个数
    int currentNum;       //但前显示图片在列表中的编号


protected:
    void run();
};



#endif // FILEINFOTHREAD

