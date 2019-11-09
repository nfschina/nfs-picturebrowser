#ifndef SHOWPICTUTELABEL_H
#define SHOWPICTUTELABEL_H

#include "common.h"

class showPictuteWidget : public QLabel
{
    Q_OBJECT

public:

    int m_value;
    QPoint m_dragPosition;
    bool m_isMousePressed;

    explicit showPictuteWidget(QWidget *parent=0, Qt::WindowFlags f=0);
    ~showPictuteWidget();

    void showPictuteWidgetSetPixmap(QString filename);

    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent * e);
    void resizeEvent(QResizeEvent *e);

signals:
    void sig_mapToParentPos(QPoint p);

public slots:

};

#endif // SHOWPICTUTELABEL_H
