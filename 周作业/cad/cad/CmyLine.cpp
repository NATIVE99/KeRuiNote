#include "CmyLine.h"




CmyLine::~CmyLine()
{
}

void CmyLine::OnDrow(QPainter& paint)
{
    QBrush brush;
    QPen pen;
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
    if(!m_bSelect)
    {
        paint.drawLine(m_PointBegin, m_PointEnd);
    }
    else
    {
        //QPen oldPen = paint.pen();

        //paint.setPen(Qt::green);
        paint.drawLine(m_PointBegin, m_PointEnd);
        //paint.setPen(oldPen);
    }
  
      
    

    
}

bool CmyLine::Contains(const QPointF &point)const
{
    QGraphicsLineItem lineItem(m_PointBegin.x(), m_PointBegin.y(),
        m_PointEnd.x(), m_PointEnd.y());

    return lineItem.contains(point);

}

