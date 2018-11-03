#include "MyRect.h"



CMyRect::CMyRect(QPoint pBegin, QPoint pEnd)
{
    m_PointEnd = pEnd;
    m_PointBegin = pBegin;
}


CMyRect::~CMyRect()
{
}

void CMyRect::OnDrow(QPainter& paint)
{
    QPen pen;
    QBrush brush;
    if (m_Isbrush == 2)
    {

        brush.setColor(m_color);
        brush.setStyle(Qt::SolidPattern);
        paint.setBrush(brush);
    }
    else
    {
        brush.setStyle(Qt::NoBrush);
        paint.setBrush(brush);
    }
    pen.setColor(m_color);
    pen.setWidth(m_wigth);
    paint.setPen(pen);
    paint.drawRect(m_PointBegin.x(), m_PointBegin.y(),m_PointEnd.x()-m_PointBegin.x(), m_PointEnd.y() - m_PointBegin.y());
}

bool CMyRect::Contains(const QPointF &point) const
{

    QRectF rectF(m_PointBegin, m_PointEnd);

    QGraphicsRectItem rectItem(rectF);

    return rectItem.contains(point);
    
}
