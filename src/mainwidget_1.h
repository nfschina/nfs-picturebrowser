#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QFrame>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDropEvent>
#include <QDropEvent>
#include <QMimeData>
//#include <QDebug>
#include "common.h"
#include "loadfileobj.h"
#include "messagebox.h"
#include <QGraphicsDropShadowEffect>
#include <QFileDialog>

class mainWidget_1 : public QFrame
{
    Q_OBJECT
public:
    QLabel *m_nameLabel;
    QLabel *m_propagandaLabel;
    QPushButton *m_openfile;
    QVBoxLayout *m_vl1;
    QWidget *m_widget;
    QVBoxLayout *vlOfMainWidget;
    QFileDialog  *filedialog;

    mainWidget_1(QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~mainWidget_1();

    void mainWidgetSetStyleSheet();

    void paintEvent(QPaintEvent *e);
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

    void mouseDoubleClickEvent(QMouseEvent *event);

signals:
    void sig_openFile(QString filename);

public slots:
    void slot_clickedOpenfile();
};

#endif // MAINWIDGET_H
