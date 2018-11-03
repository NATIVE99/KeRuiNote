#pragma once
#include <QPainter>
#include <QPoint>
#include <QColor>
class CMyChape:public QObject
{
    Q_OBJECT
public:
    CMyChape();
    ~CMyChape();
    virtual void  OnDrow(QPainter& paint)=0;
    virtual bool Contains(const QPointF &point) const = 0;
    void SetPointBegin(QPoint PointBegin);
    void SetPointEnd(QPoint PointEnd);
    void SetSelect(bool select);
    bool IsSelect();
    void WriteFile(QDataStream& ds);
    QPoint& GetPointBegin();
    QPoint& GetPointEnd();
protected:
    QPoint   m_PointBegin;
    QPoint   m_PointEnd;
    QColor   m_color;
    int      m_wigth;
    bool     m_bSelect;
    int      m_Isbrush;
};

