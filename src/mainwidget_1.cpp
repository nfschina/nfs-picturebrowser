#include "mainwidget_1.h"

mainWidget_1::mainWidget_1(QWidget *parent, Qt::WindowFlags f)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAutoFillBackground(true);
    setAcceptDrops(true);


    m_nameLabel = new QLabel;
    m_nameLabel->setPixmap(QPixmap(tr(":/prefix1/1-images/LOGO.png")));
    m_nameLabel->adjustSize();

    m_openfile = new QPushButton(tr("打开图片"));
    m_propagandaLabel = new QLabel(tr("小巧轻快 随心浏览..."));

    m_openfile->setFixedSize(170, 50);
    m_openfile->setFocusPolicy(Qt::NoFocus);
    m_openfile->setToolTip(tr("打开图片"));

    QFont ft;
    ft.setPointSize(25);
    m_nameLabel->setFont(ft);
    m_nameLabel->adjustSize();

    ft.setPointSize(15);
    m_propagandaLabel->setFont(ft);
    m_propagandaLabel->adjustSize();

    m_vl1 = new QVBoxLayout;
    m_vl1->addStretch(20);
    m_vl1->addWidget(m_nameLabel,0,Qt::AlignHCenter);
    m_vl1->addStretch(8);
    m_vl1->addWidget(m_openfile,0,Qt::AlignHCenter);
    m_vl1->addStretch(3);
    m_vl1->addWidget(m_propagandaLabel,0,Qt::AlignHCenter);
    m_vl1->addStretch(10);

    m_vl1->setMargin(0);
    m_widget = new QWidget;
    m_widget->setFixedSize(300, 275);
    m_widget->setLayout(m_vl1);

    vlOfMainWidget = new QVBoxLayout;
    vlOfMainWidget->addWidget(m_widget, 0, Qt::AlignCenter);
    vlOfMainWidget->setMargin(0);
    vlOfMainWidget->setSpacing(0);

    this->setLayout(vlOfMainWidget);

    mainWidgetSetStyleSheet();

    connect(m_openfile, SIGNAL(clicked()), this, SLOT(slot_clickedOpenfile()));
}

mainWidget_1::~mainWidget_1()
{

}

void mainWidget_1::mouseDoubleClickEvent(QMouseEvent *event)
{
}
void mainWidget_1::paintEvent(QPaintEvent *e)
{
    QPalette t_pal;
    t_pal.setBrush(QPalette::Window,QColor(235,241,255));
    this->setPalette(t_pal);
}

void mainWidget_1::dragEnterEvent(QDragEnterEvent *e)
{
    if(e->mimeData()->hasUrls())
    {
        e->accept();
        e->acceptProposedAction();
    }
    else
    {
        e->ignore();
    }
}

void mainWidget_1::dropEvent(QDropEvent *e)
{
    QString localFile;

    foreach(QUrl url, e->mimeData()->urls())
    {
        localFile = url.toLocalFile();

        QImageReader t_reader/*(localFile)*/;
        QByteArray ba = t_reader.imageFormat(localFile);
        //打包文件也能返回aa.tar.gz

        if(!ba.isEmpty())
        {
            sig_openFile(localFile);
            break;
        }
        else
        {
            QStringList format;
            format << "bmp" << "gif" << "jpg"<< "jpeg" << "png"
                   << "pbm" << "pgm" << "ppm" << "xbm" << "xpm";

            int index = localFile.lastIndexOf(".");

            if(index >= 0 && format.contains(localFile.right(localFile.length() - index - 1), Qt::CaseSensitive))
            {
                QString text = tr("无法载入图片\"")+ localFile + "\"\n" +
                        tr("分析:图片文件损坏...");
                myMessageBox t_msgBox(QString(tr("取消")), QString(tr("确定")),
                                      QString(tr("覆盖原图")), false,
                                      QPixmap(":/prefix1/1-images/error.png"),
                                      text, this);

                t_msgBox.exec();
            }

        }
    }
}

void mainWidget_1::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
}

void mainWidget_1::mouseMoveEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
}

void mainWidget_1::slot_clickedOpenfile()
{     
     QString defaultFolder;
     defaultFolder=QDir::homePath();
    //added by huj 20160226
     defaultFolder.append("/图片");//set picture to be the default DIR
    // QString fileName = openFileDialog(this, defaultFolder);
     QString fileName;
     filedialog=new QFileDialog(this);
            QStringList select_file_list;

            filedialog->setWindowModality(Qt::WindowModal);
            filedialog->setWindowTitle(tr("打开图片"));
            filedialog->setAcceptMode(QFileDialog::AcceptOpen);
            filedialog->setFileMode(QFileDialog::ExistingFile);
            filedialog->setViewMode(QFileDialog::Detail);
            filedialog->setDirectory(defaultFolder);
            filedialog->setFilter(QDir::Files);
            QStringList filters; 
	    QLocale locale;
	              if( locale.language() == QLocale::English ) 
            filters <<tr("Image(*.bmp  *.BMP  *.bitmap  *.gif  *.tif   *.tiff  *.jpg  *.JPG  *.jpeg *.JPEG  *.jpeg2000  *.jpc  *.j2k  *.jpf  *.png  *.PNG  *.pbm *.pgm *.psd  *.jp2  *.ico  *.icns)");
		      else
            filters <<"图像(*.bmp  *.BMP  *.bitmap  *.gif  *.tif   *.tiff  *.jpg  *.JPG  *.jpeg *.JPEG  *.jpeg2000  *.jpc  *.j2k  *.jpf  *.png  *.PNG  *.pbm *.pgm *.psd  *.jp2  *.ico  *.icns)";
            filedialog->setNameFilters(filters);
	    foreach (QObject *obj, filedialog->children())
     	    {
              if (obj->objectName() == "buttonBox" )
           {
                 QDialogButtonBox * buttonBox;
                 buttonBox = qobject_cast<QDialogButtonBox *>(obj);

                 buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("取消"));

            }
      }

            if(filedialog->exec())
            {
                select_file_list=filedialog->selectedFiles();

                fileName=select_file_list.at(0).toLocal8Bit().constData();
            }
            if(!(QFileInfo(fileName).isFile())&&(fileName.trimmed()!=""))//判断是不是文件并且没有取消选择文件
            {
                fileName= "";
            }


    //打开文件
    if(!fileName.isEmpty())
    {
        sig_openFile(fileName);
        //deleteMember();
    }
}

void mainWidget_1::mainWidgetSetStyleSheet()
{
    m_nameLabel->setStyleSheet("color:#aaaaaa;");
    m_propagandaLabel->setStyleSheet("color:grey");

    m_openfile->setStyleSheet("QPushButton{background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6699FF, stop:1 #6699FF);"
                               "color: rgb(255 , 255, 255);"
                               "border-radius: 7px;"
                               "font:19px;}"
                               "QPushButton:hover{background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgb(152, 203, 255), stop:1 rgb(102, 153, 255));}");
}
