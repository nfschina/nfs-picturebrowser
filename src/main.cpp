#include "widget.h"

#include <QCoreApplication>
#include <QStyleFactory>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QStringList>
#include <QDebug>
#include <QDir>

#include "define.h"

//全局配置
QString configList[ROWS];
QString globalParameter;

void setConfig();
void resetConfig(int);

int main(int argc, char *argv[])
{
      //QTranslator tran;
      //bool ok=tran.load("qt_zh_CN.qm","/usr/share/cdos-picturebrowser/application/cdos-picturebrowser/translations/");
    //设置风格
      Application a(argc, argv);
      //a.installTranslator(&tran);
      //a.setStyle(QStyleFactory::create("fusion"));

      QTranslator translator;
      QTranslator translator2;
      QTranslator translatorprinter;

      QLocale locale;
         if( locale.language() == QLocale::English )  //靠靠靠靠
         {// localnetchat_en.qm /usr/share/LocalNetChat
             qDebug() << "English system" ;
             translator.load(QString("/usr/share/cdos-picturebrowser/application/cdos-picturebrowser/translations/cdos-picturebrowser_en.qm"));  //靠靠靠
             a.installTranslator(&translator);
         }
         else{
                 translator2.load("/usr/share/qt5/translations/qtbase_zh_CN.qm");
                 a.installTranslator(&translator2);
                 translator.load("/usr/share/cdos-picturebrowser/application/cdos-picturebrowser/translations/qt_zh_CN.qm");
                 a.installTranslator(&translator);
		 translatorprinter.load("/usr/share/qt5/translations/qt_zh_CN.qm");
		 a.installTranslator(&translatorprinter);
                 a.setStyle(QStyleFactory::create("fusion"));
         }

    //a.setKeyboardInputInterval(10);

    QDir configDir(QDir::homePath() + "/" + PICTUREBROWSER);
    if(!configDir.exists())
        configDir.mkdir(QDir::homePath() + "/" + PICTUREBROWSER);

    globalParameter=argv[1];

    //读配置文件

    QFile file(configDir.absolutePath() + "/" + "config");
    if(!file.exists())
    {
        setConfig();

        if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
            std::cout << "open file failed for read" << std::endl;

        QTextStream in(&file);
        //写ROWS行
        for(int i=0; i<ROWS; i++)
        {
            if(i < ROWS-1)
                in << configList[i] << "\n";
            else
                in << configList[i];
        }

        file.close();
    }
    else
    {
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
            std::cout << "open file failed for write" << std::endl;

        QTextStream out(&file);

        QString t_text = out.readAll();
        file.close();

        QStringList t_list = t_text.split("\n", QString::SkipEmptyParts);

        if(t_list.length() != ROWS)
        {
            setConfig();
        }
        else
        {
            for(int i=0; i < ROWS; i++)
            {
                QString row = t_list.at(i);

                switch (i) {
                case 0:
                    if(row.length() != 6) //6 代表常规配置的项数
                        resetConfig(0);
                    else
                    {
                        for(int j=0; j<6; j++)
                        {
                            if(row.at(j) != QChar('1') || row.at(j) != QChar('1'))
                                row.replace(j, QChar('0')); //如果不是1或0 则置0
                        }

                        configList[0] = row;
                    }
                    break;

                case 1:
                    if(row.length() != 1)
                        resetConfig(1);
                    else
                    {
                        if(row.at(0) != QChar('1') && row.at(0) != QChar('0'))
                            resetConfig(1);
                        else
                            configList[1] = row;
                    }
                    break;

                case 2:
                    if(row.length() == 3)
                    {
                    //    if(row.toInt() >= QApplication::desktop()->availableGeometry().width() ||
                      //          row.toInt() < MINWEITH)
                        //    resetConfig(2);
                       // else
                            configList[2] = row;
                    }
                    else if(row.length() == 4)
                    {
                        if(row.at(0) == QChar('0'))
                            resetConfig(2);
                        else
                        {
                         //   if(row.toInt() >= QApplication::desktop()->availableGeometry().width() ||
                          //          row.toInt() < MINWEITH)
                            //    resetConfig(2);
                            //else
                                configList[2] = row;
                        }
                    }
                    else
                        resetConfig(2);
                    break;

                case 3:
                    if(row.length() == 3)
                    {
                       // if(row.toInt() >= QApplication::desktop()->availableGeometry().height() ||
                         //       row.toInt() < MINHEIGTH)
                           // resetConfig(3);
                       // else
                            configList[3] = row;
                    }
                    else if(row.length() == 4)
                    {
                        if(row.at(0) == QChar('0'))
                            resetConfig(3);
                        else
                        {
                           // if(row.toInt() >= QApplication::desktop()->availableGeometry().height() ||
                             //       row.toInt() < MINHEIGTH)
                               // resetConfig(3);
                           // else
                                configList[3] = row;
                        }
                    }
                    else
                        resetConfig(3);

                    break;

                case 4:
                    if(row.length() != 1)
                        resetConfig(4);
                    else
                    {
                        if(row.at(0) != QChar('1') || row.at(0) != QChar('1'))
                            resetConfig(4);
                        else
                            configList[4] = row;
                    }
                    break;

                case 5:
                    if(row.toInt() != 0)
                    {
                        if(row.length() == 3)
                        {
                            for(int j=0; j<3; j++)
                            {
                               char c = row.at(j).toLatin1();

                               switch (j) {
                               case 0:
                                   if(c > '9' || c < '1')
                                        row.replace(0, QChar('3'));
                                   break;

                               case 1:
                                   if(c > '4' || c < '0')
                                        row.replace(1, QChar('0'));
                                   break;

                               case 2:
                                   if(c > '1' || c < '0')
                                        row.replace(2, QChar('0'));
                                   break;

                               default:
                                   break;
                               }
                            }

                            configList[5] = row;
                        }
                        else if(row.length() == 4)
                        {
                            for(int j=0; j<4; j++)
                            {
                               char c = row.at(j).toLatin1();

                               switch (j) {
                               case 0:
                                   if(c != '1')
                                        row.replace(0, QChar('1'));
                                   break;

                               case 1:
                                   if(c != '0')
                                        row.replace(1, QChar('0'));
                                   break;

                               case 2:
                                   if(c > '4' || c < '0')
                                        row.replace(2, QChar('0'));
                                   break;

                               case 3:
                                   if(c > '1' || c < '0')
                                        row.replace(3, QChar('0'));
                                   break;

                               default:
                                   break;
                               }

                            }

                            configList[5] = row;
                        }
                        else
                            resetConfig(5);
                    }
                    else
                        resetConfig(5);
                    break;

                default:
                    break;
                }
            }
        }

    }

#if 0
    QFile styleSheet(":/css/style.css");
    if (!styleSheet.open(QIODevice::ReadOnly))
    {
        qWarning("Can't open the style sheet file.");
        return -1;
    }
    qApp->setStyleSheet(styleSheet.readAll());
#endif

    QFont font("SansSerif", 10, 30/*QFont::Normal*/, false);
    a.setFont(font);

    Widget w;
    w.show();

//    QFile paremeterFile(globalParameter);
 //   if(paremeterFile.exists())
   // {
     //   w.openPicture(globalParameter);
       // w.defaultSettingCheck();
   // }
	 QFileInfo fileinfo(globalParameter);
     if(fileinfo.isFile())
	 {
			 w.openPicture(globalParameter);
			 w.defaultSettingCheck();
	 }
     else if(fileinfo.isDir())
		  w.show();
    return a.exec();
}

void setConfig()
{
    configList[0] = "110100";
    configList[1] = "1";
    configList[2] = "740";
    configList[3] = "450";
    configList[4] = "1";
    configList[5] = "300";
}

void resetConfig(int i)
{
    switch (i) {
    case 0:
        configList[0] = "110100";
        break;
    case 1:
        configList[1] = "1";
        break;
    case 2:
        configList[2] = "740";
        break;
    case 3:
        configList[3] = "450";
        break;
    case 4:
        configList[4] = "1";
        break;
    case 5:
        configList[5] = "300";
        break;
    default:
        break;
    }
}
