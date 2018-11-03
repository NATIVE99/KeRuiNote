#include "MyMoveCmd.h"



CMyMoveCmd::CMyMoveCmd(std::shared_ptr<CMyChape> shape, QVector<std::shared_ptr< CMyChape>>* list)
{
    m_shape = shape;
    m_list = list;
}


CMyMoveCmd::~CMyMoveCmd()
{
}



//执行方法
void CMyMoveCmd::execute()
{
    m_list->push_back(m_shape);
}
//撤销方法
void CMyMoveCmd::undo()
{

    m_list->removeOne(m_shape);
}