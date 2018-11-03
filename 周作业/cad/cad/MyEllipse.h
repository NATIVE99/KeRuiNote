#pragma once
#include "MyChape.h"
#include <QGraphicsEllipseItem >
class CMyEllipse:public CMyChape
{
    Q_OBJECT
public:
    CMyEllipse();
    CMyEllipse(const CMyEllipse& obj) {
        this->m_PointBegin = obj.m_PointBegin;
        this->m_PointEnd = obj.m_PointEnd;
    }
    ~CMyEllipse();
    void  OnDrow(QPainter& paint);
    bool Contains(const QPointF &point) const;
};
Q_DECLARE_METATYPE(CMyEllipse);
