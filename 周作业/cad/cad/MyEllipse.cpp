#include "MyEllipse.h"
#include <qdebug>


CMyEllipse::CMyEllipse()
{
}


CMyEllipse::~CMyEllipse()
{
}

void  CMyEllipse::OnDrow(QPainter& paint)
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
    paint.drawEllipse(m_PointBegin.x(), m_PointEnd.y(), m_PointEnd.x() - m_PointBegin.x(), m_PointEnd.y() - m_PointBegin.y());
}
bool CMyEllipse::Contains(const QPointF &point) const
{
    QRectF rectF(m_PointBegin, m_PointEnd);
    QGraphicsEllipseItem EllipseItem(rectF);

    return EllipseItem.contains(point);

}