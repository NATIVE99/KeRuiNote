#include "MyMoveCmd.h"



CMyMoveCmd::CMyMoveCmd(std::shared_ptr<CMyChape> shape, QVector<std::shared_ptr< CMyChape>>* list)
{
    m_shape = shape;
    m_list = list;
}


CMyMoveCmd::~CMyMoveCmd()
{
}



//ִ�з���
void CMyMoveCmd::execute()
{
    m_list->push_back(m_shape);
}
//��������
void CMyMoveCmd::undo()
{

    m_list->removeOne(m_shape);
}