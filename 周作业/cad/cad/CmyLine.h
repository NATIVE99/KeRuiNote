#pragma once
#include <QPoint>
#include <Qpainter>
#include <QGraphicsLineItem>
#include "MyChape.h"
class CmyLine:public CMyChape
{
    Q_OBJECT
public:
    CmyLine() {}
    ~CmyLine();
    void OnDrow(QPainter& paint);
    bool Contains(const QPointF &point) const;
    CmyLine(const CmyLine& obj) {
        this->m_PointBegin = obj.m_PointBegin;
        this->m_PointEnd = obj.m_PointEnd;
    }
};
Q_DECLARE_METATYPE(CmyLine);

