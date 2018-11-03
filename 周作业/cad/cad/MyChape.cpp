#include "MyChape.h"

#include <qDebug>
extern QColor g_color;
extern int g_witgh;
extern int    g_Isbrush;
CMyChape::CMyChape():m_color(g_color), m_wigth(g_witgh), m_Isbrush(g_Isbrush)
{
    m_bSelect = false;
    qDebug() << "shape" << g_Isbrush;
}


CMyChape::~CMyChape()
{
}

void CMyChape::SetPointBegin(QPoint PointBegin)
{
    m_PointBegin = PointBegin;
}
void CMyChape::SetPointEnd(QPoint PointEnd)
{
    m_PointEnd = PointEnd;
}
QPoint& CMyChape::GetPointBegin()
{
    return m_PointBegin;
}
QPoint& CMyChape::GetPointEnd()
{
    return m_PointEnd;
}
void CMyChape::SetSelect(bool select)
{
    m_bSelect = select;
}

bool CMyChape::IsSelect()
{
    return m_bSelect;
}

void CMyChape::WriteFile(QDataStream& ds)
{
    QString qstr;
    //获取类名
    qstr = metaObject()->className();
    //把保存的数据写文件
    ds << qstr
        << m_PointBegin
        << m_PointEnd;
    qDebug() << qstr;
}