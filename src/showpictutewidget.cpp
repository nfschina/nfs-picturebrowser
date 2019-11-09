#include "showpictutewidget.h"

showPictuteWidget::showPictuteWidget(QWidget *parent, Qt::WindowFlags f)
    :QLabel(parent, f)
{
    this->setMouseTracking(true);

    // 图片适应label大小
    this->setScaledContents(true);

    m_value = 0;
    m_isMousePressed = false;

}

showPictuteWidget::~showPictuteWidget()
{

}

void showPictuteWidget::showPictuteWidgetSetPixmap(QString filename)
{
    QImageReader imgReader(filename);
    imgReader.setScaledSize(QSize(500, 300));
    QImage image;

    if (imgReader.read(&image))
    {
        this->setPixmap(QPixmap::fromImage(image));
        this->adjustSize();
    }
}

void showPictuteWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(!m_isMousePressed)
    {
        sig_mapToParentPos(mapToParent(e->pos()));
    }

    if (e->buttons() & Qt::LeftButton)
    {
        this->move(mapToParent(e->pos())- m_dragPosition);
        e->accept();
    }
}

void showPictuteWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        m_dragPosition = e->pos();
        m_isMousePressed = true;
        e->accept();
    }
}

void showPictuteWidget::mouseReleaseEvent(QMouseEvent *e)
{
    m_isMousePressed = false;
}

void showPictuteWidget::wheelEvent(QWheelEvent *e)
{
    if(e->delta() > 0)
    {
        m_value += 5;
        this->resize(this->width()+m_value, this->height()+m_value);
    }
    else
    {
        m_value -= 5;
        this->resize(this->width()+m_value, this->height()+m_value);
    }
}

void showPictuteWidget::resizeEvent(QResizeEvent *e)
{

    int t_width = this->width();
    int t_heigth = this->height();

    //图片置中
    this->setGeometry((this->parentWidget()->width() - t_width)/2, (this->parentWidget()->height() - t_heigth)/2,
                      t_width, t_heigth);
}

