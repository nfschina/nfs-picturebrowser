#include "messagebox.h"

SelectBox::SelectBox(QString title, QWidget *parent, Qt::WindowFlags f):QDialog(parent, f)
{
    setWindowFlags(Qt::FramelessWindowHint/* | Qt::Dialog*/);
    setFixedHeight(85);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);

    m_ret = -1;
    m_boxWidth = 6;    //margin==3   margin * 2  == 6;

    m_title = new QLabel(this);
    m_title->setText(title);
    m_title->adjustSize();
    m_title->setStyleSheet("font: 11px; color: white");

    QVBoxLayout *t_vl = new QVBoxLayout;
    t_vl->setMargin(0);
    t_vl->addWidget(m_title, 0, Qt::AlignCenter);

    connect(qApp, SIGNAL(sig_appMousePressed()), this, SLOT(slot_closingOperation()));

    m_hl = new QHBoxLayout;
    m_hl->setMargin(0);
    m_hl->setSpacing(2);

    m_vl = new QVBoxLayout;
    m_vl->setSpacing(0);
    m_vl->setContentsMargins(3, 0, 3, 4);
    m_vl->addLayout(t_vl);
    m_vl->addLayout(m_hl);

    setLayout(m_vl);

    m_closeBtn = new QPushButton(this);
    m_closeBtn->setFlat(true);
    m_closeBtn->setFocusPolicy(Qt::NoFocus);
    QPixmap closePix = style()->standardPixmap(QStyle::SP_TitleBarCloseButton);
    m_closeBtn->setIcon(closePix);
    m_closeBtn->setIconSize(QSize(18, 18));
    connect(m_closeBtn, SIGNAL(clicked()), this, SLOT(close()));
}

void SelectBox::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(5, 5, 5)));
    painter.drawRoundedRect(rect(), 4.0, 4.0);
}

bool SelectBox::event(QEvent *e)
{
    switch (e->type()) {
    case QEvent::Close:
        sig_ret(m_ret);
        break;

    case QEvent::Resize:
        m_closeBtn->setGeometry(width()-22, 1, 18, 18);
        break;

    case QEvent::Enter:
        disconnect(qApp, SIGNAL(sig_appMousePressed()), this, SLOT(slot_closingOperation()));
        break;

    case QEvent::Leave:
        connect(qApp, SIGNAL(sig_appMousePressed()), this, SLOT(slot_closingOperation()));
        break;

    default:
        break;
    }

    return QWidget::event(e);
}

void SelectBox::addBoxContent(const QPixmap &icon, const QString &name)
{
    m_itemList.append(new BoxContent(icon, name));
    connect(m_itemList.last(), SIGNAL(sig_clicked()), this, SLOT(slot_closingOperation()));
    m_hl->insertWidget(-1, m_itemList.last());

    m_boxWidth += CONTENT_W;

    if(m_itemList.length() > 1)
        m_boxWidth += 2; // +spacing
}

void SelectBox::slot_closingOperation()
{   
    m_ret = m_itemList.indexOf(static_cast<BoxContent *>(sender()));
    close();    
}




