#pragma once
#include <QPoint>
#include <Qpainter>
#include <QGraphicsRectItem>
#include "MyChape.h"
class CMyRect:public CMyChape
{
    Q_OBJECT
public:
    CMyRect(){}
    CMyRect(QPoint pBegin, QPoint pEnd);
    CMyRect(const CMyRect& obj) {
        this->m_PointBegin = obj.m_PointBegin;
        this->m_PointEnd = obj.m_PointEnd;
    }
    ~CMyRect();
    void OnDrow(QPainter& paint);
    bool Contains(const QPointF &point) const;
private:

};

Q_DECLARE_METATYPE(CMyRect);